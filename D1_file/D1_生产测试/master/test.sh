#!/bin/bash

ROOT_PATH=`pwd`

insmod ${ROOT_PATH}/usbserial.ko
insmod ${ROOT_PATH}/ch341.ko



key_init()
{
    echo "107" > /sys/class/gpio/export
    sleep 0.1
    echo "in" > /sys/class/gpio/gpio107/direction
}


key_selet()
{
    reval=`cat /sys/class/gpio/gpio107/value`;
    return $reval
}

start_D1_test()
{

}


while [ 1 ];do

if [ key_selet == "1" ];
then

    start_D1_test;

fi












done










# ./expect_picocom.sh

# if [ "$?" != "0" ]








