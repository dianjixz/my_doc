#!/bin/bash


if ! expect -v &>/dev/null; then
    if [ "$UID" -ne "0" ]; then
        sudo apt install expect -y
    else
        apt install expect -y
    fi
fi


# ssh -v &>/dev/null
# if [ "$?" == "127" ] ; then
#     if [ "$UID" -ne "0" ]; then
#         sudo apt install ssh -y
#     else
#         apt install ssh -y
#     fi
# fi


expect <<EOF

spawn scp -r maixhub_detector_result_2023_03_24__03_08.zip nihao@192.168.2.21:/home/nihao/work/share_dir
expect {
    "(yes/no)" {send "yes\r"; exp_continue}
    "password:" {send "***\r"}
}

expect
exit

EOF
echo ""