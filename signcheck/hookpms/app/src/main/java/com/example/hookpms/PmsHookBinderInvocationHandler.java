package com.example.hookpms;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.util.Log;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;

public class PmsHookBinderInvocationHandler implements InvocationHandler {

    private Object base;
    public final  static String ZXA  = "zxa";

    private String SIGN;
    private String appPkgName = "";

    public PmsHookBinderInvocationHandler(Object base, String sign, String appPkgName, int hashCode) {
        try{
            this.base = base;
            this.SIGN = sign;
            this.appPkgName = appPkgName;

        }catch (Exception e){
            //Log.i(ZXA, "PmsHookBinderInvocationHandler consitictor error  " + Log.getStackTraceString(e));
        }
    }


    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        Log.i(ZXA, method.getName());

        //查看是否是getPackageInfo方法
        if ("getPackageInfo".equals(method.getName())){
            String pkgName = (String) args[0];
            Integer flag = (Integer) args[1];
            //是否是获取我们需要的hook apk的签名
            if (flag == PackageManager.GET_SIGNATURES && appPkgName.equals(pkgName)){
                //将构造方法中传进来的新的签名覆盖掉原来的签名
                Signature sign = new Signature(SIGN);
                PackageInfo packageInfo = (PackageInfo) method.invoke(base,args);
                packageInfo.signatures[0] = sign;
                Log.i(ZXA, "替换签名完成");
                return packageInfo;
            }
        }


        return method.invoke(base,args);
    }
}
