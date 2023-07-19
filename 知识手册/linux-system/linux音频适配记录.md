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







stm32mp135调试成功了，由于不熟悉linux的驱动着一块，所以调试起来的时候比较费劲。
记录一下成功的操作。

音频驱动分为三大块
声卡，解码器，硬件端口。

在音频系统中，解码器主要有两种，一种是不需要控制的，直接对其输出音频数据就能播放。另一种是需要进行控制（寄存器写入）
音频通信总线一般是 i2s。

其中配置也要分三部分，三部分联合才能完成声音系统的匹配。
下面是stm32mp135直接输出i2s音频的设备树配置
``` c
	sound {
		compatible = "audio-graph-card";
		label = "STM32MP135F-DK";
		//routing =
		//	"Playback" , "MCLK",
		//	"Capture" , "MCLK",
		//	"MICL" , "Mic Bias";
		
		dais = <&sai1a_port &sai1b_port>;
		status = "okay";
	};
	OutToPins_codec: OutToPins_codec@0 {
		compatible = "OutToPins_codec";
		#reg = <0>;
		#sound-dai-cells = <0>;
		clocks = <&sai1a>;
		clock-names = "MCLK";
		status = "okay";

		OutToPins_port: port {
			#address-cells = <1>;
			#size-cells = <0>;

			OutToPins_tx0_endpoint: endpoint@0 {
							reg = <0>;
							remote-endpoint = <&sai1a_endpoint>;
							//frame-master;
							//bitclock-master;
			};

			OutToPins_tx1_endpoint: endpoint@1 {
							reg = <1>;
							remote-endpoint = <&sai1b_endpoint>;
							//frame-master;
							//bitclock-master;
			};
		};
    };
&sai1 {
	clocks = <&rcc SAI1>, <&scmi_clk CK_SCMI_PLL3_Q>, <&scmi_clk CK_SCMI_PLL3_R>;
	clock-names = "pclk", "x8k", "x11k";

    pinctrl-names = "default", "sleep";
	pinctrl-0 = <&sai1a_pins_mx>, <&sai1b_pins_mx>;
	pinctrl-1 = <&sai1a_sleep_pins_mx>, <&sai1b_sleep_pins_mx>;
    // pinctrl-0 = <&sai2a_pins_a>, <&sai2b_pins_b>;
    // pinctrl-1 = <&sai2a_sleep_pins_a>, <&sai2b_sleep_pins_b>;
    status = "okay";
};

&sai1a{
	#clock-cells = <0>;
	dma-names = "tx";
	status = "okay";

	sai1a_port: port {
		sai1a_endpoint: endpoint {
			remote-endpoint = <&OutToPins_tx0_endpoint>;
			format = "i2s";
			//format = "lsb";
			//mclk-fs = <128>;
			dai-tdm-slot-num = <4>;
			dai-tdm-slot-width = <32>;
		};
	};
};
 
	&sai1b{
		dma-names = "tx";
		// st,sync = <&sai1a 1>;
		// clocks = <&rcc SAI1_K>, <&sai1a>;
		// clock-names = "sai_ck", "MCLK";
		status = "okay";

		sai1b_port: port {
			sai1b_endpoint: endpoint {
				remote-endpoint = <&OutToPins_tx1_endpoint>;
				format = "i2s";
				//format = "lsb";
				//mclk-fs = <128>;
				dai-tdm-slot-num = <4>;
				dai-tdm-slot-width = <32>;
			};
		};
	};

```
上面的配置主要分为三部分：

sound 是用来配置声卡的，其中也包含了声音路由（虽然这个真的很烦人）。
这里面有一个比较大的坑就是 routing 。这个就是声音路由（万恶来源！）。
关于这个路由没有明确的说法。但是在135上不用填写这个。


OutToPins_codec 是用来配置解码器的，在这一块不同的芯片有不同的写法，不过主要分为两类，一类是挂载在i2c总线上的，一类是单独存在的，
着一块没什么坑的，不过比较烦的在 linux 的 5.15 版本中没有针对 i2s 直接输出的驱动了。在这里有点想骂写音频系统的人了，没事乱改什么接口，
觉得头发多吗？

