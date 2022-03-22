目录

    服务端rsync
    
    客户端lsyncd
    
        本地 => 本地
    
        本地 => 远程

服务端rsync
~~~
sudo apt install -y rsync

rsync --version
# rsync  version 3.1.1  protocol version 31

mkdir ~/backup/

echo "rsync:123456" > rsync.password

sudo mv rsync.password /etc/

sudo chown root:root /etc/rsync.password && sudo chmod 600 /etc/rsync.password

sudo vim /etc/rsyncd.conf
uid = ubuntu
gid = ubuntu
port = 873
use chroot = no
max connections = 200
timeout = 200
pid file = /var/run/rsyncd.pid
lock file = /var/run/rsync.lock
log file = /var/log/rsyncd.log
ignore errors
read only = false
list = false
hosts allow = *
auth users = rsync
secrets file = /etc/rsync.password
[backup]
comment = "backup"
path = /home/ubuntu/backup/
sudo systemctl start rsync

sudo systemctl enable rsync

sudo systemctl status rsync
~~~
客户端lsyncd
~~~
sudo apt install -y lsyncd

lsyncd --version
# Version: 2.1.5

sudo mkdir /etc/lsyncd/

sudo mkdir /var/log/lsyncd/
~~~
本地 => 本地
~~~
sudo vim /etc/lsyncd/lsyncd.conf.lua
settings {
    logfile = "/var/log/lsyncd/lsyncd.log",
    statusFile = "/var/log/lsyncd/lsyncd.status"
}

sync {
    default.rsync,
    source = "/usr/local/bin/",
    target = "/mnt"
}
sudo systemctl start lsyncd

sudo systemctl enable lsyncd

sudo systemctl status lsyncd
~~~
本地 => 远程
~~~
echo "123456" > rsync.password

sudo mv rsync.password /etc/

sudo chown root:root /etc/rsync.password && sudo chmod 600 /etc/rsync.password

sudo vim /etc/lsyncd/lsyncd.conf.lua
settings {
    logfile = "/var/log/lsyncd/lsyncd.log",
    statusFile = "/var/log/lsyncd/lsyncd.status"
}

sync {
    default.rsync,
    source = "/usr/local/bin/",
    target = "rsync@129.211.157.191::backup",
    delete = true,
    exclude = { "*.swp" },
    delay = 0,
    rsync = {
        binary = "/usr/bin/rsync",
        archive = true,
        compress = true,
        verbose = true,
        password_file = "/etc/rsync.password",
        _extra = {"--bwlimit=1024"}
    }
}
sudo systemctl restart lsyncd

sudo systemctl status lsyncd
~~~
参考

    axkibe / lsyncd
    
    实时同步lsyncd
    
    How to Synchronize Directories Using Lsyncd on Ubuntu 20.04
    
    10+ lsyncd examples to sync directories real time in CentOS/RHEL 7

作者：诺之林
链接：https://www.jianshu.com/p/c6d4c59c59da
来源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。