#!/bin/bash

# 请注意使用'LF换行'

python /home/pi/sbk_client/motor_controller.py 11 12 11 13

ping -c 1 121.40.169.248 > /dev/null 2>&1
while [ $? -ne 0 ]; do
	ping -c 1 121.40.169.248 > /dev/null 2>&1
done

notify-send 网络连接成功

# 获取网络时间
sudo ntpdate 0.cn.pool.ntp.org

# 获取当前时间
datetime1=$(date +%Y%m%d%H%M%S)
sleep 1
datetime2=$(date +%Y%m%d%H%M%S)
sleep 1
datetime3=$(date +%Y%m%d%H%M%S)
sleep 1
datetime4=$(date +%Y%m%d%H%M%S)
sleep 1
datetime5=$(date +%Y%m%d%H%M%S)
# 获取mac地址并去掉冒号
mac_addr="`cat /sys/class/net/wlan0/address | sed 's/://g'`"
# 图片名为mac地址+该程序运行时间
image_name1=${mac_addr}${datetime1}.jpg
image_name2=${mac_addr}${datetime2}.jpg
image_name3=${mac_addr}${datetime3}.jpg
image_name4=${mac_addr}${datetime4}.jpg
image_name5=${mac_addr}${datetime5}.jpg
# 客户端存放位置
clientpath="/home/pi/sbk_client/"

unclutter -idle 0.01 -root &

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

# 上传图片
# 如果上传失败显示含有上传失败文字的二维码
# 如果上传成功显示含有url的二维码，并且重试之前上传失败的图片
if [ ! -f ${image_name1} ]; then
	echo "Image does not exist."
	notify-send -t 0 图片未拍摄成功
	exit 0
elif [ ! -f ${image_name2} ]; then
	notify-send -t 0 图片未拍摄成功
	exit 0
elif [ ! -f ${image_name3} ]; then
	notify-send -t 0 图片未拍摄成功
	exit 0
elif [ ! -f ${image_name4} ]; then
	notify-send -t 0 图片未拍摄成功
	exit 0
elif [ ! -f ${image_name5} ]; then
	notify-send -t 0 图片未拍摄成功
	exit 0
fi

python edit_image.py ${image_name1} ${image_name2} ${image_name3} ${image_name4} ${image_name5}

mv ${image_name1} images/
mv ${image_name2} images/
mv ${image_name3} images/
mv ${image_name4} images/
mv ${image_name5} images/
cd images/

python /home/pi/sbk_client/motor_controller.py 15 16 1 1 &

echo "Start uploading."
notify-send  正在上传
res=`curl --max-time 180 -F "picture=@/home/pi/sbk_client/images/${image_name1}" -F "picture1=@/home/pi/sbk_client/images/${image_name2}" -F "picture2=@/home/pi/sbk_client/images/${image_name3}" -F "picture3=@/home/pi/sbk_client/images/${image_name4}" -F "picture4=@/home/pi/sbk_client/images/${image_name5}" http://deviceapi.fun-med.cn/device/v1/upload/urine/14items`
echo ${res}
#echo ${res:0:1}
#subl=${res#*:}
#subr=${subl%%,*}
#ret=$?
# 使用程序返回值作为上传成功或失败的依据
if [[ "${res}" == "" ]]; then
	cd ..
	echo "upload failed"
	qrencode -s 6 -o qr.bmp "上传失败"
	# feh 显示二维码
	feh -Y -F -m -H 480 -W 800 --bg bg.png -a 0 -E 470 -y 470 qr.bmp &
	# 5分钟后自动关机
	notify-send -t 0 上传失败，五分钟后将自动关机
	sleep 300
	shutdown now
elif [[ "${res}" != "" ]]; then
	echo "qrcode"
	cd ..
	# display qr code
	#qrencode -s 6 -o qr.bmp "http://www.sup-heal.com/#/health/healthUpload?deviceNo=${mac_addr}&midDate=${mac_addr}${datetime1}${datetime2}${datetime3}${datetime4}${datetime5}"
	qrencode -s 4 -o qr.bmp "http://userclient.fun-med.cn/health?heal=${res}"
	feh -Y -x -m -H 480 -W 800 --bg bg.png -a 0 -E 470 -y 470 qr.bmp &
	# re-upload
	# 重传部分，循环上传并且删除（如果成功）
	#reupload_images=`ls upload_failed/`
	#if [ ! `ls upload_failed/ $*|wc -w` -eq 0 ]; then 
	#	for reup_img in ${reupload_images}; do
	#		# re-upload them!
	#		./upload_img ${reup_img}
	#		reupret=$?
	#		if [ ${reupret} -eq 0 ]; then
	#			rm ${reup_img}
	#		fi
  	#	done
	#fi
	# shutdown in few minutes
	notify-send -t 0 上传成功，五分钟后将自动关机
	sleep 300
	shutdown now
fi

exit 0
