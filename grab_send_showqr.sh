#!/bin/bash

# 请注意使用'LF换行'

python /home/pi/sbk_client/motor_controller.py 11 12 6 7

ping -c 1 121.40.169.248 > /dev/null 2>&1
while [ $? -ne 0 ]; do
	ping -c 1 121.40.169.248 > /dev/null 2>&1
done

notify-send 网络连接成功

# 获取网络时间
sudo ntpdate 0.cn.pool.ntp.org

# 获取当前时间
datetime1=$(date +%Y%m%d%H%M%S)
# 获取mac地址并去掉冒号
mac_addr="`cat /sys/class/net/wlan0/address | sed 's/://g'`"
# 图片名为mac地址+该程序运行时间
image_name1=${mac_addr}${datetime1}.jpg
# 客户端存放位置
clientpath="/home/pi/sbk_client/"

unclutter -idle 0.01 -root &

# 检查摄像头是否存在
if [ ! -c "/dev/video0" ]; then
	echo "no cam"
	notify-send -t 0 设备连接断开
	python /home/pi/sbk_client/motor_controller.py 15 16 3 3
	exit 0
fi

# 检查路径是否存在
if [ ! -x ${clientpath} ]; then
	echo "No such dir"
	python /home/pi/sbk_client/motor_controller.py 15 16 3 3
	exit 0
fi
cd ${clientpath}

if [ ! -d "./images" ]; then
	mkdir images/
fi

notify-send  正在拍摄
# 拍摄图片
# https://github.com/twam/v4l2grab
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name1}
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name1}

if [ ! -f ${image_name1} ]; then
	echo "Image does not exist."
	notify-send -t 0 图片未拍摄成功
	python /home/pi/sbk_client/motor_controller.py 15 16 3 3
	exit 0
fi

mv ${image_name1} images/
cd images/

python /home/pi/sbk_client/motor_controller.py 15 16 3 3 &

python /home/pi/sbk_client/algorithm_blo_detection_new.py ${image_name1}

filename="/home/pi/sbk_client/res.txt"

pos_data=`head -n 1 ${filename}`
res=`curl --max-time 180 -F "picture=@/home/pi/sbk_client/images/${image_name1}" -F "coordinates=${pos_data}"  http://deviceapi.fun-med.cn/device/v2/upload/fluid/14items`

filename="/home/pi/sbk_client/block_pos.txt"
res=$(cat ${filename})
qrencode -s 4 -o qr.bmp "${res}"
feh -Y -x -m -H 480 -W 800 --bg bg.png -a 0 -E 470 -y 470 qr.bmp &

exit 0
