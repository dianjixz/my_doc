


# 关于爱芯的mmc的系统包重新配置


在替换爱芯的 rootfs 时，如果直接替换 sdk/rootfs 中的文件，系统是无法正常启动的。换一种方法，直接替换爱芯 axp 包内 rootfs，安装后就能正常启动了。


下面是替换的详细步骤。
```bash
# sudo apt install android-sdk-libsparse-utils

axp_pag=$1  
rootfs_pag=$2

mkdir axp_tmp
unzip $axp_pag -d axp_tmp

rm axp_tmp/rootfs_sparse.ext4

img2simg $rootfs_pag axp_tmp/rootfs_sparse.ext4 4096

cd axp_tmp
zip -r ../out.zip .
cd ..

mv out.zip out.zip.axp
rm -r axp_tmp

```


```bash
rm rootfs_sparse.ext4
img2simg /home/nihao/work/img/ax630c_llm/compile-openwrt-21-rootfs-20240819.ext4 rootfs_sparse.ext4 4096
zip -r ../out.zip .
cd .. && mv out.zip out.zip.axp

# 解包simg2img sparse.img raw.img
```
/soc/scripts/usb-adb.sh start && { /usr/local/m5stack/bin/ax_usb_adb_event.sh &> /dev/null & }


还有就是爱芯平台在linux中的下载，本来是想进行协议逆向，但是考虑到工作量，暂时还不想做。




# 关于爱芯对 linux 系统的适配的记录

在移植 rootfs 过程中，遇到了一个爱芯 ddr 打印的问题。准确的来说，这个也不算问题。因为系统ddr的初始化确实需要那样做，但它在第二次启动时打印了太多的log了。
刚开始，只有新做的openwrt系统存在这个问题，爱芯自己的rootfs不存在。我以为爱芯对rootfs做了什么特殊的事情。但我排查梳理的bl、kernel、atf、rootfs时，发现了一些问题。
第一、爱芯对整个系统的适配大多数的时候只做了初始化处理，除非像必须释放资源的设别才会有关闭适配。像poweroff、reboot、这种操作，几乎没有适配。
第二、bl阶段的代码，有很多的log打印函数，没有做log打印管理，或许是我没发现。但是这真的带来的很大的困扰。为此，我强制关闭了ddr初始化代码文件中的所用打印函数。
总的来说，代码质量不算太好。不过无所谓了，几乎不会有人去动这部分代码。但遇到问题后，几乎不存在解决的机会。（对ddr上千个寄存器的初始化操作完全没有兴趣～-～）。


新产品的内存定义到4+32,emmc的储存加大带来了固件的增大。同时烧录时间也被拉长。
为此期望在尽可能的缩小固件的大小来减少固件的烧录时间。
首先尝试了减少 rootfs 大小的方式来减少烧录时间，这个做法确实生效了，预计节约了1分钟所有的烧录时间，但是这样的做法造成了文件系统和分区大小不匹配。由于是根分区，所以没有办法直接进行resize2fs
。只能尝试挂载到其他分区进行resize2fs，然后跳转会来。最快的情况需要大该半分钟，但也存在需要50多秒的尝试时间。所以这样几乎无法完成很好的扩容工作，反而增加了系统的制作难度。
我也尝试单独增加一个分区来实现这个操作，但发现，对于整个系统来讲，几乎无法有效的把整体的烧录时间降下来，反而增加了大量的工作量。
为此决定，只进行rootfs的替换，不再做其他的操作了。
慢速设备中，尝试做整体的烧录时间优化是一个非常痛苦的操作。



非常确定的是，爱芯给到我们的 sdk 应该是有问题的。主要是 ax_fb.ko 模块出问题，在启动时，我们拿到的SDK 无法加载 vfb 配置。我将友商的 ax_fb.ko 模块替换后， vfb 可以正常加载了。我重新编译了SDK，然后在SDK的那套体系下，vfb 可以正常加载了。还不知道爱芯动了那里的手脚，总之，这样的事情真的是非常的恶心。烦死了人了。
我似乎要重新检查SDK了，但是我真的不想这么做，真的好恶心人。我的时间全部浪费在这些事情上了。

