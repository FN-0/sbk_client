#!/bin/bash

# 请注意使用'LF换行'

sleep 5
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

# 检查路径是否存在
if [ ! -x ${clientpath} ]; then
	echo "No such dir"
	exit 0
fi
cd ${clientpath}

if [ ! -d "./images" ]; then
	mkdir images/
fi

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
	exit 0
fi
mv ${image_name1} images/
mv ${image_name2} images/
mv ${image_name3} images/
mv ${image_name4} images/
mv ${image_name5} images/
cd images/

echo "Start uploading."
res=`curl -F "picture=@/home/pi/sbk_client/images/${image_name1}" -F "picture1=@/home/pi/sbk_client/images/${image_name2}" -F "picture2=@/home/pi/sbk_client/images/${image_name3}" -F "picture3=@/home/pi/sbk_client/images/${image_name4}" -F "picture4=@/home/pi/sbk_client/images/${image_name5}" http://www.sup-heal.com:9080/picture/FiveimageUpload`
echo ${res}
#subl=${res#*:}
#subr=${subl%%,*}
ret=$?
# 使用程序返回值作为上传成功或失败的依据
if [ ${ret} -eq 2 ]; then
	cd ..
	echo "upload failed"
	qrencode -o qr.bmp "上传失败"
	# feh 显示二维码
	feh -Y -F -m -H 480 -W 800 --bg bg.png -a 0 -E 470 -y 470 qr.bmp &
	# 5分钟后自动关机
	sleep 300
	shutdown now
elif [ ${ret} -eq 0 ]; then
	echo "qrcode"
	cd ..
	# display qr code
	qrencode -s 6 -o qr.bmp "http://www.sup-heal.com/#/health/healthUpload?deviceNo=${mac_addr}&midDate=${mac_addr}${datetime1}${datetime2}${datetime3}${datetime4}${datetime5}"
	feh -Y -F -m -H 480 -W 800 --bg bg.png -a 0 -E 470 -y 470 qr.bmp &
	sleep 300
	shutdown now
fi

exit 0
