package org.samael.demo2;

public class NativeLib {

    // Used to load the 'demo2' library on application startup.
    static {
        System.loadLibrary("demo2");
    }

    /**
     * A native method that is implemented by the 'demo2' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}