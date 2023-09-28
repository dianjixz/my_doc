TinyALSA 是一个在 Linux 内核中与 ALSA 接口的小型库。tinyalsa主要是为了解决alsa库过于庞大，同时接口繁杂，不利于用户的使用。tinyalsa主要是alsa内核的用户层音频接口，屏蔽内核的设备操作过程，为用户层提供对于设备的基本操作。具有体积小，能够满足基本点pcm采集，播放和设备控制。



1 编译调试

下载：

git clone https://github.com/tinyalsa/tinyalsa.git

编译过程比较简单，可以支持linux和嵌入式设备的交叉编译:
支持主流的make，cmake和Meson

make 
make install

注意点：
交叉编译需要修改makefile，添加编译工具链

#设置安装目录：
export PREFIX ?= $(shell pwd)/install/arm
#设置交叉编译器
export CROSS_COMPILE := arm-linux-gnueabihf-

2 API介绍

旧版本直接在在asoundlib.h提供对外接口，新版本采用的是引用mixer.h，pcm.h文件。核心的API并没有变化。主要分为几个模块：音频播放、音频采集、设备控制。

2.1 音频采集/播放

2.1.1 打开关闭设备

这里主要是对于pcm_config结构的填充，主要包括基本第一个是声卡id（采集通道id）；第二参数设备id一般是0需要根据实际调整；第三参数：设置输入PCM_IN、输出PCM_OUT；第四个参数pcm_config设置 设备的通道数，采样率，采样精度，步长等参数。

//打开或者关闭音频设备
struct pcm *pcm_open(unsigned int card, unsigned int device,
                     unsigned int flags, struct pcm_config *config);

struct pcm_config {
    unsigned int channels; //通道数
    unsigned int rate; 	   //采样率
    unsigned int period_size; //步长/分片；步长=采集返回数据量大小；如10m数据量=16000*16*10/(1000*8)=320
    unsigned int period_count;//步长内的分片
    enum pcm_format format; //采样精度，位深
	
	//默认下面阀值设置为0，采用内部默认值
    unsigned int start_threshold;
    unsigned int stop_threshold;
    unsigned int silence_threshold;

    /* Minimum number of frames available before pcm_mmap_write() will actually
     * write into the kernel buffer. Only used if the stream is opened in mmap mode
     * (pcm_open() called with PCM_MMAP flag set).   Use 0 for default.
     */
    int avail_min;//不设置
    unsigned int in_init_channels;    // 采集通道数
};

#define PCM_OUT        0x00000000
#define PCM_IN         0x10000000
#define PCM_MMAP       0x00000001
#define PCM_NOIRQ      0x00000002
#define PCM_NORESTART  0x00000004

2.1.2 关闭设备
关闭设备将打开的pcm句柄传入pcm_close。

#关闭设备将打开的pcm传入即可
int pcm_close(struct pcm *pcm);

2.1.33 采集pcm数据

采集设备pcm数据，只需要传入pcm句柄和对应的buf即可获取的设备的采集数据。

count = pcm_get_buffer_size(pcm);
data = malloc(count);
int pcm_read(struct pcm *pcm, void *data, unsigned int count);

注意点：
这里的count来自库内部pcm_get_buffer_size计算：

unsigned int pcm_get_buffer_size(struct pcm *pcm);

2.1.4 播放pcm数据
播放pcm数据，只需要传入pcm句柄和对应的buf即可向设备写数据，即实现播放。

int pcm_write(struct pcm *pcm, const void *data, unsigned int count);

2.2 设备控制

2.2.1 打开mixer管理器
打开声卡，只需要传入，声卡id即可返回一个mixer句柄。

struct mixer *mixer_open(unsigned int card);

2.2.2 关闭mixer管理器
关闭设备将打开的mixer 句柄传入mixer_close。

void mixer_close(struct mixer *mixer);

2.2.3 设备控制

设备控制主要是根据不同的模式进行设置，主要包括喇叭音量，麦克风音量和设备模式控制等等；

设置模式分为字符串和数值设置两个接口根据不同类型决定

//设置数值
int mixer_ctl_set_value(struct mixer_ctl *ctl, unsigned int id, int value);
//设置字符串MIXER_CTL_TYPE_ENUM类型
int mixer_ctl_set_enum_by_string(struct mixer_ctl *ctl, const char *string);

enum CTRL_TYPE_ENUM{
	CTRL_TYPE_AUDIO_START = 0,
	CTRL_TYPE_AUDIO_SPK_VOLUME = 0,	//喇叭音量
	CTRL_TYPE_AUDIO_MIC_VOLUME1 = 4,//麦克风音量
	CTRL_TYPE_AUDIO_MIC_VOLUME2 = 6,
	
	CTRL_TYPE_AUDIO_MODE = 16,//设备模式：手柄（handset），免提（spk），耳机（headset）等
	CTRL_TYPE_AUDIO_END = 16,

	CTRL_TYPE_VEDIO_START = 0x20000000,
	CTRL_TYPE_VEDIO_END,
};


3 使用案例
3.1 采集端

int main()
{
	int cardid = 0;
	int deviceid = 0;
	struct pcm_config config; 
	struct pcm *ppcm;
	config.channels = 1;    
	config.in_init_channels = 1;
	config.rate = 16000;    
	config.period_size = 80;    
	config.period_count = 4;             
	config.format = PCM_FORMAT_S16_LE;  
	
	config.start_threshold = 0;    
	config.stop_threshold = 0;    
	config.silence_threshold = 0;
	
	ppcm = pcm_open(card, device, PCM_IN, &config); 
	if(!ppcm || !pcm_is_ready(ppcm))
		return -1;  
	
	int size = pcm_get_buffer_size(pcm);
	int ret = 0;
	unsigned char *pbuffer = (unsigned char *)malloc(size);
	if(NULL == pbuffer)
	{
		return -1; 
	}
	while(runing_flag)
	{
		ret = pcm_read(ppcm, pbuffer, size);
		if(ret < 0)
		{
			continue ;
		}
	
		capture_callback(pbuffer, size);
	}
	
	pcm_close(ppcm);
	ppcm = NULL;
	
	return 0; 
}