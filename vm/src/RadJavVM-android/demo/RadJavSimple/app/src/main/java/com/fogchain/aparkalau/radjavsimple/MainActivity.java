package com.fogchain.aparkalau.radjavsimple;

import android.content.res.AssetManager;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'radjav-native' library on application startup.
    static {
        System.loadLibrary("radjav-native");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //initialize cached assets if necessary
        File externalCacheDir = this.getExternalCacheDir();
        String absolutePath = null;
        if (externalCacheDir != null) {
            absolutePath = externalCacheDir.getAbsolutePath();

        }

        prepareCachedFiles(externalCacheDir);
        absolutePath += "/probe.xrj";

        String currentPath = System.getProperty("user.dir");

        int tmp = initializeVM(absolutePath, absolutePath/*currentPath*/, "");
        tmp += runApplication();
        tmp += runApplicationFromFile();
        shutdownVM();

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI().concat(Integer.toString(tmp)));
    }

    private void prepareCachedFiles(File externalCacheDir) {
        //copy files from assets if not exist in cache

        AssetManager assetManager = getAssets();

        File v8_natives = new File(externalCacheDir.getPath() + "/natives_blob.bin");
        if (!v8_natives.exists()) {
            copyAsset(assetManager, "natives_blob.bin", v8_natives);
        }

        File v8_snapshot = new File(externalCacheDir.getPath() + "/snapshot_blob.bin");
        if (!v8_snapshot.exists()) {
            copyAsset(assetManager, "snapshot_blob.bin", v8_snapshot);
        }

        File v8_icu_data = new File(externalCacheDir.getPath() + "/icudtl.dat");
        if (!v8_icu_data.exists()) {
            copyAsset(assetManager, "icudtl.dat", v8_icu_data);
        }

        File probe_xrj = new File(externalCacheDir.getPath() + "/probe.xrj");
        if (!probe_xrj.exists()) {
            copyAsset(assetManager, "probe.xrj", probe_xrj);
        }
    }

    private static boolean copyAsset(AssetManager assetManager,
                                     String fromAssetPath, File to) {
        InputStream in = null;
        OutputStream out = null;
        try {
            in = assetManager.open(fromAssetPath);
            out = new FileOutputStream(to);
            copyFile(in, out);
            in.close();
            in = null;
            out.flush();
            out.close();
            out = null;
            return true;
        } catch(Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    private static void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1){
            out.write(buffer, 0, read);
        }
    }
    /**
     * A native method that is implemented by the 'radjav-native' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native int initializeVM(String execPath, String filePath, String parameters);
    public native void shutdownVM();
    public native int runApplication();
    public native int runApplicationFromFile();
}
