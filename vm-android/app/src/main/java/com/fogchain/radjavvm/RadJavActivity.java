package com.fogchain.radjavvm;

import android.app.Activity;
import android.os.Bundle;

public class RadJavActivity extends Activity {

    // Used to load the 'RadJavWrapper' library on application startup.
    static {
        System.loadLibrary("RadJavWrapper");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Example of a call to a native method
        android.util.Log.d("RadJavActivity", stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'RadJavWrapper' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
