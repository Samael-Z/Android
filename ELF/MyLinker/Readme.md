### 工程说明 
android10 环境下的自定义linker，只能加载64位的so文件
Mylinker是参考[ngiokweng大佬](https://github.com/ngiokweng)的自实现linker完成的。 
整体架构一致，细微处略有改动，毕竟不是copy的。 
关于如何实现参考ngiokweng大佬写的[帖子](https://bbs.kanxue.com/thread-282316.htm)
### 工程结构说明 
1. demo1是没有用到的模块，mylinker加载的是demo2的so 
2. pkg文件是打包好的apk和so，把so压到/data/local/tmp目录即可
3. util是封装的日志输出的文件夹
4. 有些头文件是没用的，如果把所有需要的类，结构体，#define都放到soinfo.h中会显得臃肿，本意是创建一个comnom.h文件包含所有被引用的东西，但是从bionic中扣来扣去文件就变得多了起来，又把类，结构体，变量重构到一起了。头文件引用这块不是很优雅。
### 踩坑点 
1. LoadSegments() 在调用mmap64时出现的权限异常的错误，通过关闭selinux可以解决 root用户下 setenforce 0 
2. get_soinfo()遍历maps文件一直找不到libmylinker.so,尝试过换到android7下可以在maps文件中找到
android 8 x86_64的环境中也找不到，开始还以为是环境问题，都打算自己从源码里扣new soinfo结构代码自己new算了，想想还是算了，在google中找到[原因](https://github.com/darvincisec/DetectFrida/issues/44)了，原来是要加这个属性【android:extractNativeLibs="true"】也许是ide版本太低了，也许是android 10 系统的原因。
3. link_image()用到的plain_reloc_iterator类原本我是放到linker_sleb_128.h文件中的。编译的时候报未匹配初始化列表的错误，很奇怪，没找到为啥，把这个类挪到Myloader.h文件就不报了，可能是交叉引用的问题。
### 展望
1. 现在是通过替换加载apk的so文件的soinfo结构体实现加载的，后面可以尝试对soinfo链表的断链
2. 屏蔽掉maps文件中待加载so的信息
3. 可以加载32位的so
4. 执行完后构造函数后让so文件执行jni_onload
