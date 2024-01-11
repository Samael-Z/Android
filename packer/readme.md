#### 源程序-hellobutton
#### 加壳程序-packer1
#### 加壳逻辑
核心是通过替换contextImpl——>mPackageInfo(LoadedApk)——>mClassLoader(ClassLoader)成员来完成加壳
其次是加壳的时机问题，由于安卓程序执行的一个类是MainActivity，我们需要在执行该类之前完成替换ClassLoader的工作。
在执行MainActivity之前执行的是Application类，我们新建一个app类继承于Application并且实现Application的初始化函数，在初始化函数中做替换ClassLoader的工作
关于资源的处理：用源文件的res资源文件和mainifest文件提后加壳程序的资源文件即可。
关于反编译，实测是jeb和GDA反编译出来的java代码全是壳的代码，而用jadx反编译出来的代码有源程序的代码。**
