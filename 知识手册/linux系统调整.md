ubuntu-创建桌面快捷方式
====================
第一步：/usr/share/applications目录下创建桌面配置文件mytest.desktop

``` bash
[Desktop Entry]
Name=mytest //应用程序名称
GenericName=mytest //应用程序通用名称
Comment=Qt mytest //应用程序简要描述
Exec=/usr/bin/untitled //可执行文件绝对路径
Terminal=flase //是否开启一个终端执行
Type=Application //快捷方式类型为应用
Icon=/home/forlinx/imgs/rtc-icon.png //图标文件绝对路径
Encoding=UTF-8 //编码格式
```

第二步：将可执行文件拷贝到/usr/bin目录下
第三步：将图标文件拷贝到/home/forlinx/imgs目录下
第四步：将/usr/bin/applications目录下图标拷贝到桌面
————————————————
版权声明：本文为CSDN博主「Paper_Love」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_42952079/article/details/125381405


linux的内存使用。
/proc/sys/vm/min_free_kbytes 是 Linux 内核中一个用于管理内存的参数，它用来指定系统中保留的最小空闲内存量，以确保系统在内存紧张时有足够的空闲内存来处理紧急情况。这个参数主要用于避免系统在内存不足时出现严重的问题，例如无法响应请求、无法分配新的进程等。

当系统中的空闲内存量低于 min_free_kbytes 时，Linux 内核会尝试采取一些措施来释放内存，以确保系统有足够的内存可用。这些措施可能包括清理缓存和缓冲区、回收未使用的内存页等。目标是尽量避免内存不足导致系统崩溃或性能下降。

min_free_kbytes 参数的默认值通常是一个比较保守的值，它会在系统启动时根据系统配置自动设置。你可以通过修改该参数来调整系统在内存紧张时的行为，但是需要慎重考虑，确保不会对系统的稳定性和性能造成负面影响。
min_free_kbytes设置过大时会导致过量依赖交换区内存，无法完全使用内存，导致性能下降。





