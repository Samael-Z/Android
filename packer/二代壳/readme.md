packer221.7z
**源程序-hellobutton**  
**加壳程序-packer221**  
**加壳逻辑**  
加壳程序程序再attachBaseContext期间通过反射替换mCookie的方式完成加固的功能  
**加壳步骤**  
1，把dex文件加载到内存  
2，反射调用openDexFile加载dex内存得到源程序的mCookie，接下来就是用源程序的mCookie替换加壳程序的mCookie  
3，依次反射调用BaseDexClassLoader---DexPathList:pathList----Element[]:dexElements----DexFile:dexFile---int:mCookie来等到加壳程序的mCookie  
期间遇到的问题  
//报错，不知道是不是加载时机问题（java.lang.NoSuchMethodError: android.os.Trace.endSection）  
//出错原因：源程序最低编译android4.4与当前虚拟机android4.1不匹配  
注意，通过反射调用openDexFile加载dex的加壳方式再后面的android版本中被修复


packer222_err.7z
**源程序-testempty**  
**加壳程序-packer222**  
**参考链接**  
https://www.cnblogs.com/ltyandy/p/11642108.html  
在此基础上进行小修改，无法正常加载我的dex文件，报错信息：java.lang.NoClassDefFoundError: com.example.testempty.R$layout。不知道为啥报这个错误  
看起来是资源问题，但是资源已经拷贝了过来，不清楚原因暂时存档