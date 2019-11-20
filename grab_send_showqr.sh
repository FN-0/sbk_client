#!/bin/bash

# 请注意使用'LF换行'
# 隐藏指针
unclutter -idle 0.01 -root &

while :
do
	repeat=0
	str=`head /dev/urandom | cksum`
	random_name=${str:0:5}
	# 获取mac地址并去掉冒号
	mac_addr="`cat /sys/class/net/wlan0/address | sed 's/://g'`"
	# 图片名为mac地址+该程序运行时间
	image_name=${random_name}.jpg
	# 检测重复
	if [ -f images/${image_name} ]; then
		repeat=1
	fi
	# 无重复跳出循环
	if [ ${repeat} -eq 0 ]; then
		break
	fi
done

# 客户端存放位置
clientpath="/home/pi/sbk_client/"

if [ ! -c "/dev/video0" ]; then
	echo "no cam"
	timeout=0
	while [ ! -c "/dev/video0" ]; do
		sleep 2
		notify-send -t 1000 设备未插入
		let timeout+=2
		if [ 300 -eq ${timeout} ]; then
			shutdown now
		fi
	done
fi

# 检查路径是否存在
if [ ! -x ${clientpath} ]; then
	echo "No such dir"
	notify-send 发生未知错误
	exit 0
fi
cd ${clientpath}

if [ ! -d "./images" ]; then
	mkdir images/
fi

notify-send  正在拍摄
# 拍摄图片
# https://github.com/twam/v4l2grab
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name}
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name}

if [ ! -f ${image_name} ]; then
	echo "Image does not exist."
	notify-send 图片未拍摄成功
	exit 0
fi

mv ${image_name} images/
notify-send -t 0 ${datetime}


exit 0
