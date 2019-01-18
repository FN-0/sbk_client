#!/bin/bash

# !!!修改时请注意使用'LF换行'!!!

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

#url="http://www.sup-heal.com/#/health/healthUpload?deviceNo=${mac_addr}&midDate=${mac_addr}${datetime1}${datetime2}${datetime3}${datetime4}${datetime5}"
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

if [ ! -d "./upload_failed" ]; then
	mkdir upload_failed/
fi

# 拍摄图片
# https://github.com/twam/v4l2grab
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name1}
sleep 1
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name2}
sleep 1
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name3}
sleep 1
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name4}
sleep 1
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name5}

# upload & show qr code & upload fail process
# 上传图片
# 如果上传失败显示含有上传失败文字的二维码
# 如果上传成功显示含有url的二维码，并且重试之前上传失败的图片
if [ ! -f ${image_name1} ]; then
	echo "Image does not exist."
	exit 0
fi
mv ${image_name1} images/
mv ${image_name2} images/
mv ${image_name3} images/
mv ${image_name4} images/
mv ${image_name5} images/
cd images/
# sleep 10 # 经实际测试，上传时可能还没连上wifi需要加延时
echo "Start uploading."
../upload_img ${image_name1} ${image_name2} ${image_name3} ${image_name4} ${image_name5}
ret=$?
# 使用程序返回值作为上传成功或失败的依据
if [ ${ret} -eq 2 ]; then
	#cp ${image_name} ../upload_failed/
	cd ..
	echo "upload failed"
	# show "upload failed" with qr code
	qrencode -o qr.bmp "upload failed"
	# feh 显示二维码
	#feh -F qr.bmp &
	feh -Y -F -m -H 480 -W 800 --bg bg.png -a 0 -E 470 -y 470 qr.bmp &
	# 5分钟后自动关机
	sleep 300
	shutdown now
elif [ ${ret} -eq 0 ]; then
	echo "qrcode"
	cd ..
	# display qr code
	qrencode -o qr.bmp -s 6 "http://www.sup-heal.com/#/health/healthUpload?deviceNo=${mac_addr}&midDate=${mac_addr}${datetime1}${datetime2}${datetime3}${datetime4}${datetime5}"
	feh -Y -F -m -H 480 -W 800 --bg bg.png -a 0 -E 470 -y 470 qr.bmp &
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
	sleep 300
	shutdown now
fi

exit 0
