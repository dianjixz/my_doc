主要实现功能

主要想实现出的demo样式为，一台设备抓屏并且将抓到的屏幕传输到另一台设备上。主要实现的环境如下：

    windows抓屏推流
    虚拟机上搭建rtsp流媒体服务器（centos7）
    windows拉流

实现

关于代码中的 const char* url = “rtsp://192.168.83.129:8554/zyx”;

    192.168.83.129是我的虚拟机ip地址
    后面的/zyx可以随便写，但是要保证推流程序和拉流的是一致的
    ：8554是默认的端口，不用改

1 windows抓屏推流（vs2019）
``` c++
#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <stdio.h>
#include<conio.h>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")
#define __STDC_CONSTANT_MACROS
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }
//引入头文件
extern "C"
{
#include "libavformat/avformat.h"
	//引入时间
#include "libavutil/time.h"
#include <libavutil/opt.h>
#include <libavutil/avutil.h>
#include <libavdevice/avdevice.h>
#include <libswscale/swscale.h>

}
//引入库
#pragma comment(lib,"avformat.lib")
//工具库，包括获取错误信息等
#pragma comment(lib,"avutil.lib")
//编解码的库
#pragma comment(lib,"avcodec.lib")

#pragma comment(lib,"avdevice.lib")
#pragma comment(lib,"swscale.lib")
int main(int argc, char* argv[])
{
	//推流部分
	AVFormatContext* pInputFormatContext = NULL;
	AVCodec* pInputCodec = NULL;
	AVCodecContext* pInputCodecContex = NULL;

	AVFormatContext* pOutputFormatContext = NULL;
	AVCodecContext* pOutCodecContext = NULL;
	AVCodec* pOutCodec = NULL;
	AVStream* pOutStream = NULL;

	av_register_all();

	avformat_network_init();

	avdevice_register_all();

	//const char* out_file = "rtmp://121.5.5.221:1935/rtmp_live/zyx";
	const char* out_file = "rtsp://192.168.83.129:8554/zyx";   //推流地址
	int ret, i;
	int videoindex = -1;
	//输入（Input） 抓屏部分设置
	pInputFormatContext = avformat_alloc_context();
	AVDictionary* options = NULL;
	AVInputFormat* ifmt = av_find_input_format("gdigrab");
	av_dict_set(&options, "framerate", "25", 0);
	av_dict_set(&options,"video_size","1920x1080",0);
	av_dict_set(&options, "start_time_realtime", 0, 0);
	if (avformat_open_input(&pInputFormatContext, "desktop", ifmt, &options) != 0) { //Grab at position 10,20 真正的打开文件,这个函数读取文件的头部并且把信息保存到我们给的AVFormatContext结构体中
		printf("Couldn't open input stream.\n");
		getchar();
		return -1;
	}
	if ((ret = avformat_find_stream_info(pInputFormatContext, 0)) < 0) {
		printf("Failed to retrieve input stream information");
		getchar();
		return -1;
	}
	av_dump_format(pInputFormatContext, 0, "desktop", 0);
	for (i = 0; pInputFormatContext->nb_streams; i++)
		if (pInputFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			cout << "pInputFormatContext=" << pInputFormatContext->streams[i]->codec->bit_rate << endl;
			videoindex = i;
			break;
		}

	//输入流解码器初始化
	pInputCodecContex = pInputFormatContext->streams[videoindex]->codec;
	pInputCodecContex->flags |= 0x00080000;
	pInputCodec = avcodec_find_decoder(pInputCodecContex->codec_id);
	if (pInputCodec == NULL)
	{
		printf("Codec not found.\n");
		getchar();
		return -1;
	}
	//打开解码器
	if (avcodec_open2(pInputCodecContex, pInputCodec, NULL) < 0)
	{
		printf("Could not open codec.\n");
		getchar();
		return -1;
	}
	//为一帧图像分配内存
	AVFrame* pFrame;
	AVFrame* pFrameYUV;
	pFrame = av_frame_alloc();
	pFrameYUV = av_frame_alloc();//为转换来申请一帧的内存(把原始帧->YUV)
	pFrameYUV->format = AV_PIX_FMT_YUV420P;
	pFrameYUV->width = pInputCodecContex->width;
	pFrameYUV->height = pInputCodecContex->height;
	unsigned char* out_buffer = (unsigned char*)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pInputCodecContex->width, pInputCodecContex->height));
	//现在我们使用avpicture_fill来把帧和我们新申请的内存来结合
	avpicture_fill((AVPicture*)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pInputCodecContex->width, pInputCodecContex->height);
	struct SwsContext* img_convert_ctx;
	img_convert_ctx = sws_getContext(pInputCodecContex->width, pInputCodecContex->height, pInputCodecContex->pix_fmt, pInputCodecContex->width, pInputCodecContex->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
	//=============================================================================================
	//输出流配置
	avformat_alloc_output_context2(&pOutputFormatContext, NULL, "rtsp", out_file); //RTMP  定义一个输出流信息的结构体
	if (!pOutputFormatContext) {
		printf("Could not create output context\n");
		ret = AVERROR_UNKNOWN;
		getchar();
		return -1;
	}
	av_opt_set(pOutputFormatContext->priv_data, "rtsp_transport", "tcp", 0);
	//输出流编码器配置
	pOutCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
	if (!pOutCodec) {
		printf("Can not find encoder! \n");
		getchar();
		return -1;
	}
	pOutCodecContext = avcodec_alloc_context3(pOutCodec);
	//pOutCodecContext->flags |= 0x00080000;
	//像素格式,
	pOutCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;
	//size
	pOutCodecContext->width = pInputCodecContex->width;
	pOutCodecContext->height = pInputCodecContex->height;
	//目标码率
	pOutCodecContext->bit_rate = 4000000;
	//每250帧插入一个I帧,I帧越小视频越小
	pOutCodecContext->gop_size = 10;
	//Optional Param B帧
	pOutCodecContext->max_b_frames = 0;  //设置B帧为0，则DTS与PTS一致

	pOutCodecContext->time_base.num = 1;
	pOutCodecContext->time_base.den = 25;

	//pOutCodecContext->lmin = 1;
	//pOutCodecContext->lmax = 50;
	//最大和最小量化系数
	pOutCodecContext->qmin = 10;
	pOutCodecContext->qmax = 51;
	if (pOutputFormatContext->oformat->flags & AVFMT_GLOBALHEADER)
		pOutCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

	//avcodec_parameters_from_context(pOutStream->codecpar, pInputCodecContex);

	AVDictionary* param = 0;
	//    //H.264
	//av_opt_set(pOutCodecContext->priv_data, "preset", "superfast", 0);
	av_opt_set(pOutCodecContext->priv_data, "tune", "zerolatency", 0);//实时编码


	if (avcodec_open2(pOutCodecContext, pOutCodec, &param) < 0)
	{
		printf("Failed to open encoder! \n");
		getchar();
		return -1;
	}
	pOutStream = avformat_new_stream(pOutputFormatContext, pOutCodec);
	if (pOutStream == NULL)
	{
		printf("Failed create pOutStream!\n");
		getchar();
		return -1;
	}
	pOutStream->time_base.num = 1;
	pOutStream->time_base.den = 25;
	pOutStream->codec = pOutCodecContext;
	
	//写文件头
	int r = avformat_write_header(pOutputFormatContext, NULL);
	if (r < 0)
	{
		printf("Failed write header!\n");
		getchar();
		return -1;
	}

	AVPacket* packet = (AVPacket*)av_malloc(sizeof(AVPacket));
	int got_picture;

	AVPacket pkt;
	int picture_size = avpicture_get_size(pOutCodecContext->pix_fmt, pOutCodecContext->width, pOutCodecContext->height);
	av_new_packet(&pkt, picture_size);

	int frame_index = 0;
	while ((av_read_frame(pInputFormatContext, packet)) >= 0)
	{
		printf("to av_read_frame! \n");
		if (packet->stream_index == videoindex)
		{

			//真正解码,packet to pFrame
			 avcodec_decode_video2(pInputCodecContex, pFrame, &got_picture, packet);
			//printf("真正解码,packet to pFrame! \n%d", got_picture);
			if (got_picture)
			{
				sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pInputCodecContex->height, pFrameYUV->data, pFrameYUV->linesize);
				pFrameYUV->pts = frame_index;
				
				int picture;
				//真正编码pkt
				int ret = avcodec_encode_video2(pOutCodecContext, &pkt, pFrameYUV, &picture);
				//pkt.pts = frame_index;
				//pkt.dts = pkt.pts;
				if (ret < 0) {
					printf("Failed to encode! \n");
					getchar();
					return -1;
				}
				if (picture == 1)
				{
					pkt.stream_index = pOutStream->index;
					AVRational time_base = pOutStream->time_base;//{ 1, 1000 };
					AVRational r_framerate1 = { 50, 2 };//{ 50, 2 };
					int64_t calc_pts = (double)frame_index * (AV_TIME_BASE) * (1 / av_q2d(r_framerate1));
					pkt.pts = av_rescale_q(calc_pts, { 1, AV_TIME_BASE }, time_base);  //enc_pkt.pts = (double)(framecnt*calc_duration)*(double)(av_q2d(time_base_q)) / (double)(av_q2d(time_base));
					pkt.dts = pkt.pts;
					frame_index++;
					ret = av_interleaved_write_frame(pOutputFormatContext, &pkt);
					//av_dump_format(pOutputFormatContext, 0, out_file, 1);

					if (ret < 0) {
						printf("Error muxing packet\n");
						break;
					}

					av_free_packet(&pkt);
				}
			}
		}
		av_free_packet(packet);
	}

	//Write file trailer
	av_write_trailer(pOutputFormatContext);

	sws_freeContext(img_convert_ctx);
	//fclose(fp_yuv);
	av_free(out_buffer);
	av_free(pFrameYUV);
	av_free(pFrame);
	avcodec_close(pInputCodecContex);
	avformat_close_input(&pInputFormatContext);

	avcodec_close(pOutStream->codec);
	av_free(pOutCodec);
	avcodec_close(pOutCodecContext);
	avformat_free_context(pOutputFormatContext);


	return 0;
}





```


