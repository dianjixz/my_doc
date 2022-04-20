libIconv库实现GBK、[Unicode](https://so.csdn.net/so/search?q=Unicode&spm=1001.2101.3001.7020)、UTF-8三种编码互转比window api更为简单，而且libIconv库跨平台。

IibIconv库在windows下的编译参照[windows下使用VS编译libIconv库](http://blog.csdn.net/bladeandmaster88/article/details/54808644)
 

http://www.gnu.org/software/libiconv/可以查看支持转换的编码类型。

下面是几个关键[库函数](https://so.csdn.net/so/search?q=库函数&spm=1001.2101.3001.7020)的注释：

(1) iconv_t iconv_open(const char *tocode, const char *fromcode);
 //此函数说明将要进行哪两种编码的转换,tocode是目标编码,fromcode是原编码,
 //该函数返回一个转换句柄,供以下两个函数使用。

(2) size_t iconv(iconv_t cd,char **inbuf,size_t *inbytesleft,char **outbuf,size_t *outbytesleft);
 //此函数从inbuf中读取字符,转换后输出到outbuf中,inbytesleft用以记录还未转换的字符数,
 //outbytesleft用以记录输出缓冲的剩余空间。

(3) int iconv_close(iconv_t cd);
 //此函数用于关闭转换句柄,释放资源。

注意定义参数量的时候 inbytesleft  outbytesleft一定是 size_t 类型的,不然返回值就会是-1；

outbuf 指针会被改变,需要备份一个指针.

