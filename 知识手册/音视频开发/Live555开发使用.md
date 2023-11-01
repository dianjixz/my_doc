
一、Live555RTSP推流概述

Live555是一个开源的跨平台的RTSP服务器与客户端实现，支持多种操作系统平台和编程语言，被广泛应用于视频会议、流媒体播放器、视频监控等领域。

而RTSP协议则是一种实时流传输协议，常用于音频和视频的直播传输。

因此，Live555的RTSP推流能够将实时流通过RTSP协议传输到客户端，实现音频和视频的直播传输。
二、Live555RTSP推流的安装与配置

安装：

git clone https://github.com/sobellabs/live555.git
cd live555
./genMakefiles linux
make
sudo make install

配置：

./testRTSPServer

通过上述命令启动Live555的RTSP服务器，并默认监听554端口。
三、Live555RTSP推流的实现
1、初始化环境

在调用Live555的RTSP推流之前，需要先进行环境的初始化。

UsageEnvironment* env = BasicUsageEnvironment::createNew();
TaskScheduler* scheduler = TaskScheduler::createNew();

使用BasicUsageEnvironment与TaskScheduler，可以创建一个新的Live555环境对象。
2、创建音频和视频的源

创建音频或视频的资源，是RTSP推流中的重要一环。

对于音频和视频，Live555提供了两个类：AudioFileSource和H264VideoFileSource，可以分别创建音频和H264视频的源。

AudioFileSource* audioSource = AudioFileSource::createNew(*env, audioFilePath);
H264VideoFileSource* videoSource = H264VideoFileSource::createNew(*env, videoFilePath, False);

其中其中audioFilePath和videoFilePath是音频和视频的文件路径。

注意：因为RTC协议的限制，音频必须为G.711a格式。

除此之外，还需要设置音频和视频的编码参数。

audioSource->setAudioFormatParameter(audioSamplingRate, audioChannels);
videoSource->setPacketizationMode(H264VideoStreamFramer::H264PacketizationMode::SingleNalUnit);
videoSource->setVideoFormatParameter(spsBytes, spsSize, ppsBytes, ppsSize, width, height, frameRate);

其中，audioSamplingRate是音频的采样率，audioChannels是音频的声道数，spsBytes、spsSize、ppsBytes、ppsSize分别是H264视频编码中的sps和pps参数，width、height是视频的宽高，frameRate是视频的帧率。

需要注意的是，H264视频的Packetization Mode可以设置为SingleNalUnit，这可以避免在传输过程中产生额外的NALU头。
3、创建媒体资源

将音频和视频源打包成媒体资源，并设置音频和视频的数据流ID。

MediaSession* mediaSession = MediaSession::createNew(*env, "liveSession", "liveSession description");
MediaStream* audioStream = MediaStream::createNew(*env, audioSource);
MediaStream* videoStream = MediaStream::createNew(*env, videoSource);
mediaSession->addSubsession(audioStream);
mediaSession->addSubsession(videoStream);
audioStream->setStreamId(1);
videoStream->setStreamId(2);

4、创建RTSP服务器对象

创建RTSP服务器对象，并设置RTSP认证和RTSP监听地址。

RTSPServer* rtspServer = RTSPServer::createNew(*env, listenAddr);
UserAuthenticationDatabase* authDB = new UserAuthenticationDatabase;
authDB->addUserRecord(authUserName, authPassword);
rtspServer->setAuthenticationDatabase(authDB);

5、将媒体资源添加到RTSP服务器对象中

将上述创建的媒体资源添加到RTSP服务器对象中，并为每个媒体资源设置RTSP URL。

char const* streamURLPrefix = "rtsp://";
char const* streamURLSuffix = "/trackID=";
char* audioURL = NULL;
char* videoURL = NULL;
audioURL = new char[strlen(streamURLPrefix) + strlen(rtspServerIP) + strlen(streamURLSuffix) + 5];
sprintf(audioURL, "%s%s:%d%s%d", streamURLPrefix, rtspServerIP, rtspServerPort, streamURLSuffix, 1); 
videoURL = new char[strlen(streamURLPrefix) + strlen(rtspServerIP) + strlen(streamURLSuffix) + 5];
sprintf(videoURL, "%s%s:%d%s%d", streamURLPrefix, rtspServerIP, rtspServerPort, streamURLSuffix, 2);
audioStream->getStreamSource()->setRTSPSourceParameters(rtspServer, audioURL, authUserName, authPassword);
videoStream->getStreamSource()->setRTSPSourceParameters(rtspServer, videoURL, authUserName, authPassword);

6、启动RTSP服务器

最后，通过如下代码启动Live555的RTSP服务器。

