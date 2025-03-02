package com.example.signcheck01;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Application;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;

import java.lang.reflect.Field;
import java.util.Base64;
import java.security.MessageDigest;


public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        Log.i("zxa", "com.example.signcheck01 is called-反抗-检查 IPackageManager 有没有被动态代理  true是包名一致没被代理   【"  +  checkPMProxy()  + "】");
        Log.i("zxa", "com.example.signcheck01 is called-检查签名的有没有被修改   true是签名一致没被修改   【"  +  checkAPI()  + "】");
        Log.i("zxa", "com.example.signcheck01 is called-反抗-检查 Application   true是AppName一致没被修改   【"  +  checkApplication()  + "】");
    }
    private  boolean checkPMProxy()  {
        String truePMName = "android.content.pm.IPackageManager$Stub$Proxy";
        String nowPMName = "";
        try{
            PackageManager pm = getPackageManager();
            Field mPMField = pm.getClass().getDeclaredField("mPM");
            mPMField.setAccessible(true);

            Object mPM = mPMField.get(pm);
            nowPMName = mPM.getClass().getName();
        }catch (Exception e){
            e.printStackTrace();
        }

        return nowPMName.equals(truePMName) ;
    }

    private  boolean checkApplication()  {
        Application nowApp = getApplication();
        String trueAppName = "ProxyApp";
        String nowAppName = nowApp.getClass().getSimpleName();
        return trueAppName.equals(nowAppName);



    }

    private  boolean checkAPI()  {
        String trueSignMD5 = "d0add9987c7c84aeb7198c3ff26ca152";
        String nowSignMD5 = "";
        Signature[] signs = null;
        try{
            //得到签名md5
            if (Build.VERSION.SDK_INT >= 28){
                PackageInfo packageInfo = getPackageManager().getPackageInfo(getPackageName(),PackageManager.GET_SIGNING_CERTIFICATES);

                signs = packageInfo.signingInfo.getApkContentsSigners();
            }else {
                PackageInfo packageInfo = getPackageManager().getPackageInfo(getPackageName(),PackageManager.GET_SIGNATURES);
                signs = packageInfo.signatures;
            }
            String signBase64 =Base64.getEncoder().encodeToString(signs[0].toByteArray());
            nowSignMD5 = myMD5(signBase64);
            Log.i("zxa", "checkAPI:  原来的签名=  " + trueSignMD5 + " 新的签名md5 = " + nowSignMD5);
        }catch (Exception e){
            e.printStackTrace();
        }
        return trueSignMD5.equals(nowSignMD5);
    }

    public  String myMD5(String str){
        try {
            // 创建加密对象
            MessageDigest digest = MessageDigest.getInstance("md5");

            // 调用加密对象的方法，加密的动作已经完成
            byte[] bs = digest.digest(str.getBytes());
            // 接下来，我们要对加密后的结果，进行优化，按照mysql的优化思路走
            // mysql的优化思路：
            // 第一步，将数据全部转换成正数：
            String hexString = "";
            for (byte b : bs) {
                // 第一步，将数据全部转换成正数：
                // 解释：为什么采用b&255
                /*
                 * b:它本来是一个byte类型的数据(1个字节) 255：是一个int类型的数据(4个字节)
                 * byte类型的数据与int类型的数据进行运算，会自动类型提升为int类型 eg: b: 1001 1100(原始数据)
                 * 运算时：
                 *  b:   0000 0000 0000 0000 0000 0000 1001 1100
                 *  255: 0000 0000 0000 0000 0000 0000 1111 1111
                 * 结果：0000 0000 0000 0000 0000 0000 1001 1100
                 * 此时的temp是一个int类型的整数
                 */
                int temp = b & 255;
                // 第二步，将所有的数据转换成16进制的形式
                // 注意：转换的时候注意if正数>=0&&<16，那么如果使用Integer.toHexString()，可能会造成缺少位数
                // 因此，需要对temp进行判断
                if (temp < 16 && temp >= 0) {
                    // 手动补上一个“0”
                    hexString = hexString + "0" + Integer.toHexString(temp);
                } else {
                    hexString = hexString + Integer.toHexString(temp);
                }
            }
            return hexString;
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return "";
    }
}