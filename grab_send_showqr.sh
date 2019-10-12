#!/bin/bash

# 请注意使用'LF换行'

#python /home/pi/sbk_client/motor_controller.py 11 12 6 7

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
im_path="/home/pi/sbk_client/example.jpg"
unclutter -idle 0.01 -root &
cd ${clientpath}
cp example.jpg ./images/${image_name1}
notify-send  正在拍摄
sleep 3

#python /home/pi/sbk_client/motor_controller.py 15 16 3 3 &
pos_data="[[953,192,14,10],[953,248,14,10],[953,302,14,10],[953,359,14,10],[953,417,14,10],[953,472,14,10],[953,526,14,10],[953,580,14,10],[953,637,14,10],[953,692,14,10],[953,745,14,10],[953,799,14,10],[953,853,14,10],[953,906,14,10]]"
notify-send  正在上传
res=`curl --max-time 180 -F "picture=@/home/pi/sbk_client/images/${image_name1}" -F "rgb=${pos_data}"  http://121.40.169.248:9080/picture/python/pythonUploadAndAnalysis`
echo ${res}
if [[ "${res}" == "" ]]; then
	#cd ..
	echo "upload failed"
	qrencode -s 6 -o qr.bmp "上传失败"
	# feh 显示二维码
	feh -Y -F -m -H 480 -W 800 --bg bg.png -a 0 -E 470 -y 470 qr.bmp &
	# 5分钟后自动关机
	notify-send -t 0 网络信号不佳
elif [[ "${res}" != "" ]]; then
	echo "qrcode"
	#cd ..
	qrencode -s 4 -o qr.bmp "http://userclient.fun-med.cn/health?heal=${res}"
	feh -Y -x -m -H 480 -W 800 --bg bg.png -a 0 -E 470 -y 470 qr.bmp &
fi
exit 0
