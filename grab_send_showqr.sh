#!/bin/bash
# 获取当前时间
datetime=$(date +%Y%m%d%H%M%S)
# 获取mac地址
mac_addr="`cat /sys/class/net/wlan0/address`"
image_name=${mac_addr}${datetime}.jpg
clientpath="/home/pi/14src/"

# 检查路径是否存在
if [ ! -x ${clientpath} ]; then
	echo "No such dir"
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
./v4l2grab -d/dev/video0 -W1920 -H1080  -q100 -m -o${image_name}

# upload & show qr code & upload fail process
# 上传图片
# 如果上传失败显示含有上传失败文字的二维码
# 如果上传成功显示含有url的二维码，并且重试之前上传失败的图片
if [ ! -f ${image_name} ]; then
	echo "Image does not exist."
	exit 0
fi
mv ${image_name} images/
echo "Start uploading."
./upload_img ./images/${image_name}
#echo $?
if [ $? -eq 2 ]; then
	cp ./images/${image_name} upload_failed/
	echo "upload failed"
	# show "upload failed" with qr code
	./qrcode_upload_failed
	feh -F qr.bmp &
	shutdown +10 "System will shutdown after 10 minutes"
elif [ $? -eq 0 ]; then
	# display qr code
	./qrcode
	feh -F qr.bmp &
	# re-upload
	# 重传部分，循环上传并且删除（如果成功）
	reupload_images=`ls upload_failed/`
	if [ ! `ls upload_failed/ $*|wc -w` -eq 0 ]; then 
		for reup_img in ${reupload_images}; do
		# re-upload them!
		./upload_img ${reup_img}
		if [ $? -eq 0 ]; then
			rm ${reup_img}
		fi
  	done
	fi
	# shutdown in few minutes
	shutdown +5 "System will shutdown after 5 minutes"
fi

exit 0
