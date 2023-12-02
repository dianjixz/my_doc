#!/bin/sh
input_string=$(cat /proc/ax_proc/uid)
sha224_hash=$(echo -n "$input_string" | sha256sum | cut -d' ' -f1)
oui_bytes1="${sha224_hash:0:12}"
oui_bytes2="${sha224_hash:12:12}"
oui_bytes3="${sha224_hash:24:12}"
oui_bytes4="${sha224_hash:36:12}"

tmp_str="$(echo $oui_bytes1 | sed -r 's/(..)(..)(..)(..)(..)(..)/\1:\2:\3:\4:\5:\6/')"
tmp_str1=$(echo "$tmp_str" | tr 'a-z' 'A-Z')
mac1=$(echo "$tmp_str1" | sed 's/./0/2')

tmp_str="$(echo $oui_bytes2 | sed -r 's/(..)(..)(..)(..)(..)(..)/\1:\2:\3:\4:\5:\6/')"
tmp_str1=$(echo "$tmp_str" | tr 'a-z' 'A-Z')
mac2=$(echo "$tmp_str1" | sed 's/./0/2')

tmp_str="$(echo $oui_bytes3 | sed -r 's/(..)(..)(..)(..)(..)(..)/\1:\2:\3:\4:\5:\6/')"
tmp_str1=$(echo "$tmp_str" | tr 'a-z' 'A-Z')
mac3=$(echo "$tmp_str1" | sed 's/./0/2')

tmp_str="$(echo $oui_bytes4 | sed -r 's/(..)(..)(..)(..)(..)(..)/\1:\2:\3:\4:\5:\6/')"
tmp_str1=$(echo "$tmp_str" | tr 'a-z' 'A-Z')
mac4=$(echo "$tmp_str1" | sed 's/./0/2')



ifconfig usb0 down
ifconfig eth0 down


ifconfig eth0 hw ether $mac1
ifconfig usb0 hw ether $mac2
#ifconfig wlan0 hw ether $mac3
#ifconfig wlan1 hw ether $mac4


ifconfig usb0 up
ifconfig eth0 up
