RK系列SDK -- dummy codec虚拟声卡注册
https://blog.csdn.net/hb9312z/article/details/103315401


基于NXP iMX6ULL 扩展音频解码器 MAX98357A
https://www.xjx100.cn/news/402172.html?action=onClick

Linux ALSA音频框架及RK3399 DTS音频配置
https://blog.csdn.net/pingis58/article/details/121604216?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522168952225016800186598910%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=168952225016800186598910&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-4-121604216-null-null.142^v88^control_2,239^v2^insert_chatgpt&utm_term=simple-audio-card&spm=1018.2226.3001.4187


STM32MP157驱动开发——Linux 音频驱动
https://blog.csdn.net/weixin_45682654/article/details/128489323?spm=1001.2101.3001.6650.14&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-14-128489323-blog-80642091.235%5Ev38%5Epc_relevant_default_base3&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-14-128489323-blog-80642091.235%5Ev38%5Epc_relevant_default_base3&utm_relevant_index=19

Linux驱动开发|音频驱动
https://blog.csdn.net/Chuangke_Andy/article/details/122494425?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-0-122494425-blog-128489323.235^v38^pc_relevant_default_base3&spm=1001.2101.3001.4242.1&utm_relevant_index=3

linux ALSA 驱动架构
https://www.shili8.cn/article/detail_20000380669.html


Openwrt开发之声卡RT5670挂载
https://www.cnblogs.com/weishengzhong/p/11103935.html

RK3399 Linux4.4 Audio （simple-card）开发指南 
http://blog.itpub.net/31557861/viewspace-2216415/



RK3399 Audio驱动讲解
https://blog.csdn.net/u013463707/article/details/118089695

Jetson Nano使用PCM5102音频DAC模块
https://www.jianshu.com/p/e7e84f48b653






















``` c
codec_ext: max98357a@0 {
    compatible = "maxim,max98357a";
    #sound-dai-cells = <0>;
};

sound {
    compatible = "simple-audio-card";
    status = "okay";
    simple-audio-card,name = "max98357a";

    simple-audio-card,format = "i2s";
    simple-audio-card,bitclock-master = <&dailink_master_cpu>;
    simple-audio-card,frame-master = <&dailink_master_cpu>;

    simple-audio-card,codec {
        sound-dai = <&codec_ext>;
    };

    dailink_master_cpu: simple-audio-card,cpu {
        sound-dai = <&sai2>;
    };

};


&sai2 {
    pinctrl-names = "default";
    pinctrl-0 = <&pinctrl_sai2>;

    assigned-clocks = <&clks IMX6UL_CLK_SAI2_SEL>,
              <&clks IMX6UL_CLK_PLL4_AUDIO_DIV>;
    assigned-clock-parents = <&clks IMX6UL_CLK_PLL4_AUDIO_DIV>;
    assigned-clock-rates = <2>, <196608000>;

    fsl,sai-asynchronous;
    /*fsl,sai-mclk-direction-output;*/
    status = "okay";
};


```





STM32MP157驱动开发——Linux 音频驱动

https://blog.csdn.net/weixin_45682654/article/details/128489323?spm=1001.2101.3001.6650.14&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-14-128489323-blog-80642091.235%5Ev38%5Epc_relevant_default_base3&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-14-128489323-blog-80642091.235%5Ev38%5Epc_relevant_default_base3&utm_relevant_index=19



``` c
&i2c4 {
	pinctrl-names = "default", "sleep";
	pinctrl-0 = <&i2c4_pins_a>;
	pinctrl-1 = <&i2c4_pins_sleep_a>;
	status = "okay";
	/delete-property/dmas;
	/delete-property/dma-names;
	cs42l51: cs42l51@4a {
		compatible = "cirrus,cs42l51";
		reg = <0x4a>;
		#sound-dai-cells = <0>;
		VL-supply = <&v3v3>;
		VD-supply = <&v1v8_audio>;
		VA-supply = <&v1v8_audio>;
		VAHP-supply = <&v1v8_audio>;
		reset-gpios = <&gpioz 7 GPIO_ACTIVE_LOW>;
		clocks = <&sai2a>;
		clock-names = "MCLK";
		status = "okay";
		
		cs42l51_port: port {
			#address-cells = <1>;
			#size-cells = <0>;
			
			cs42l51_tx_endpoint: endpoint@0 {
				reg = <0>;
				remote-endpoint = <&sai2a_endpoint>;
				frame-master;
				bitclock-master;
			};
				
			cs42l51_rx_endpoint: endpoint@1 {
				reg = <1>;
				remote-endpoint = <&sai2b_endpoint>;
				frame-master;
				bitclock-master;
			};
		};
	};
};
v1v8_audio: regulator-v1v8-audio {
	compatible = "regulator-fixed";
	regulator-name = "v1v8_audio";
	regulator-min-microvolt = <1800000>;
	regulator-max-microvolt = <1800000>;
	regulator-always-on;
	regulator-boot-on;
};
&sai2 {
	clocks = <&rcc SAI2>, <&rcc PLL3_Q>, <&rcc PLL3_R>;
	clock-names = "pclk", "x8k", "x11k";
	pinctrl-names = "default", "sleep";
	pinctrl-0 = <&sai2a_pins_a>, <&sai2b_pins_b>;
	pinctrl-1 = <&sai2a_sleep_pins_a>, <&sai2b_sleep_pins_b>;
	status = "okay";
	
	sai2a: audio-controller@4400b004 {
		#clock-cells = <0>;
		dma-names = "tx";
		clocks = <&rcc SAI2_K>;
		clock-names = "sai_ck";
		status = "okay";
		
		sai2a_port: port {
			sai2a_endpoint: endpoint {
				remote-endpoint = <&cs42l51_tx_endpoint>;
				format = "i2s";
				mclk-fs = <256>;
				dai-tdm-slot-num = <2>;
				dai-tdm-slot-width = <32>;
			};
		};
	};
			
	sai2b: audio-controller@4400b024 {
		dma-names = "rx";
		st,sync = <&sai2a 2>;
		clocks = <&rcc SAI2_K>, <&sai2a>;
		clock-names = "sai_ck", "MCLK";
		status = "okay";
	
		sai2b_port: port {
			sai2b_endpoint: endpoint {
				remote-endpoint = <&cs42l51_rx_endpoint>;
				format = "i2s";
				mclk-fs = <256>;
				dai-tdm-slot-num = <2>;
				dai-tdm-slot-width = <32>;
			};
		};
	};
};

sound: sound {
	compatible = "audio-graph-card";
	label = "STM32MP1-DK";
	routing =
		"Playback" , "MCLK",
		"Capture" , "MCLK",
		"MICL" , "Mic Bias";
	dais = <&sai2a_port &sai2b_port>;
	status = "okay";
};


```


























