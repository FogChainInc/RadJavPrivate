package com.fogchain.radjavvm;

import java.nio.ByteBuffer;

public class UiCallback implements Runnable
{
    static {
        System.loadLibrary("RadJavWrapper");
    }

    private ByteBuffer callbackData;

    UiCallback(ByteBuffer data)
    {
        this.callbackData = data;
    }

    @Override
    public void run()
    {
        RadJavUiCallback(callbackData);
    }

    private native void RadJavUiCallback(ByteBuffer data);
}
