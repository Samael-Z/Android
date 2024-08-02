package com.example.antidebug02;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Debug;
import android.util.Log;
import android.widget.TextView;

import com.example.antidebug02.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'antidebug02' library on application startup.
    static {
        System.loadLibrary("antidebug02");
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
        checkDebugPort();
    }

    /**
     * A native method that is implemented by the 'antidebug02' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public  static  void checkDebugPort(){
        if ( Debug.isDebuggerConnected()){
            Log.i("zxa", "当前进程正在被调试");
        }else {
            Log.i("zxa", "当前进程没有被调试");
        }



    }


}