+---PARTITION: Phys(0x44E00000, 0x4FFFFFFF), Size=182272KB(178MB),    NAME="anonymous"
 nBlock(Max=74, Cur=74, New=69, Free=8)  nbytes(Max=133554176B(130424KB,127MB), Cur=133554176B(130424KB,127MB), New=133410816B(130284KB,127MB), Free=12763136B(12464KB,12MB))  Block(Max=46837760B(45740KB,44MB), Min=4096B(4KB,0MB), Avg=1135668B(1109KB,1MB)) 
   |-Block: phys(0x44E00000, 0x44E3FFFF), cache =non-cacheable, length=256KB(0MB),    name="venc_ko"
   |-Block: phys(0x44E40000, 0x44E7FFFF), cache =non-cacheable, length=256KB(0MB),    name="venc_ko"
   |-Block: phys(0x44E80000, 0x44E80FFF), cache =non-cacheable, length=4KB(0MB),    name="venc_ko"
   |-Block: phys(0x44E81000, 0x44EC0FFF), cache =non-cacheable, length=256KB(0MB),    name="jenc_ko"
   |-Block: phys(0x44EC1000, 0x44F00FFF), cache =non-cacheable, length=256KB(0MB),    name="jenc_ko"
   |-Block: phys(0x44F01000, 0x44F01FFF), cache =non-cacheable, length=4KB(0MB),    name="jenc_ko"
   |-Block: phys(0x44F02000, 0x44F05FFF), cache =non-cacheable, length=16KB(0MB),    name="TDP_DEV"
   |-Block: phys(0x44F06000, 0x44F07FFF), cache =non-cacheable, length=8KB(0MB),    name="GDC CMD"
   |-Block: phys(0x44F08000, 0x4506BFFF), cache =non-cacheable, length=1424KB(1MB),    name="comm_pool_0"
   |-Block: phys(0x4506C000, 0x4537DFFF), cache =non-cacheable, length=3144KB(3MB),    name="comm_pool_1"
   |-Block: phys(0x4537E000, 0x4715AFFF), cache =non-cacheable, length=30580KB(29MB),    name="comm_pool_2"
   |-Block: phys(0x4715B000, 0x4782AFFF), cache =non-cacheable, length=6976KB(6MB),    name="comm_pool_3"
   |-Block: phys(0x4782B000, 0x4A4D5FFF), cache =non-cacheable, length=45740KB(44MB),    name="comm_pool_4"
   |-Block: phys(0x4A4D6000, 0x4AC9AFFF), cache =cacheable, length=7956KB(7MB),    name="joint_w_null"
   |-Block: phys(0x4AC9B000, 0x4AE33FFF), cache =non-cacheable, length=1636KB(1MB),    name="npu_m_null"
   |-Block: phys(0x4AE34000, 0x4B027FFF), cache =non-cacheable, length=2000KB(1MB),    name="npu_rb_null"
   |-Block: phys(0x4B028000, 0x4B0BDFFF), cache =non-cacheable, length=600KB(0MB),    name="SAMPLE-CV"
   |-Block: phys(0x4B0BE000, 0x4B1E9FFF), cache =non-cacheable, length=1200KB(1MB),    name="SAMPLE-CV"
   |-Block: phys(0x4B1EA000, 0x4B315FFF), cache =non-cacheable, length=1200KB(1MB),    name="SAMPLE-CV"
   |-Block: phys(0x4B316000, 0x4B441FFF), cache =non-cacheable, length=1200KB(1MB),    name="joint"
   |-Block: phys(0x4B442000, 0x4B48CFFF), cache =non-cacheable, length=300KB(0MB),    name="joint"
   |-Block: phys(0x4B48D000, 0x4B49FFFF), cache =non-cacheable, length=76KB(0MB),    name="joint"
   |-Block: phys(0x4B4A0000, 0x4B4A5FFF), cache =cacheable, length=24KB(0MB),    name="cam_pdb_t"
   |-Block: phys(0x4B4A6000, 0x4B4E5FFF), cache =non-cacheable, length=256KB(0MB),    name="dmabuf_glb"
   |-Block: phys(0x4B4E6000, 0x4B4E6FFF), cache =cacheable, length=4KB(0MB),    name="isp_pipe_pdb_t"
   |-Block: phys(0x4B4E7000, 0x4B4E7FFF), cache =cacheable, length=4KB(0MB),    name="isp_chn_pdb_t"
   |-Block: phys(0x4B4E8000, 0x4B4F0FFF), cache =cacheable, length=36KB(0MB),    name="unpack-buffer"
   |-Block: phys(0x4B4F1000, 0x4B514FFF), cache =cacheable, length=144KB(0MB),    name="temp-chn"
   |-Block: phys(0x4B515000, 0x4B530FFF), cache =cacheable, length=112KB(0MB),    name="temp-scale"
   |-Block: phys(0x4B531000, 0x4B5A4FFF), cache =cacheable, length=464KB(0MB),    name="3a-init"
   |-Block: phys(0x4B5A5000, 0x4B5C2FFF), cache =cacheable, length=120KB(0MB),    name="rltm-hist"
   |-Block: phys(0x4B5C3000, 0x4B628FFF), cache =cacheable, length=408KB(0MB),    name="joint_w_null"
   |-Block: phys(0x4B629000, 0x4B74AFFF), cache =non-cacheable, length=1160KB(1MB),    name="npu_m_null"
   |-Block: phys(0x4B74B000, 0x4B74BFFF), cache =non-cacheable, length=4KB(0MB),    name="k_drv_ctx"
   |-Block: phys(0x4B74C000, 0x4B74CFFF), cache =non-cacheable, length=4KB(0MB),    name="npu_m_null"
   |-Block: phys(0x4B74D000, 0x4B754FFF), cache =cacheable, length=32KB(0MB),    name="joint_w_null"
   |-Block: phys(0x4B755000, 0x4B75FFFF), cache =non-cacheable, length=44KB(0MB),    name="npu_m_null"
   |-Block: phys(0x4B760000, 0x4B7CEFFF), cache =cacheable, length=444KB(0MB),    name="rltm-curve"
   |-Block: phys(0x4B7CF000, 0x4B83DFFF), cache =cacheable, length=444KB(0MB),    name="rltm-curve"
   |-Block: phys(0x4B83E000, 0x4B840FFF), cache =cacheable, length=12KB(0MB),    name="GDC MESH"
   |-Block: phys(0x4B841000, 0x4BC40FFF), cache =non-cacheable, length=4096KB(4MB),    name="dmabuf_pipe_0"
   |-Block: phys(0x4BC41000, 0x4BD21FFF), cache =non-cacheable, length=900KB(0MB),    name="gp_isp_npu"
   |-Block: phys(0x4BD22000, 0x4BD22FFF), cache =non-cacheable, length=4KB(0MB),    name="tBuf"
   |-Block: phys(0x4BD23000, 0x4BEA6FFF), cache =non-cacheable, length=1552KB(1MB),    name="user_pool_0"
   |-Block: phys(0x4BEA7000, 0x4BEF7FFF), cache =non-cacheable, length=324KB(0MB),    name="user_pool_1"
   |-Block: phys(0x4BEF8000, 0x4BF42FFF), cache =non-cacheable, length=300KB(0MB),    name="user_pool_2"
   |-Block: phys(0x4BF43000, 0x4C00EFFF), cache =cacheable, length=816KB(0MB),    name="user_pool_3"
   |-Block: phys(0x4C00F000, 0x4C01DFFF), cache =cacheable, length=60KB(0MB),    name="user_pool_4"
   |-Block: phys(0x4C01E000, 0x4C03DFFF), cache =non-cacheable, length=128KB(0MB),    name="jenc_ewl"
   |-Block: phys(0x4C03E000, 0x4C19FFFF), cache =non-cacheable, length=1416KB(1MB),    name="venc_fifo_0"
   |-Block: phys(0x4C1A0000, 0x4C1A8FFF), cache =non-cacheable, length=36KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C1A9000, 0x4C1B1FFF), cache =non-cacheable, length=36KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C1B2000, 0x4C1BAFFF), cache =non-cacheable, length=36KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C1BB000, 0x4C1C3FFF), cache =non-cacheable, length=36KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C1C4000, 0x4C2C2FFF), cache =non-cacheable, length=1020KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C2C3000, 0x4C3C1FFF), cache =non-cacheable, length=1020KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C3C2000, 0x4C441FFF), cache =non-cacheable, length=512KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C442000, 0x4C4C1FFF), cache =non-cacheable, length=512KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C4C2000, 0x4C4C2FFF), cache =non-cacheable, length=4KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C4C3000, 0x4C4C3FFF), cache =non-cacheable, length=4KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C4C4000, 0x4C4C4FFF), cache =non-cacheable, length=4KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C4C5000, 0x4C4C5FFF), cache =non-cacheable, length=4KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C4C6000, 0x4C4C6FFF), cache =non-cacheable, length=4KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C4C7000, 0x4C4C7FFF), cache =non-cacheable, length=4KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C4C8000, 0x4C4C8FFF), cache =non-cacheable, length=4KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C4C9000, 0x4C4C9FFF), cache =non-cacheable, length=4KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C4CA000, 0x4C4CCFFF), cache =non-cacheable, length=12KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C4CD000, 0x4C4CFFFF), cache =non-cacheable, length=12KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C4D0000, 0x4C4D0FFF), cache =non-cacheable, length=4KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C4D1000, 0x4C4D1FFF), cache =non-cacheable, length=4KB(0MB),    name="venc_ewl"
   |-Block: phys(0x4C4D2000, 0x4C897FFF), cache =non-cacheable, length=3864KB(3MB),    name="REGION"
   |-Block: phys(0x4C898000, 0x4CC5DFFF), cache =non-cacheable, length=3864KB(3MB),    name="REGION"
   |-Block: phys(0x4CC5E000, 0x4CCDDFFF), cache =non-cacheable, length=512KB(0MB),    name="cv_npu"
   |-Block: phys(0x4CCDE000, 0x4CD5DFFF), cache =non-cacheable, length=512KB(0MB),    name="cv_npu"

---CMM_USE_INFO:
 total size=182272KB(178MB),used=130424KB(127MB + 376KB),remain=51848KB(50MB + 648KB),partition_number=1,block_number=74














