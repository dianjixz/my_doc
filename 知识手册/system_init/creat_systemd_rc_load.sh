#!/bin/bash

if [ "$(id -u)" != "0" ]; then
	echo "This script must be run as root. Exiting..."
	exit 1
fi

file="/etc/rc.local"

if [ ! -f "$file" ]; then
	echo "Creating $file..."
	touch "$file"
	chmod +x "$file"
	cat <<EOF >$file
#!/bin/bash


exit 0
EOF
else
	echo "$file already exists."
fi

sed -i '$s/$/\n[Install]\nWantedBy=multi-user.target/' /lib/systemd/system/rc-local.service

systemctl enable rc-local
