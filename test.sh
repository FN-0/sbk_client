#!/bin/bash

# 请注意使用'LF换行'

python /home/pi/sbk_client/motor_controller.py 11 12 3.5 4.5

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

# 检查路径是否存在
if [ ! -x ${clientpath} ]; then
	echo "No such dir"
	#python /home/pi/sbk_client/motor_controller.py 15 16 3 3
	exit 0
fi
cd ${clientpath}

if [ ! -d "./images" ]; then
	mkdir images/
fi

#python /home/pi/sbk_client/motor_controller.py 15 16 3 3 &

python /home/pi/sbk_client/block_detector.py /home/pi/sbk_client/test/b827ebb0316220200428172444.jpg

filename="/home/pi/sbk_client/block_pos.txt"
pos_data=`head -n 1 ${filename}`

if [[ "${pos_data}" == "0" ]]; then
    notify-send -t 0 试纸位置错误
    exit 0
fi

echo "Start uploading."
notify-send  正在处理
res=`curl --max-time 180 -F "picture=@/home/pi/sbk_client/test/b827ebb0316220200428172444.jpg" -F "coordinates=${pos_data}"  http://deviceapi.fun-med.cn/device/v2/upload/fluid/14items`
echo ${res}
# 使用程序返回值作为上传成功或失败的依据
if [[ "${res}" == "" ]]; then
	cd ..
	echo "upload failed"
	qrencode -s 6 -o /home/pi/sbk_client/qr.bmp "上传失败"
	# feh 显示二维码
	feh -Y -F -m -H 480 -W 800 --bg /home/pi/sbk_client/bg.png -a 0 -E 470 -y 470 /home/pi/sbk_client/qr.bmp &
elif [[ "${res}" != "" ]]; then
	echo "qrcode"
	cd ..
	qrencode -s 4 -o /home/pi/sbk_client/qr.bmp "${res}"
	feh -Y -x -m -H 480 -W 800 --bg /home/pi/sbk_client/bg.png -a 0 -E 470 -y 470 /home/pi/sbk_client/qr.bmp &
fi

exit 0
