package org.samael.mylinker;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import org.samael.mylinker.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'mylinker' library on application startup.
    static {
        System.loadLibrary("mylinker");
        //System.loadLibrary("demo2");
        //先加载load2是为了测试程序是否能够正确运行，
        //test()实现手工linker demo2，后执行demo2的fun1函数
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());

        Log.d("zxa", "MainActivity onCreate called");
        int pid = android.os.Process.myPid();
        Log.d("zxa", "pid = " + pid );
        test();
        demo2fun1();
        //test2();
    }

    /**
     * A native method that is implemented by the 'mylinker' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native String demo2fun1();

    public native void test();
    public native void test2();
}