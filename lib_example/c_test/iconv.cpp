    #include <iostream>  
    #include <string>  
    #include <iconv.h>  
    using namespace std;  
      
    #pragma comment(lib,”libIconv.lib”)  
      
    //编码转换，source_charset是源编码，to_charset是目标编码  
    std::string code_convert(char source_charset, char *to_charset, const std::string& sourceStr) //sourceStr是源编码字符串  
    {  
        iconv_t cd = iconv_open(to_charset, source_charset);//获取转换句柄，void类型  
        if (cd == 0)  
            return “”;  
      
        size_t inlen = sourceStr.size();  
        size_t outlen = 255;  
        char inbuf = (char)sourceStr.c_str();  
        char outbuf[255];//这里实在不知道需要多少个字节，这是个问题  
        //char outbuf = new char[outlen]; 另外outbuf不能在堆上分配内存，否则转换失败，猜测跟iconv函数有关  
        memset(outbuf, 0, outlen);  
      
        char *poutbuf = outbuf; //多加这个转换是为了避免iconv这个函数出现char()[255]类型的实参与char**类型的形参不兼容  
        if (iconv(cd, &inbuf, &inlen, &poutbuf,&outlen) == -1)  
            return “”;  
      
        std::string strTemp(outbuf);//此时的strTemp为转换编码之后的字符串  
        iconv_close(cd);  
        return strTemp;  
    }  
      
    //gbk转UTF-8    
    std::string GbkToUtf8(const std::string& strGbk)// 传入的strGbk是GBK编码   
    {  
        return code_convert(“gb2312”, “utf-8”,strGbk);  
    }  
      
    //UTF-8转gbk  
    std::string Utf8ToGbk(const std::string& strUtf8)  
    {  
        return code_convert(“utf-8”, “gb2312”, strUtf8);  
    }  
      
    //gbk转unicode,”UCS-2LE”代表unicode小端模式  
    std::string GbkToUnicode(const std::string& strGbk)// 传入的strGbk是GBK编码   
    {  
        return code_convert(“gb2312”, “UCS-2LE”,strGbk);  
    }  
      
    //unicode转gbk  
    std::string UnicodeToGbk(const std::string& strGbk)// 传入的strGbk是GBK编码   
    {  
        return code_convert(“UCS-2LE”, “gb2312”,strGbk);  
    }  
      
    int main()   
    {  
       //1、ANSI/GBK编码    
        string strGbk = ”我”;    
        int num = strGbk.size();//获取两个字符数，也是我字所占的字节数    
        
        unsigned char p = (unsigned char)strGbk.c_str();        
        for (int i = 0; i < num; i++)        
        {        
            printf(”%0x”, p);        
            p++;        
        }  //输出ced2 所以我的GBK编码是0xced2    
        printf(”\n”);       
        
        char gbk[] = {0xce, 0xd2, 0x00}; //加上0x00字符串结束符,不会输出乱码    
        cout<<gbk<<endl;//输出汉字我    
        
        
        //2、unicode编码    
        string strUnicode = GbkToUnicode(”我”);//转成unicode编码    
        num = strUnicode.size();  
        p = (unsigned char)strUnicode.c_str();        
        for (int i = 0; i < num; i++)        
        {        
            printf(”%0x”, p);        
            p++;        
        }  //输出1162 因为默认是小端模式，所以我的unicode编码是0x6211    
        printf(”\n”);       
        
        char unicode[]= {0x11,0x62, 0x00}; //加上0x00字符串结束符,不会输出乱码    
        cout<<UnicodeToGbk(unicode)<<endl;//需要先将unicode字符串转成gbk之后才能用cout输出    
        
        
        //3、UTF-8编码    
        string strUtf8  = GbkToUtf8(”我”);//转成utf8编码    
        num = strUtf8.size();//num=3    
        p = (unsigned char)strUtf8.c_str();        
        for (int i = 0; i < num; i++)        
        {        
            printf(”%0x”, *p);        
            p++;        
        }  //输出e68891    
        printf(”\n”);       
        
        char utf8[] = {0xe6, 0x88, 0x91,0x00}; //加上0x00字符串结束符,不会输出乱码    
        cout<<Utf8ToGbk(utf8)<<endl;//需要先将utf8字符串转成gbk之后才能用cout输出    
      
        return 0;  
    }  
