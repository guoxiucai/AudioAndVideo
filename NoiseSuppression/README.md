
# 音频降噪处理库

## 代码抽取自webrtc

https://webrtc.googlesource.com/src/+/refs/heads/master/modules/audio_processing/ns/

## 优化
1. 移除过多的依赖，精简为两个文件
2. 多采样率适配，调整支持8K，16K，32K，48K采样率
3. 定点采样（我们录制的是定点采用，实际测试比最新的webrtc里的浮点处理效果要好）

## C 使用

```
NsHandle *nsHandle = WebRtcNs_Create();

WebRtcNs_Init(nsHandle, 16000);

// 降噪等级，0~3
WebRtcNs_set_policy(nsHandle, 2);

int num_bands = 1;
int16_t *nsIn[1] = {pcm_buffer};
int16_t *nsOut[1] = {denoised_buffer};

WebRtcNs_Analyze(nsHandle, nsIn[0]);

WebRtcNs_Process(nsHandle, (const int16_t *const *) nsIn, num_bands, nsOut);

```

## 整形格式PCM与浮点格式PCM

整型格式PCM，现在终端用户用的最多的，是16位的整数。

使用浮点格式来保存音频数据，对于正确还原声音动态以及减小音效处理时产生的误差是十分有利的。但浮点运算的复杂度，要远远大于整数运算的复杂度，而且浮点格式在播放的时候还是需要把浮点数转化为整数，才能进行数模转换，而得到扬声器能够播放的模拟信号。

考虑到目前终端录制的pcm为整型格式，如果使用浮点格式处理，还需要经过两次转换，而且浮点处理会增加cpu消耗，所以最后使用了int16的接口进行降噪。


## 编译依赖环境

* python(>=2.7.11),

* cmake(3.8.x),

* NDK(android-ndk-r13b) 编译 android 需要

* 配置 `ANDROID_NDK_HOME` 环境变量指向 NDK 目录，编译 android 需要

> 支持 VS2017, VS2015, Android Studio(>3.x), Xcode(9.2+)

## 工程的生成

- 生成 iOS 工程

`$python bootstrap.py ios`

- 生成 Mac 工程

`$python bootstrap.py osx`

- 生成 windows VS2017 工程

`$python bootstrap.py win2017`

- 生成 windows VS2015 工程

`$python bootstrap.py win2015`

- 生成 Android 工程

`1. $python bootstrap.py android`

`2. $python build.py android`


## 工程的说明

 `./build/` 目录下的生成文件都由 `bootstrap.py` 脚本生成，各个平台的 SDK 工程的编译可在 `./sdk/` 目录下找对应平台的 workspace
 