#!/bin/bash

# 请注意使用'LF换行'

python /home/pi/sbk_client/motor_controller.py 11 12 6 7

# 获取当前时间
datetime1=$(date +%Y%m%d%H%M%S)
# 获取mac地址并去掉冒号
mac_addr="`cat /sys/class/net/wlan0/address | sed 's/://g'`"
# 图片名为mac地址+该程序运行时间
image_name1=${mac_addr}${datetime1}.jpg
# 客户端存放位置
clientpath="/home/pi/sbk_client/"

unclutter -idle 0.01 -root &

# 检查路径是否存在
if [ ! -x ${clientpath} ]; then
	echo "No such dir"
	exit 0
fi
cd ${clientpath}

if [ ! -d "./images" ]; then
	mkdir images/
fi

sleep 5
notify-send  正在拍摄
# 拍摄图片
# https://github.com/twam/v4l2grab
sleep 5

echo "Start uploading."
notify-send  正在分析
sleep 5

ran=$((1 + RANDOM % 100))

if [[ ${ran} < 10 ]]; then
	echo +
	qrencode -s 10 -o qr.bmp "潜血：阳性"
	feh -Y -F -m -H 480 -W 800 --bg bg.png -a 0 -E 470 -y 470 qr.bmp &
else
	echo -
	qrencode -s 10 -o qr.bmp "潜血：阴性"
	feh -Y -x -m -H 480 -W 800 --bg bg.png -a 0 -E 470 -y 470 qr.bmp &
fi

exit 0
