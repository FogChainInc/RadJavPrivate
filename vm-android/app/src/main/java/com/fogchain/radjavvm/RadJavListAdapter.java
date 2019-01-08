package com.fogchain.radjavvm;

import android.database.DataSetObserver;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListAdapter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class RadJavListAdapter implements ListAdapter {

    public enum CallbackType
    {
        ARE_ALL_ITEMS_ENABLED,
        IS_ENABLED,
        REGISTER_DATA_SET_OBSERVER,
        UNREGISTER_DATA_SET_OBSERVER,
        GET_COUNT,
        GET_ITEM,
        GET_ITEM_ID,
        HAS_STABLE_IDS,
        GET_VIEW,
        GET_ITEM_VIEW_TYPE,
        GET_VIEW_TYPE_COUNT,
        IS_EMPTY,
        GET_AUTO_FILL_OPTIONS
    }

    private HashMap<CallbackType, NativeCallback> callbacks = new HashMap<>();
    private List<DataSetObserver> observers = new ArrayList<>();

    public RadJavListAdapter() {
    }

    public void setCallback(CallbackType type, NativeCallback callback) {
        callbacks.put(type, callback);
    }

    @Override
    public boolean areAllItemsEnabled() {
        NativeCallback nativeCallback = callbacks.get(CallbackType.ARE_ALL_ITEMS_ENABLED);
        if (nativeCallback != null)
            return (boolean) nativeCallback.run();

        return true;
    }

    @Override
    public boolean isEnabled(int position) {
        NativeCallback nativeCallback = callbacks.get(CallbackType.IS_ENABLED);
        if (nativeCallback != null)
            return (boolean) nativeCallback.run(position);

        return true;
    }

    @Override
    public void registerDataSetObserver(DataSetObserver observer) {
//        NativeCallback nativeCallback = callbacks.get(CallbackType.REGISTER_DATA_SET_OBSERVER);
//        if (nativeCallback != null)
//            nativeCallback.run(observer);
        observers.add(observer);
    }

    @Override
    public void unregisterDataSetObserver(DataSetObserver observer) {
//        NativeCallback nativeCallback = callbacks.get(CallbackType.UNREGISTER_DATA_SET_OBSERVER);
//        if (nativeCallback != null)
//            nativeCallback.run(observer);
        observers.remove(observer);
    }

    @Override
    public int getCount() {
        NativeCallback nativeCallback = callbacks.get(CallbackType.GET_COUNT);
        if (nativeCallback != null)
            return (int) nativeCallback.run();

        return 0;
    }

    @Override
    public Object getItem(int position) {
        NativeCallback nativeCallback = callbacks.get(CallbackType.GET_ITEM);
        if (nativeCallback != null)
            return nativeCallback.run(position);

        return null;
    }

    @Override
    public long getItemId(int position) {
        NativeCallback nativeCallback = callbacks.get(CallbackType.GET_ITEM_ID);
        if (nativeCallback != null)
            return (long) nativeCallback.run(position);

        return 0;
    }

    @Override
    public boolean hasStableIds() {
        NativeCallback nativeCallback = callbacks.get(CallbackType.HAS_STABLE_IDS);
        if (nativeCallback != null)
            return (boolean) nativeCallback.run();

        return false;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        NativeCallback nativeCallback = callbacks.get(CallbackType.GET_VIEW);
        if (nativeCallback != null)
            return (View) nativeCallback.run(position, convertView, parent);

        return null;
    }

    @Override
    public int getItemViewType(int position) {
        NativeCallback nativeCallback = callbacks.get(CallbackType.GET_ITEM_VIEW_TYPE);
        if (nativeCallback != null)
            return (int) nativeCallback.run(position);

        return 0;
    }

    @Override
    public int getViewTypeCount() {
        NativeCallback nativeCallback = callbacks.get(CallbackType.GET_VIEW_TYPE_COUNT);
        if (nativeCallback != null)
            return (int) nativeCallback.run();

        return 1;
    }

    @Override
    public boolean isEmpty() {
        NativeCallback nativeCallback = callbacks.get(CallbackType.IS_EMPTY);
        if (nativeCallback != null)
            return (boolean) nativeCallback.run();

        return true;
    }

    @Override
    public CharSequence[] getAutofillOptions() {
//        NativeCallback nativeCallback = callbacks.get(CallbackType.GET_AUTO_FILL_OPTIONS);
//        if (nativeCallback != null)
//            return (CharSequence[]) nativeCallback.run();

        return new CharSequence[0];
    }

    private void notifyChanged() {
        for (DataSetObserver observer : observers)
        {
            observer.onChanged();
        }
    }

    private void notifyInvalidated() {
        for (DataSetObserver observer : observers)
        {
            observer.onInvalidated();
        }
    }
}