3 windows拉流并显示

``` c++
#include <stdio.h>
#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <conio.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")
#define __STDC_CONSTANT_MACROS
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }       
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include "SDL2/SDL.h"
};

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) //必要
//Refresh Event
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

#define SFM_BREAK_EVENT  (SDL_USEREVENT + 2)
int thread_exit=0;

int sfp_refresh_thread(void *opaque){
	thread_exit=0;
	while (!thread_exit) {
		SDL_Event event;
		event.type = SFM_REFRESH_EVENT;
		SDL_PushEvent(&event);
		SDL_Delay(1);
	}
	thread_exit=0;
	//Break
	SDL_Event event;
	event.type = SFM_BREAK_EVENT;
	SDL_PushEvent(&event);

	return 0;
}


int main(int argc, char* argv[])
{
	//------------SDL----------------
	int screen_w,screen_h;
	SDL_Window *screen; 
	SDL_Renderer* sdlRenderer;
	SDL_Texture* sdlTexture;
	SDL_Rect sdlRect;
	SDL_Thread *video_tid;
	SDL_Event event;

	struct SwsContext *img_convert_ctx;

	//const char* url = "rtmp://121.5.5.221:1935/rtmp_live/zyx";
    const char* url = "rtsp://192.168.83.129:8554/zyx";
	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();
	avdevice_register_all();


	//AVInputFormat* ifmt = av_find_input_format("gdigrab");
	AVDictionary* avdic = NULL;
	char option_key[] = "rtsp_transport";
	char option_value[] = "tcp";
	av_dict_set(&avdic, option_key, option_value, 0);
	/*char option_key2[] = "max_delay";
	char option_value2[] = "0";
	av_dict_set(&avdic, option_key2, option_value2, 0);*/
	
	if (avformat_open_input(&pFormatCtx, url, 0, &avdic) != 0) {
		printf("Couldn't open input stream.\n");
		return -1;
	}
	
	if(avformat_find_stream_info(pFormatCtx,NULL)<0){
		printf("Couldn't find stream information.\n");
		return -1;
	}
	videoindex=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++) 
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
			videoindex=i;
			break;
		}
	if(videoindex==-1){
		printf("Didn't find a video stream.\n");
		return -1;
	}
	pCodecCtx=pFormatCtx->streams[videoindex]->codec;
	pCodecCtx->flags |= 0x00080000;
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	
	if(pCodec==NULL){
		printf("Codec not found.\n");
		return -1;
	}
	if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
		printf("Could not open codec.\n");
		return -1;
	}

	pFrame=av_frame_alloc();
	pFrameYUV=av_frame_alloc();
	out_buffer=(uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 
		pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 
	

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return -1;
	} 
	//SDL 2.0 Support for multiple windows
	screen_w = 0.8*pCodecCtx->width;
	screen_h = 0.8 *pCodecCtx->height;
	//screen = SDL_CreateWindow("Simplest ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		//screen_w, screen_h,SDL_WINDOW_OPENGL);
	screen = SDL_CreateWindow("Simplest ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_w , screen_h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if(!screen) {  
		printf("SDL: could not create window - exiting:%s\n",SDL_GetError());  
		return -1;
	}
	sdlRenderer = SDL_CreateRenderer(screen, -1, 0);  
	//IYUV: Y + U + V  (3 planes)
	//YV12: Y + V + U  (3 planes)
	sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,pCodecCtx->width,pCodecCtx->height);  

	sdlRect.x=0;
	sdlRect.y=0;
	sdlRect.w=screen_w;
	sdlRect.h=screen_h;

	packet=(AVPacket *)av_malloc(sizeof(AVPacket));

	video_tid = SDL_CreateThread(sfp_refresh_thread,NULL,NULL);
	//------------SDL End------------
	//Event Loop
	
	for (;;) {
		//Wait
		SDL_WaitEvent(&event);
		if(event.type==SFM_REFRESH_EVENT){
			//------------------------------
			if(av_read_frame(pFormatCtx, packet)>=0){
				if(packet->stream_index==videoindex){
					ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
					if(ret < 0){
						printf("Decode Error.\n");
						//return -1;
					}
					if(got_picture){
						sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
						//SDL---------------------------
						SDL_UpdateTexture( sdlTexture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0] );  
						SDL_RenderClear( sdlRenderer );  
						//SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );  
						SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, NULL);  
						SDL_RenderPresent( sdlRenderer );  
						//SDL End-----------------------
					}
				}
				av_free_packet(packet);
			}else{
				//Exit Thread
				break;
				thread_exit=1;
			}
		}else if(event.type==SDL_QUIT){
			break;
			thread_exit=1;
		}else if(event.type==SFM_BREAK_EVENT){
			break;
		}

	}

	sws_freeContext(img_convert_ctx);

	SDL_Quit();
	//--------------
	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}



```


