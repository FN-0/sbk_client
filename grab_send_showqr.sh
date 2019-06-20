#!/bin/bash

# 请注意使用'LF换行'
# 隐藏指针
unclutter -idle 0.01 -root &

while :
do
	repeat=0
	datetime1="201906240"
	str=`head /dev/urandom | cksum`
	datetime1=datetime1+${str:0:5}
	#until [ ${#datetime1} -eq 14 ]
	#do	
	#done
	datetime2=`expr ${datetime1} + 1`
	datetime3=`expr ${datetime1} + 2`
	datetime4=`expr ${datetime1} + 3`
	datetime5=`expr ${datetime1} + 4`
	# 获取mac地址并去掉冒号
	mac_addr="`cat /sys/class/net/wlan0/address | sed 's/://g'`"
	# 图片名为mac地址+该程序运行时间
	image_name1=${mac_addr}${datetime1}.jpg
	image_name2=${mac_addr}${datetime2}.jpg
	image_name3=${mac_addr}${datetime3}.jpg
	image_name4=${mac_addr}${datetime4}.jpg
	image_name5=${mac_addr}${datetime5}.jpg
	# 检测重复
	if [ -f images/${image_name1} ]; then
		repeat=1
	elif [ -f images/${image_name2} ]; then
		repeat=1
	elif [ -f images/${image_name3} ]; then
		repeat=1
	elif [ -f images/${image_name4} ]; then
		repeat=1
	elif [ -f images/${image_name5} ]; then
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
	#feh -Y -F -m -H 480 -W 800 --bg bg.png -a 0 -E 470 -y 470 nowebcam.png &
	while [ ! -c "/dev/video0" ]; do
		sleep 2
		notify-send -t 1000 摄像头未插入
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
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name1}
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name1}
sleep 1
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name2}
sleep 1
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name3}
sleep 1
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name4}
sleep 1
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name5}

if [ ! -f ${image_name1} ]; then
	echo "Image does not exist."
	notify-send 图片未拍摄成功
	exit 0
fi

mv ${image_name1} images/
mv ${image_name2} images/
mv ${image_name3} images/
mv ${image_name4} images/
mv ${image_name5} images/
#notify-send 保存成功

# shutdown in few minutes
notify-send -t 0 ${datetime1}
notify-send -t 0 ${datetime2}
notify-send -t 0 ${datetime3}
notify-send -t 0 ${datetime4}
notify-send -t 0 ${datetime5}
#三分钟后将自动关机
#sleep 180
#shutdown now

exit 0
