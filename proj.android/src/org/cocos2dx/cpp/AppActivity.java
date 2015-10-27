//package com.IHMTEK.LudoMuse;
package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Intent;

import org.cocos2dx.cpp.jniFacade.WifiDirectFacade;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.os.Bundle;

import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.MotionEvent;

import java.lang.Override;


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
		
	}
	
    


	@Override
	protected void onStop()
	{
		DebugManager.print("[FINISHING] on stop", WifiDirectManager.DEBUGGER_CHANNEL);
	    _wifiFacade.clear(); //TODO totest
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


}