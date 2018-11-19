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
import android.app.Application;
import android.content.res.AssetManager;
import android.view.ViewGroup;
import android.os.Build;

import java.io.File;
import java.io.IOException;


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

    public static String getAbi()
    {
        return Build.SUPPORTED_ABIS[0];
    }

    public String prependWithExternalCacheDir(String fileName)
    {
        String result = getExternalCacheDir().getAbsolutePath();

        result += File.separator + fileName;

        return result;
    }

    private boolean cacheJsEngineFiles()
    {
        AssetManager assetManager = getAssets();

        String abi = getAbi();
        String sourcePath = "v8" + File.separator + abi;

        String filesToCopy[] = {"natives_blob.bin",
                                "snapshot_blob.bin"};

        for (String fileName : filesToCopy)
        {
            String sourceFilePath = sourcePath + File.separator + fileName;

            File outputFile = new File(prependWithExternalCacheDir(fileName));

            try
            {
                if(!outputFile.exists())
                {
                    if (!outputFile.createNewFile())
                        return false;
                }
                else
                {
                    continue;
                }
            }
            catch (IOException e)
            {
                return false;
            }

            //if (!AssetsUtils.copyRawFileFromAssets(assetManager, sourceFilePath, outputFile))
            if (!AssetsUtils.copyCompressedFileFromAssets(assetManager, sourceFilePath, outputFile))
                return false;
        }

        return true;
    }

    private boolean cacheExamples()
    {
        String source = "examples.zip";
        String destination = prependWithExternalCacheDir("examples");

        File examplesDir = new File(destination);
        if (examplesDir.exists())
            return true;

        if (examplesDir.mkdir())
        {
            return AssetsUtils.unzipFileFromAssets(getAssets(), source, destination);
        }

        return false;
    }

    public ViewGroup initialize(Activity activity)
    {
        if (!initialized)
        {
            mainWindow = new RadJavLayout(activity);
            initialized = true;

            cacheJsEngineFiles();
            cacheExamples();
        }

        return mainWindow;
    }

    public void run(final String appFile)
    {
        paused = false;

        if (!thread_started)
        {
            final RadJavApplication app = this;

            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    RadJavRun(app, mainWindow, prependWithExternalCacheDir(""), appFile);
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

    public native int RadJavRun(RadJavApplication application, ViewGroup mainViewGroup, String cacheDir, String appFile);
}
