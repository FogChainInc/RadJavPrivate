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

import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.channels.FileChannel;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class AssetsUtils
{
    public static boolean copyRawFileFromAssets(AssetManager assetManager, String fileName, File destinationFile)
    {
        FileChannel inputChannel = null, outputChannel = null;
        try
        {
            AssetFileDescriptor inputAssetFileDescriptor = assetManager.openFd(fileName);
            FileInputStream inputStream = inputAssetFileDescriptor.createInputStream();
            inputChannel = inputStream.getChannel();
            FileOutputStream outputStream = new FileOutputStream(destinationFile);
            outputChannel = outputStream.getChannel();
            inputChannel.transferTo(inputAssetFileDescriptor.getStartOffset(), inputAssetFileDescriptor.getLength(), outputChannel);
        }
        catch (IOException ioe)
        {
            return false;
        }
        finally
        {
            try
            {
                if (inputChannel != null)
                {
                    inputChannel.close();
                }

                if (outputChannel != null)
                {
                    outputChannel.close();
                }
            }
            catch (IOException e)
            {}
        }

        return true;
    }

    public static boolean copyCompressedFileFromAssets(AssetManager assetManager, String fileName, File destinationFile)
    {
        InputStream inputStream = null;
        OutputStream outputStream = null;

        try
        {
            inputStream = assetManager.open(fileName);

            outputStream = new FileOutputStream(destinationFile);
            moveData(inputStream, outputStream);

        } catch(IOException e)
        {
            return false;
        }
        finally
        {
            try
            {
                if (inputStream != null)
                {
                    inputStream.close();
                }

                if (outputStream != null)
                {
                    outputStream.close();
                }
            }
            catch (IOException e)
            {}
        }

        return true;
    }

    public static boolean unzipFileFromAssets(AssetManager assetManager, String zipFile, String destinationDir)
    {
        try
        {
            InputStream inputStream = assetManager.open(zipFile);
            unzipFile(inputStream, new File(destinationDir));
        }
        catch(IOException e)
        {
            return false;
        }

        return true;
    }

    private static void moveData(InputStream in, OutputStream out) throws IOException
    {
        byte[] buffer = new byte[8192];

        int read;

        while((read = in.read(buffer)) != -1)
        {
            out.write(buffer, 0, read);
        }
    }

    private static void unzipFile(InputStream zipFileInputStream, File targetDirectory) throws IOException
    {
        ZipInputStream zis = new ZipInputStream(
                                new BufferedInputStream(zipFileInputStream));
        try
        {
            ZipEntry ze;
            int count;
            byte[] buffer = new byte[8192];
            while ((ze = zis.getNextEntry()) != null)
            {
                File file = new File(targetDirectory, ze.getName());
                File dir = ze.isDirectory() ? file : file.getParentFile();
                if (!dir.isDirectory() &&
                        !dir.mkdirs())
                {
                    throw new FileNotFoundException("Failed to ensure directory: " +
                            dir.getAbsolutePath());
                }

                if (ze.isDirectory())
                    continue;

                FileOutputStream fout = new FileOutputStream(file);
                try
                {
                    while ((count = zis.read(buffer)) != -1)
                        fout.write(buffer, 0, count);
                }
                finally
                {
                    fout.close();
                }
            }
        }
        finally
        {
            zis.close();
        }
    }
}
