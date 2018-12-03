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

import android.view.ContextMenu;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.widget.CompoundButton;

import java.nio.ByteBuffer;

//TODO: Implement other interfaces
public class UiEventListener implements View.OnClickListener,
                                        View.OnLongClickListener,
                                        View.OnFocusChangeListener,
                                        View.OnKeyListener,
                                        View.OnTouchListener,
                                        View.OnCreateContextMenuListener,
                                        android.widget.CompoundButton.OnCheckedChangeListener
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

    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        RadJavUiEvent(_data);
    }

    @Override
    public void onFocusChange(View v, boolean hasFocus) {
        RadJavUiEvent(_data);
    }

    @Override
    public boolean onKey(View v, int keyCode, KeyEvent event) {
        return RadJavUiEvent(_data);
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        return RadJavUiEvent(_data);
    }

    @Override
    public void onCreateContextMenu(ContextMenu menu, View v, ContextMenu.ContextMenuInfo menuInfo) {
        RadJavUiEvent(_data);
    }
}