2025年 05月 13日 星期二 14:31:52 CST
vfb的问题似乎好了，但是我不能理解，我在尝试复现。但貌似并没有，我不知道复现这个还要浪费我多长时间，但是第一次它确实出现了。

2025年 05月 14日 星期三 10:07:43 CST
lgx5aW95oOz6aqC5Lq677yMQVgg5ZKMIEhYIOS7luWmiOeahOecn+WdkeS6uuOAgui/meS4qumXrumi
mOern+eEtuaYr+S4gOW8gOWni+S7luS7rOS4i+eahOWll++8jOaIkeecn+eahOaYr+acjeS6hu+8
jOefpemBkyBIWCDkuIDnm7TlnZHmiJHvvIzku5blsLHmmK/kuIDkuKrlpKflnZHvvIzov5nmrKHn
nJ/lnZHnmoTmiJHml6DoqIDku6Xlr7njgILov5jopoEgbW9kdWxlIGxsbSDmsqHmnInkvb/nlKjo
v5npg6jliIbku6PnoIHvvIzkuI3nhLbmiJHnnJ/nmoTml6Dor63kuobjgIIK6L+Z5Liq6Zeu6aKY
6KaB5LuO5LiA5byA5aeL5byA5Y+RIGF4NjMwQyDlvIDlp4vjgIJIWCDmnJ/mnJvmiJHku6znmoTm
nb/lrZDog73lpJ/mmL7npLrmoYzpnaLvvIzkuo7mmK/ku5bpl67niLHoiq/opoHkuobkuIDkuKro
jqvlkI3lhbblppnnmoTooaXkuIHvvIzpgqPkuKrooaXkuIHmja7or7TmmK8gdWJ1bnR1IOeahOah
jOmdouaYvuekuumAgumFjeS7o+eggeOAguaIkeS/oeS6hu+8jOaIkeS7lOe7hueahOWvueavlOS6
huavj+S4gOWkhOeahOS7o+egge+8jOWPkeeOsOWPquaYr+WinuWKoOS6huS4gOS6m+mFjee9rumA
iemhuSAgCui/meeci+i1t+adpeS6uueVnOaXoOWus+eahO+8jOaIluiuuOaYr+WinuWKoOafkOaW
uemdoueahOWKn+iDveWQp++8jOaIkeWcqOaWsOeahFNES+S4reS5n+ayoeacieWPkeeOsOi/meS6
m+S7o+eggeeahOWtmOWcqOOAguaIkeS4gOebtOS7peS4uumCo+S6m+S7o+eggeWcqOafkOS6m+el
nuenmOeahOWcsOaWueW3peS9nOedgOOAguebtOWIsOaIkeWPkeeOsCxheDYzMGMga2l055qEIHZm
YiDkuIDnm7TmiqXplJnvvIzmmK/pnZ7luLjkuKXph43nmoTplJnor6/vvIznm7TmjqXlr7zoh7Tl
hoXmoLjltKnmuoPjgIIgIArmiJHkuYvliY3mn6XliLDkuobku5bku6znmoTmsqHmnInlvIDmlL7m
upDku6PnoIHnmoTlhoXmoLjpg6jliIbvvIzlubblj43msYfnvJbkuobku5bku6zvvIzmj5DkuqTn
u5lGQUUg5ZCO77yMRkFFIOihqOekuuaXoOiDveS4uuWKm++8jOS7luS5n+eci+S4jeWIsOWOn+Wn
i+S7o+eggeOAguebtOWIsOaIkeS4gOmBjeS4gOmBjeeahOWvueavlOa1i+ivle+8jOe8qeWwj+iM
g+WbtO+8jOe7iOS6juWPkeeOsOS6hui/meS4quWdkeeCueOAggrlj6rog73or7TvvIxIWCDkuI3l
j6/kv6HvvIzku5bnu5nnmoTkuJzopb/ln7rmnKzpg73kuI3lj6/kv6HvvIzkuI3mmK/ov5nph4zm
nInpl67popjvvIzlsLHmmK/pgqPph4zmmK/kuKrlnZHjgILmiJHmr4/lpKnpg73lnKjloavku5bn
moTlnZHjgILnnJ/ml6Dor63vvIzotbfnoIHov5jmnInlr7nmr5TmtYvor5XvvIxIWOeahOmCo+S6
m+S4nOilv++8jOi/nuS4gOS4quWvueavlOeahOS4nOilv+mDveayoeacie+8jOe6r+mdoOi/kOaw
lOi/mOaYr+aXtumXtOS4gOeCueS4gOeCueeahOWOu+aJo+OAgiAgCuWmguaenOaYr+S4muS9meaX
tumXtOi/mOeul+Wlve+8jOS9huaYr+i/meaYr+W3peS9nO+8jOaIkeimgeS4uuaIkeeahOW3peaX
tui0n+i0o++8jOS4uuiAgeadv+i0n+i0o+OAguWmguaenOecn+eahOS7luWdkeS6huaIkeWkqumV
v+aXtumXtO+8jOayoeacieS6uuS8mueQhuino+aIke+8jOiAgeadv+S5n+S4jeS8muWboOS4uuaY
r+S7luWdkeS6huaIkeiAjOaUvui/h+aIke+8jOWPquS8muS4uuaIkeaJk+S4gOS4qui+g+S9juea
hOe7qeaViO+8jOeEtuWQjuaJo+aIkeeahOmSseOAgiAgCuWPquiDveivtOS4lueVjOaYr+S4gOS4
quiNieWPsOePreWtkO+8jOWPquiDvemdoOaIkeS7rOi/meS6m+W5sua0u+eahOe8nee8neihpeih
peOAgui/mOWlveaIkei/mOiDveaKl++8jOS5n+iuuOetieWIsOWTquS4gOWkqe+8jOS7luiGqOiD
gOWIsOeci+S4jeS4iuaIkeeahOaXtuWAme+8jOaIkeWwseivpei1sOS6huOAgiAK5LuO5Lq65oCn
55qE6KeS5bqm77yM5aSn6YOo5YiG55qE6aKG5a+86ICF77yM57uE57uH6ICF6YO96KeJ5b6X5a6M
5oiQ5LiA5Liq6aG555uu5aSn5aSa6YO95piv6Ieq5bex55qE5Yqf5Yqz77yM6ICM6YKj5Lqb5bmy
5rS755qE5Lmf5Lya6KeJ5b6X5piv6Ieq5bex5bmy5LqG5rS75omN5pyJ5LqG6L+Z5Liq5oiQ5p6c
44CC5Zyo5oiR55yL5p2l77yM5Zui6Zif5bCx5YOP5Lq65L2T55qE5Zub6IKi77yM5pyJ55qE5Lq6
5om/5ouF5LqG5aSn6ISR55qE6YOo5YiG77yM5pyJ55qE5Lq65om/5ouF5LqG5Zub6IKi55qE6YOo
5YiG77yM5aSn5a62Cum9kOW/g+WNj+WKm+aJjeiDveato+W4uOeahOeUn+a0u++8jOiOt+W+l+ea
hOiDvemHj+mcgOimgeWFrOW5s+eahOaMieeFp+mcgOimgeeahOWIhue7mei6q+S9k+eahOavj+S4
gOS4que7huiDnu+8jOWmguaenOayoeacieiDvemHj++8jOi6q+S9k+WwseS8muWQnuWZrOW5sua0
u+eahOiCjOiCiee7huiDnu+8jOebtOWIsOmlv+atu+S4uuatouOAgui/meaYr+i6q+S9k+eahOac
rOiDve+8jOS5n+aYr+S4gOS4quWbouS9k++8jOS4gOS4quWFrOWPuOeahOacrOiDveOAguaIkeim
geaPkOaXqeWBmuWlveWHhuWkh++8jArlgZrkuIDkupvop4TliJLvvIzmiJHnmoTmnKrmnaXlj6ro
g73miJHoh6rlt7HljrvotJ/otKPvvIzmsqHmnInkurrog73kuLrmiJHnmoTmmI7lpKnotJ/otKPj
gIIK












