opensuse关闭端口

sudo iptables -A INPUT -p tcp --dport 22 -j DROP

sudo iptables -A OUTPUT -p tcp --dport 22 -j DROP



opensuse打开端口

sudo iptables -A INPUT -ptcp --dport 端口号 -j ACCEPT

sudo iptables -A OUTPUT -ptcp --dport 端口号 -j ACCEPT