sai1 这个是音频的硬件接口。标准音频接口是 i2s ，但是在不同的设备上有叫不同的名字的。比如 stm32的就是 sai 。在其他设备上就是 i2s。
看厂家的说法是这个接口可以兼容多种输出，反正没有想象中那么好用。

针对音频总线 i2s 。我个人的感觉，i2s 不属于纯粹的总线，它更多的是用来传递数字音频信息的。没有总线一说。
其实针对这个i2s比较烦的，高速的 i2c 完全可以当音频线用了，干嘛非要开发i2s,占用的线多，又没有特别的优势。

``` c
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Based on generic Bluetooth SCO link
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include <sound/soc.h>

static const struct snd_soc_dapm_widget OutToPins_widgets[] = {
	SND_SOC_DAPM_INPUT("RX"),
	SND_SOC_DAPM_OUTPUT("TX"),
};

static const struct snd_soc_dapm_route OutToPins_routes[] = {
	{ "Capture", NULL, "RX" },
	{ "TX", NULL, "Playback" },
};

static struct snd_soc_dai_driver OutToPins_dai[] = {
	{
		.name = "OutToPins-pcm",
		.playback = {
			.stream_name = "Playback",
			.channels_min = 1,
			.channels_max = 4,
 			.rates = SNDRV_PCM_RATE_CONTINUOUS,
 			/* DMA does not support 24 bits transfers */
 			.formats =
 				SNDRV_PCM_FMTBIT_S16_LE |
 				SNDRV_PCM_FMTBIT_S32_LE,
		},
		.capture = {
			 .stream_name = "Capture",
			.channels_min = 1,
			.channels_max = 4,
 			.rates = SNDRV_PCM_RATE_CONTINUOUS,
 			/* DMA does not support 24 bits transfers */
 			.formats =
 				SNDRV_PCM_FMTBIT_S16_LE |
 				SNDRV_PCM_FMTBIT_S32_LE,
		},
	},

};

static int OutToPins_of_xlate_dai_id(struct snd_soc_component *component,
				   struct device_node *endpoint)
{
	/* return dai id 0, whatever the endpoint index */
	return 0;
}

static const struct snd_soc_component_driver soc_component_dev_OutToPins = {
	.dapm_widgets		= OutToPins_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(OutToPins_widgets),
	.dapm_routes		= OutToPins_routes,
	.num_dapm_routes	= ARRAY_SIZE(OutToPins_routes),
	.of_xlate_dai_id	= OutToPins_of_xlate_dai_id,
	.idle_bias_on		= 1,
	.use_pmdown_time	= 1,
	.endianness		= 1,
	.non_legacy_dai_naming	= 1,
};

static int OutToPins_probe(struct platform_device *pdev)
{
	printk("fuck fuck you!----------------\r");
	return devm_snd_soc_register_component(&pdev->dev,
				      &soc_component_dev_OutToPins,
				      OutToPins_dai, ARRAY_SIZE(OutToPins_dai));
}

static int OutToPins_remove(struct platform_device *pdev)
{
	return 0;
}


#if defined(CONFIG_OF)
static const struct of_device_id OutToPins_codec_of_match[] = {
	{ .compatible = "OutToPins_codec", },
	{},
};
MODULE_DEVICE_TABLE(of, OutToPins_codec_of_match);
#endif

static struct platform_driver OutToPins_driver = {
	.driver = {
		.name = "OutToPins_codec",
		.of_match_table = of_match_ptr(OutToPins_codec_of_match),
	},
	.probe = OutToPins_probe,
	.remove = OutToPins_remove,
};

module_platform_driver(OutToPins_driver);

MODULE_AUTHOR("martin.lesniak@st.com");
MODULE_DESCRIPTION("ASoC generic output driver");
MODULE_LICENSE("GPL");

```











