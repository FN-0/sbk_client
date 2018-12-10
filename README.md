# 苏贝康14项尿检客户端  

## 主要内容
> #### 功能说明
> #### 使用方法

## 正文
### 功能说明
+ 该客户端程序依次实现了拍照上传显示二维码的过程。
+ 拍照使用了[twam的v4l2grab][1]实现。具体使用参考该作者写的文档。
+ 上传图片使用了[libcurl][2]库并使用[post][3]方法实现。修改请参考官方文档。
+ 二维码生成程序使用了[rsky的qrcode][4]，[使用参考][5]。
+ 以上三个功能流程均使用[*shell script*][6]控制。

[1]:https://github.com/twam/v4l2grab
[2]:https://curl.haxx.se/libcurl/
[3]:https://www.wikiwand.com/en/POST_(HTTP)
[4]:https://github.com/rsky/qrcode
[5]:http://www.voidcn.com/article/p-umkmzkrw-bee.html
[6]:https://www.wikiwand.com/zh-hans/Shell%E8%84%9A%E6%9C%AC

### 使用方法
+ 项目git仓库` git@121.40.169.248:/sbk_src/sbk_client.git `，使用公司现有的服务器作为私有git仓库。参考：[搭建Git服务器][7]。
+ 在`/home`目录下使用` git clone git@121.40.169.248:/sbk_src/sbk_client.git `把项目克隆至树莓派上，当然需要在安装[git][8]后。
    ` git@121.40.169.248's password: funengsbk `
+ 克隆至树莓派后注意`grab_send_showqr.sh`文件中的**第9行**，你可以任意修改该客户端存放的位置，但请注意修改后对其他程序的影响。
+ `grab_send_showqr.sh`文件中的**第28行**为拍照命令，如需要修改参数请看作者文档。
+ 在`/home`目录下使用`git clone https://github.com/twam/v4l2grab`克隆项目。并进行[编译安装][9]。并把`v4l2grab`目录内的可执行文件`v4l2grab`放入`sbk_client`目录中。
+ 分别进入`qrcode_src`目录与`qrcode_failed_src`目录，并分别执行`g++ *.cpp *.c -o qrcode`与`g++ *.cpp *.c -o qrcode_upload_failed`。
+ 把以上两个目录里编译成功的可执行文件复制到上一级目录即`sbk_client`中。
+ `sudo apt-get install feh`安装[feh][10]。
+ 以下是对上传图片实现的说明：
> 1. 使用`tar zxvf curl-7.62.0.tar.gz`命令解压`libcurl`压缩包，当然如果有新版本可以去[官网下载页面][11]下载。
> 2. `cd`进解压生成的目录执行`./configure`与`make && sudo make install`进行编译安装，当然如果有权限问题请使用`sudo`执行。
> 3. `libcurl`安装完成后进行上传程序的编译，例如`gcc upload_img.c -o upload_img -lcurl`，请注意，可执行文件名`upload_img`在`grab_send_showqr.sh`中有对应的使用，如果想更改程序名，则在*shell script*中也应该相应地更改。`-lcurl`是在编译时需要链接的库。
> 4. `upload_img.c`中使用`argv[1]`作为图片名传入程序，所以在*shell*中使用时不能包含任何多余的前缀或后缀。
+ 二维码有两个生成程序，如果上传成功显示包含请求url的二维码，如果上传失败显示包含`上传失败`文字的二维码。
+ 如以上步骤都无错完成，则可以执行*shell script*来进行完整测试。
+ 如测试无误，则把*shell script*添加至开机自动运行。[几种设置树莓派开机自启的方法][12]，我在使用中发现文章的**第3种方法**在实际应用中表现最好。

[7]:https://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/00137583770360579bc4b458f044ce7afed3df579123eca000
[8]:https://git-scm.com/
[9]:https://github.com/twam/v4l2grab/wiki/Installation
[10]:https://wiki.archlinux.org/index.php/Feh_(%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87)
[11]:https://curl.haxx.se/download.html
[12]:https://www.jianshu.com/p/1a160067d8fd
