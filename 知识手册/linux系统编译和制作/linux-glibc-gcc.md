linux系统中 glibc gcc linux-heard的版本依赖问题
===========================================

``` bash
../configure --host=arm-hisiv200-linux-gnueabi \
            CC=arm-hisiv200-linux-gcc \
            CFLAGS="-g -O2 -U_FORTIFY_SOURCE" \
            --enable-add-ons  \
            --prefix=/root/temp/lijiancheng/henan/glibc-2.11.1/build_hi3716m/installdir/ \
            libc_cv_forced_unwind=yes \
            libc_cv_c_cleanup=yes \
            --enable-kernel=2.6.0

../configure --host=arm-linux-gnueabihf --build=x86_64-linux-gnu CC=arm-linux-gnueabihf-gcc --prefix=$(pwd)/build_install

../configure --host=arm-linux-gnueabihf \
            --build=
            CC=arm-linux-gnueabihf-gcc \
            --disable-profile \
            --enable-obsolete-rpc \
            --disable-werror \
            --enable-add-ons \
            CFLAGS="-g -O2 -fPIC -U_FORTIFY_SOURCE -funwind-tables" \
            LDFLAGS="-Wl,--dynamic-linker=/lib/ld-linux.so.3" \
            libc_cv_forced_unwind=yes \
            libc_cv_c_cleanup=yes \
            libc_cv_ssp=yes \
            --enable-kernel=3.0.0 \
            --prefix=$(pwd)/build_install

```
extern int __sigsetjmp (struct __jmp_buf_tag __env[1], int __savemask) __THROWNL;
extern int __sigsetjmp (struct __jmp_buf_tag __env[1], int __savemask) __THROWNL;