centos7上ffmpeg的搭建与命令行的推流

centos7上ffmpeg环境搭建

yum install epel-release
rpm -v --import http://li.nux.ro/download/nux/RPM-GPG-KEY-nux.ro
rpm -Uvh http://li.nux.ro/download/nux/dextop/el7/x86_64/nux-dextop-release-0-5.el7.nux.noarch.rpm
yum install ffmpeg ffmpeg-devel

ffmpeg -version//出来版本即安装成功

    1
    2
    3
    4
    5
    6

//利用命令行直接抓屏推流
ffmpeg -f x11grab -s 800x600 -framerate 12 -i :0.0+100,200 -preset ultrafast -s 800x600 -threads 10 -codec:v h264 -f rtsp rtsp://localhost:8554/zyx


————————————————
版权声明：本文为CSDN博主「傲娇小二哈」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_39668905/article/details/127230041



2 虚拟机上搭建rtsp流媒体服务器（centos7）
https://github.com/aler9/rtsp-simple-server/releases


for((;;)); do \
        ./objs/ffmpeg/bin/ffmpeg -re -i ./doc/source.200kbps.768x320.flv \
        -vcodec copy -acodec copy \
        -f flv -y rtmp://192.168.1.170/live/livestream; \
        sleep 1; \
    done

ffmpeg -re -i  -vcodec copy -acodec copy -f flv -y rtmp://192.168.2.76:8080/live/livestream