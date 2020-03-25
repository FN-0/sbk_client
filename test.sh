#!/bin/bash

# 请注意使用'LF换行'

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

python /home/pi/sbk_client/algorithm_blo_detection_new.py /home/pi/sbk_client/test/test.jpg

filename="/home/pi/sbk_client/block_pos.txt"
pos_data=`head -n 1 ${filename}`
res=`curl --max-time 180 -F "picture=@/home/pi/sbk_client/test/test.jpg" -F "coordinates=${pos_data}"  http://deviceapi.fun-med.cn/device/v2/upload/fluid/14items`

filename="/home/pi/sbk_client/res.txt"
res=$(cat ${filename})
qrencode -s 4 -o qr.bmp "${res}"
feh -Y -x -m -H 480 -W 800 --bg bg.png -a 0 -E 470 -y 470 qr.bmp &

exit 0