package org.samle.mydebug;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.os.Process;
import android.widget.TextView;

import org.samle.mydebug.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'mydebug' library on application startup.
    static {
        System.loadLibrary("mydebug");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        //TextView tv = binding.sampleText;
        //tv.setText(stringFromJNI());

        Button btn1 = findViewById(R.id.button);
        Button btn2 = findViewById(R.id.button2);
        Button btn3 = findViewById(R.id.button3);
        Button btn4 = findViewById(R.id.button4);

        btn1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                //killProcess(Process.myPid());

               int ret =  checkdbus();
               Log.i("zxa","called btn1 onclick  function  ret = " + ret);
            }
        });


        btn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int ret = checkfd();
                Log.i("zxa","called btn2 onclick  function  ret = " + ret);

            }
        });


        btn3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int ret = checkmaps();
                Log.i("zxa","called btn3 onclick  function ret =  " + ret);
            }
        });

        btn4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.i("zxa","called btn4 onclick  function ");

            }
        });
    }

    /**
     * A native method that is implemented by the 'mydebug' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();



    public native int killProcess(int pid);
    public native int checkdbus();
    public native int checkfd();
    public native int checkmaps();
}