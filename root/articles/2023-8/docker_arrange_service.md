1. 下载发布目标os image，ubuntu一般是最近稳定版，22.04

## 查看镜像
docker search ubuntu

## 拉取Ubuntu镜像
docker pull ubuntu

## 查看拉取到的镜像
docker images

## 查看更详细的Ubuntu信息
docker inspect ubuntu

## 创建并启动Ubuntu容器
docker run --name ubuntu22.04 -it -p 2060:22 -v /home/jason:/home/jing ubuntu bash 
其中name表示容器名称，it表示打开交互操作，p表示端口映射 ，v表示共享目录，ubuntu镜像名

2. 软件部署

将elf，so文件依赖拷贝到执行目录：
ldd <file> | awk '{print $3}' | xargs -i cp -L {} <target directory>

export LD_LIBRARY_PATH=<<target directory>>运行即可

一般image和编译环境os版本类似，不会出问题，否则容易core dump。
编译的时候最好同时设置rpath, 在$ORIGIN/ 执行目录加载so

[rpath是什么](https://www.cnblogs.com/qiumingcheng/p/14722155.html)
[cmake设置rpath](https://blog.csdn.net/weixin_38693938/article/details/113399924)

除了ldd命令查看依赖， nm命令可以看到so的详细导出符号， 

[ldd命令详解](https://blog.csdn.net/f_carey/article/details/109686310)
