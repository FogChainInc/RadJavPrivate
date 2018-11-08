package com.fogchain.radjavvm;

import android.app.Activity;
import android.app.Application;
import android.view.ViewGroup;

public final class RadJavApplication extends Application
{
    static {
        System.loadLibrary("RadJavWrapper");
    }

    static private boolean initialized = false;
    static private boolean thread_started = false;
    static private boolean paused = false;
    static private boolean shutdown = false;

    static private ViewGroup mainWindow;

    public ViewGroup initialize(Activity activity)
    {
        if (!initialized)
        {
            mainWindow = new RadJavLayout(activity);
            initialized = true;
        }

        return mainWindow;
    }

    public void run(String appFile)
    {
        paused = false;

        if (!thread_started)
        {
            final RadJavApplication app = this;

            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    RadJavRun(app, mainWindow, "app.xrj");
                }
            });

            thread.start();

            thread_started = true;
        }
    }

    public void pause()
    {
        paused = true;
    }

    public void shutdown()
    {
        initialized = false;
        shutdown = true;
    }

    public native int RadJavRun(RadJavApplication application, ViewGroup mainViewGroup, String appFile);
}
