package com.fogchain.radjavvm;

import android.view.View;

import java.nio.ByteBuffer;

//TODO: Implement other interfaces
public class UiEventListener implements View.OnClickListener,
                                        View.OnLongClickListener
{
    static {
        System.loadLibrary("RadJavWrapper");
    }

    public UiEventListener(ByteBuffer data)
    {
        _data = data;
    }

    @Override
    public void onClick(View v)
    {
        RadJavUiEvent(_data);
    }

    @Override
    public boolean onLongClick(View v)
    {
        return RadJavUiEvent(_data);
    }

    private native boolean RadJavUiEvent(ByteBuffer data, Object... args);

    private ByteBuffer _data;
}
