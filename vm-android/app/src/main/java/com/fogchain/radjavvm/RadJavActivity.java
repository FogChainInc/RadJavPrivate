package com.fogchain.radjavvm;

import android.app.Activity;
import android.os.Bundle;
import android.view.ViewGroup;

import java.io.File;


public class RadJavActivity extends Activity
{
    static {
        System.loadLibrary("RadJavWrapper");
    }

    protected ViewGroup mainViewGroup;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //Initialize native code
        RadJavApplication app = (RadJavApplication)getApplication();
        mainViewGroup = app.initialize(this);
    }

    @Override
    protected void onResume() {
        super.onResume();

        //Attach main UI to this activity
        setContentView(mainViewGroup);

        //Continue native code execution
        RadJavApplication app = (RadJavApplication)getApplication();

        final String appFile = app.prependWithExternalCacheDir("examples" + File.separator + "mobile" + File.separator + "init.xrj");

        app.run(appFile);
    }

    @Override
    protected void onPause() {
        super.onPause();

        //Pause native code
        RadJavApplication app = (RadJavApplication)getApplication();
        app.pause();

        //Detach mainViewGroup from this Activity
        ViewGroup parentViewGroup = (ViewGroup)(mainViewGroup.getParent());
        parentViewGroup.removeView(mainViewGroup);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}