https://ftp.gnu.org/gnu/glibc/
http://mirrors.ustc.edu.cn/gnu/libc/
glibc-1.09-crypt.tar.gz                  
glibc-1.09.1.tar.gz                      
glibc-2.0.1.bin.alpha-linux.tar.gz       
glibc-2.0.1.bin.m68k-linux.tar.gz        
glibc-2.0.1.tar.gz                       
glibc-2.0.6.tar.gz                       
glibc-2.1-2.1.1.diff.gz                  
glibc-2.1.1-2.1.2.diff.gz                
glibc-2.1.1.tar.gz                       
glibc-2.1.2-2.1.3.diff.gz                
glibc-2.1.2.tar.gz                       
glibc-2.1.3.tar.gz                       
glibc-2.10.1.tar.gz                      
glibc-2.11.1.tar.gz                      
glibc-2.11.2.tar.gz                      
glibc-2.11.3.tar.gz                      
glibc-2.11.tar.gz                        
glibc-2.12.1.tar.gz                      
glibc-2.12.2.tar.gz                      
glibc-2.13.tar.gz                        
glibc-2.14.1.tar.gz                      
glibc-2.14.tar.gz                        
glibc-2.15.tar.gz                        
glibc-2.16.0.tar.gz                      
glibc-2.17.tar.gz                        
glibc-2.18.tar.gz                        
glibc-2.19.tar.gz                        
glibc-2.2-2.2.1.diff.gz                  
glibc-2.2-2.2.1.xdelta                   
glibc-2.2.1-2.2.2.diff.gz                
glibc-2.2.1.tar.gz                       
glibc-2.2.2-2.2.3.diff.gz                
glibc-2.2.2.tar.gz                       
glibc-2.2.3-2.2.4.diff.gz                
glibc-2.2.3-nss.diff                     
glibc-2.2.3.tar.gz                       
glibc-2.2.4-2.2.5.diff.gz                
glibc-2.2.4.tar.gz                       
glibc-2.2.5-2.3.diff.bz2                 
glibc-2.2.5-2.3.diff.gz                  
glibc-2.2.5.tar.gz                       
glibc-2.2.tar.gz                         
glibc-2.20.tar.gz                        
glibc-2.21.tar.gz                        
glibc-2.22.tar.gz                        
glibc-2.23.tar.gz                        
glibc-2.24.tar.gz                        
glibc-2.25.tar.gz                        
glibc-2.26.tar.gz                        
glibc-2.27.tar.gz                        
glibc-2.28.tar.gz                        
glibc-2.29.tar.gz                        
glibc-2.3-2.3.1.diff.gz                  
glibc-2.3.1-2.3.2.diff.bz2               
glibc-2.3.1-2.3.2.diff.gz                
glibc-2.3.1.tar.gz                       
glibc-2.3.2-2.3.3.diff.bz2               
glibc-2.3.2-2.3.3.diff.bz2.sig           
glibc-2.3.2-2.3.3.diff.gz                
glibc-2.3.2-2.3.3.diff.gz.sig            
glibc-2.3.2.tar.gz                       
glibc-2.3.3-2.3.4.diff.bz2               
glibc-2.3.3-2.3.4.diff.bz2.sig           
glibc-2.3.3-2.3.4.diff.gz                
glibc-2.3.3-2.3.4.diff.gz.sig            
glibc-2.3.3.tar.gz                       
glibc-2.3.4-2.3.5.diff.gz                
glibc-2.3.4-2.3.5.diff.gz.sig            
glibc-2.3.4.tar.gz                       
glibc-2.3.5.tar.gz                       
glibc-2.3.6.tar.gz                       
glibc-2.3.tar.gz                         
glibc-2.30.tar.gz                        
glibc-2.31.tar.gz                        
glibc-2.32.tar.gz                        
glibc-2.33.tar.gz                        
glibc-2.34.tar.gz                        
glibc-2.35.tar.gz                        
glibc-2.36.tar.gz                        
glibc-2.37.tar.gz                        
glibc-2.4-2.5.diff.gz                    
glibc-2.4-2.5.diff.gz.sig                
glibc-2.4.tar.gz                         
glibc-2.5-2.5.1.diff.bz2                 
glibc-2.5-2.5.1.diff.bz2.sig             
glibc-2.5-2.5.1.diff.gz                  
glibc-2.5-2.5.1.diff.gz.sig              
glibc-2.5.1.tar.gz                       
glibc-2.5.tar.gz                         
glibc-2.6-2.6.1.diff.bz2                 
glibc-2.6-2.6.1.diff.bz2.sig             
glibc-2.6-2.6.1.diff.gz                  
glibc-2.6-2.6.1.diff.gz.sig              
glibc-2.6.1.tar.gz                       
glibc-2.6.tar.gz                         
glibc-2.7.tar.gz                         
glibc-2.8.tar.gz                         
glibc-2.9.tar.gz                         
glibc-crypt-2.1.tar.gz                   
glibc-crypt.README                       
glibc-libidn-2.10.1.tar.gz               
glibc-libidn-2.3.4.tar.gz                
glibc-libidn-2.3.5.tar.gz                
glibc-libidn-2.3.6.tar.gz                
glibc-libidn-2.4.tar.gz                  
glibc-libidn-2.5.1.tar.gz                
glibc-libidn-2.5.tar.gz                  
glibc-libidn-2.6.1.tar.gz                
glibc-libidn-2.6.tar.gz                  
glibc-libidn-2.7.tar.gz                  
glibc-libidn-2.8.tar.gz                  
glibc-libidn-2.9.tar.gz                  
glibc-linuxthreads-2.0.1.tar.gz          
glibc-linuxthreads-2.0.6.tar.gz          
glibc-linuxthreads-2.1.1.tar.gz          
glibc-linuxthreads-2.1.2.tar.gz          
glibc-linuxthreads-2.1.3.tar.gz          
glibc-linuxthreads-2.2.1.tar.gz          
glibc-linuxthreads-2.2.2.tar.gz          
glibc-linuxthreads-2.2.3.tar.gz          
glibc-linuxthreads-2.2.4.tar.gz          
glibc-linuxthreads-2.2.5.tar.gz          
glibc-linuxthreads-2.2.tar.gz            
glibc-linuxthreads-2.3.1.tar.gz          
glibc-linuxthreads-2.3.2.tar.gz          
glibc-linuxthreads-2.3.3.tar.gz          
glibc-linuxthreads-2.3.4-2.3.5.diff.gz   
glibc-linuxthreads-2.3.4-2.3.5.diff.gz.sig
glibc-linuxthreads-2.3.4.tar.gz          
glibc-linuxthreads-2.3.5.tar.gz          
glibc-linuxthreads-2.3.6.tar.gz          
glibc-linuxthreads-2.3.tar.gz            
glibc-localedata-2.0.1.tar.gz            
glibc-localedata-2.0.6.tar.gz            
glibc-ports-2.10.1.tar.gz                
glibc-ports-2.11.tar.gz                  
glibc-ports-2.12.1.tar.gz                
glibc-ports-2.13.tar.gz                  
glibc-ports-2.14.1.tar.gz                
glibc-ports-2.14.tar.gz                  
glibc-ports-2.15.tar.gz                  
glibc-ports-2.16.0.tar.gz                
glibc-ports-2.3.5.tar.gz                 
glibc-ports-2.3.6.tar.gz                 
glibc-ports-2.4.tar.gz                   
glibc-ports-2.6-2.6.1.diff.gz            
glibc-ports-2.6-2.6.1.diff.gz.sig        
glibc-ports-2.6.1.tar.gz                 
glibc-ports-2.6.tar.gz                   
glibc-ports-2.7.tar.gz                   
glibc-ports-2.8.tar.gz                   
glibc-ports-2.9.tar.gz                   
