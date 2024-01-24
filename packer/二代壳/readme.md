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