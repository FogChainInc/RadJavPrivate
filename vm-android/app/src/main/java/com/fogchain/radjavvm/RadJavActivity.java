/*
 MIT-LICENSE
 Copyright (c) 2018 Higher Edge Software, LLC

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial
 portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
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

        final String appFile = app.prependWithExternalCacheDir("examples" + File.separator + "mobile" + File.separator + "window.xrj");

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
