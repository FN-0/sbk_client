# 苏贝康14项尿检客户端  

## 主要内容
> #### 功能说明
> #### 使用方法
> #### 接口调用

## 正文
### 功能说明
+ 该客户端程序依次实现了拍照上传显示二维码的过程。
+ 拍照使用了[twam的v4l2grab][1]实现。具体使用参考该作者写的文档。
+ 上传图片使用了`curl`。
+ 二维码生成程序使用了`qrencode`。
+ 以上三个功能流程均使用[*shell script*][6]控制。

[1]:https://github.com/twam/v4l2grab
[6]:https://www.wikiwand.com/zh-hans/Shell%E8%84%9A%E6%9C%AC

### 使用方法
+ 克隆至树莓派后注意`grab_send_showqr.sh`文件中的客户端存放的位置，但请注意修改后对其他程序的影响。
+ `grab_send_showqr.sh`文件中的拍照命令如需要修改参数请看作者文档。
+ 在`/home`目录下使用`git clone https://github.com/twam/v4l2grab`克隆项目。并进行[编译安装][9]。并把`v4l2grab`目录内的可执行文件`v4l2grab`放入`sbk_client`目录中。
+ 安装`qrencode` `libnotify-bin`
+ `sudo apt-get install feh`安装[feh][10]。
+ 以下是对上传图片实现的说明：
> 1. 使用`tar zxvf curl-7.62.0.tar.gz`命令解压`libcurl`压缩包，当然如果有新版本可以去[官网下载页面][11]下载。
> 2. `cd`进解压生成的目录执行`./configure`与`make && sudo make install`进行编译安装，当然如果有权限问题请使用`sudo`执行。
> 3. `LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib` `export LD_LIBRARY_PATH` `ldconfig`
> 4. 执行*shell script*来进行完整测试。
+ 如测试无误，则把*shell script*添加至开机自动运行。[几种设置树莓派开机自启的方法][12]，我在使用中发现文章的**第3种方法**在实际应用中表现最好。

[9]:https://github.com/twam/v4l2grab/wiki/Installation
[10]:https://wiki.archlinux.org/index.php/Feh_(%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87)
[11]:https://curl.haxx.se/download.html
[12]:https://www.jianshu.com/p/1a160067d8fd

### 接口调用
1. 上传图片调用接口
> + 上传图片需要调用服务器端的上传图片接口
2. 二维码返回值接口
> + 二维码为字符内容，手机端需要根据其字符内容来判断是否选择获取结果或告诉用户上传失败
