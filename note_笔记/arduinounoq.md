# arduino UNO Q
最近用 ai 比较多了，想着ai比人聪明呢，结果搞到最后了，还是需要我手动去操作。

查到的信息：
arduino UNO Q使用Boot Loader Specification 的启动引导。

devicetree-overlay 可以用来添加dtbo 的 dts 补充，这很有用。
arduino UNO Q 不支持使用 devicetree-overlay。


https://www.digikey.cn/zh/forum/t/topic/6509
系统烧烧录。


      sudo fdtoverlay \
    -i /boot/efi/dtb/qcom/qrb2210-arduino-imola-base.dtb \
    -o /boot/efi/dtb/qcom/qrb2210-arduino-imola-jd9852.dtb \
    `pwd`/jd9852a.dtbo
    
     devicetree /dtb/qcom/qrb2210-arduino-imola-jd9852.dtb
    
    sudo modetest -M msm -s 34:170x320
    sudo modetest -M msm -c
    sudo modetest -M msm -s 34:720x1280
    sudo modetest -M msm -s 34:480x480



















































Boot Loader Specification 的启动引导：