TaskToken rtspServerTask = rtspServer->createNewTask();
schedulableTask = env->taskFactory().create(&RTSPServer::serverWatchdogTask, rtspServerTask);
scheduleDelayedTask(schedulableTask, 0);  

四、Live555RTSP推流的补充说明
1、RTSP推流的常见问题

在使用Live555进行RTSP推流时，常见的问题包括音频不流畅、视频采集异常、缓存过多等情况。

一种解决方法是在客户端中对音视频数据进行限制，如限制视频码率、限制音频采样率等。
2、其他Live555的应用

Live555还可以应用于高性能音视频直播、音视频录制、视频通话等其他领域。

Live555提供了丰富的API文档和示例，开发者可以根据自身需求进行开发。
3、示例代码
1) RTSP推流示例代码：

// 创建音视频源
AudioFileSource* audioSource = AudioFileSource::createNew(*env, audioFilePath);
H264VideoFileSource* videoSource = H264VideoFileSource::createNew(*env, videoFilePath, False);
audioSource->setAudioFormatParameter(audioSamplingRate, audioChannels);
videoSource->setPacketizationMode(H264VideoStreamFramer::H264PacketizationMode::SingleNalUnit);
videoSource->setVideoFormatParameter(spsBytes, spsSize, ppsBytes, ppsSize, width, height, frameRate);

// 音视频源打包成媒体资源
MediaSession* mediaSession = MediaSession::createNew(*env, "liveSession", "liveSession description");
MediaStream* audioStream = MediaStream::createNew(*env, audioSource);
MediaStream* videoStream = MediaStream::createNew(*env, videoSource);
mediaSession->addSubsession(audioStream);
mediaSession->addSubsession(videoStream);
audioStream->setStreamId(1);
videoStream->setStreamId(2);

// 创建RTSP服务器并添加媒体资源
RTSPServer* rtspServer = RTSPServer::createNew(*env, listenAddr);
UserAuthenticationDatabase* authDB = new UserAuthenticationDatabase;
authDB->addUserRecord(authUserName, authPassword);
rtspServer->setAuthenticationDatabase(authDB);
char const* streamURLPrefix = "rtsp://";
char const* streamURLSuffix = "/trackID=";
char* audioURL = NULL;
char* videoURL = NULL;
audioURL = new char[strlen(streamURLPrefix) + strlen(rtspServerIP) + strlen(streamURLSuffix) + 5];
sprintf(audioURL, "%s%s:%d%s%d", streamURLPrefix, rtspServerIP, rtspServerPort, streamURLSuffix, 1); 
videoURL = new char[strlen(streamURLPrefix) + strlen(rtspServerIP) + strlen(streamURLSuffix) + 5];
sprintf(videoURL, "%s%s:%d%s%d", streamURLPrefix, rtspServerIP, rtspServerPort, streamURLSuffix, 2);
audioStream->getStreamSource()->setRTSPSourceParameters(rtspServer, audioURL, authUserName, authPassword);
videoStream->getStreamSource()->setRTSPSourceParameters(rtspServer, videoURL, authUserName, authPassword);

// 启动RTSP服务器
TaskToken rtspServerTask = rtspServer->createNewTask();
schedulableTask = env->taskFactory().create(&RTSPServer::serverWatchdogTask, rtspServerTask);
scheduleDelayedTask(schedulableTask, 0);  

2) 音频采集与播放示例代码：

// 创建音频采集器
TaskScheduler* scheduler = BasicTaskScheduler::createNew();
UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);
AudioLiveSource* audioSource = AudioLiveSource::createNew(*env);
DeviceParameters param;
param.device = "/dev/dsp";
param.channels = audioChannels;
param.samplerate = audioSamplingRate;
audioSource->setAudioInput(param);

// 将音频采集器添加到媒体资源中
MediaSession* mediaSession = MediaSession::createNew(*env, "mediaSession");
mediaSession->addSubsession(audioStream);

// 创建音频播放器
TaskScheduler* scheduler = BasicTaskScheduler::createNew();
UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);
AudioDeviceSource* audioOutput = AudioDeviceSource::createNew(*env);
DeviceParameters param;
param.device = "/dev/dsp";
param.channels = audioChannels;
param.samplerate = audioSamplingRate;
audioOutput->setAudioOutput(param);

// 将音频播放器添加到媒体资源中
MediaSession* mediaSession = MediaSession::createNew(*env, "mediaSession");
mediaSession->addSubsession(audioStream);

// 启动音频采集器和音频播放器
audioSource->start();
audioOutput->startPlaying();

https://www.python100.com/html/DUEL674Q834L.html