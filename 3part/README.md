#####目录结构
* **boost**
* **mysql**
解压mysql-connector-c-6.1.6-src.zip，运行cmake 编译动态库 拷贝到3part\mysql下
拷贝include文件
拷贝mysql ./mysql-connector-c-6.1.6-src/obj/zlib/Debug文件中的zlib库到zlib文件夹
注意编译生成zlib需要将其修改为MDd