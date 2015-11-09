//package com.IHMTEK.LudoMuse;
package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Intent;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.Size;

import org.cocos2dx.cpp.jniFacade.WifiDirectFacade;
import org.cocos2dx.cpp.jniFacade.JniCppFacade;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;
import org.cocos2dx.lib.Cocos2dxActivity;

import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.widget.Toast;

import java.io.File;
import java.io.IOException;
import java.lang.Override;
import java.util.List;

/**
 * This is the main activity for LudoMuse. THERE MUST NOT BE ANY GRAPHICS
 * COMPONENTS (Buttons, TextFiels...) IN THE WHOLE JAVA PART OF LUDOMUSE, BUT
 * ESPECIALLY IN THIS CLASS. If there are any graphics component, they might
 * show up upon LudoMuse startup, regardless of the AppDelegate or any C++ entry
 * point This class is to replace the MainActivity.java class, but should not
 * depend on any graphics component as stated earlier.
 */

public class AppActivity extends Cocos2dxActivity {
	private static AppActivity instance;

	private WifiDirectFacade _wifiFacade;

	public static Activity getInstance()
	{
		return instance;
	}

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		// DebugManager.printInfo();
		instance = this;
		super.onCreate(savedInstanceState);

		_wifiFacade = new WifiDirectFacade(this);
		
		if ((getIntent().getFlags() & Intent.FLAG_ACTIVITY_BROUGHT_TO_FRONT) != 0) 
		{
		    finish();
		    return;
		}

	}

	@Override
	protected void onStop()
	{
		DebugManager.print("[FINISHING] on stop",
				WifiDirectManager.DEBUGGER_CHANNEL);
		//_wifiFacade.clear(); // TODO totest
		super.onStop();
	}

	/* register the broadcast receiver with the intent values to be matched */
	@Override
	protected void onResume()
	{
		super.onResume();
		_wifiFacade.resume();
	}

	/* unregister the broadcast receiver */
	@Override
	protected void onPause()
	{
		super.onPause();
		_wifiFacade.pause();
	}

	// Taking photo

	static final int REQUEST_TAKE_PHOTO = 1;

	public void dispatchTakePictureIntent()
	{
		//4096 * 4096
		/*Camera camera = Camera.open();
		Parameters param = camera.getParameters();
		List<Size> sizes = param.getSupportedPictureSizes();
	    Camera.Size size = sizes.get(0);
	    for (int i = 0; i < sizes.size(); i++) {
	        if (sizes.get(i).width > size.width)
	            size = sizes.get(i);
	    }
	    param.setPictureSize(size.width, size.height);*/
	    
		Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
		// Ensure that there's a camera activity to handle the intent
		if (takePictureIntent.resolveActivity(getPackageManager()) != null)
		{
			// Create the File where the photo should go
			File photoFile = null;
			try
			{
				photoFile = createImageFile();
			}
			catch (IOException ex)
			{
				// Error occurred while creating the File
			}
			// Continue only if the File was successfully created
			if (photoFile != null)
			{
				takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT,
						Uri.fromFile(photoFile));
				startActivityForResult(takePictureIntent, REQUEST_TAKE_PHOTO);
			}
		}
	}

	String mCurrentPhotoPath;

	private File createImageFile() throws IOException
	{
		// Create an image file name
		/*String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss")
				.format(new Date());
		String imageFileName = "JPEG_" + timeStamp + "_";*/
		String imageFileName = "MyPic";
		File storageDir = Environment
				.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES);
		File image = File.createTempFile(imageFileName, /* prefix */
				".jpg", /* suffix */
				storageDir /* directory */
		);

		// Save a file: path for use with ACTION_VIEW intents
		mCurrentPhotoPath =image.getAbsolutePath();
		return image;
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
	    super.onActivityResult(requestCode, resultCode, data);

	    if (requestCode == REQUEST_TAKE_PHOTO) {
	        if (resultCode == RESULT_OK) {
	        	Toast.makeText(this, "Image saved to:\n" + mCurrentPhotoPath, Toast.LENGTH_LONG).show();
	        	JniCppFacade.setCurrentPicturePath(mCurrentPhotoPath);
	          //  handleAvatarUpload(data); // which uses Uri selectedImage = data.getData();
	        } 
	    }
	}
	
}