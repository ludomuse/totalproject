package org.cocos2dx.cpp.jniFacade;

import java.io.File;
import java.util.List;

import org.cocos2dx.cpp.DebugManager;
import org.cocos2dx.cpp.sockets.CallBackMethod;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;

import android.app.Activity;

/**
 * This is a wrapper for the WifiDirectManager, that expose simple method such
 * as onReceive(data) or send(data)F
 * 
 * @author Gregoire
 * 
 */
public class WifiDirectFacade {

	private WifiDirectManager _manager;
	
	//There, we set callback that are going to be called by the WifiDirectManager
	//when receiving data such as int, char, byte, string, ....
	private CallBackMethod cmOnGettingsPeers = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onGettingPeers(_manager.getDeviceList());
		}
	};
	private CallBackMethod cmOnReceiveString = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((String) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveInt = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((Integer) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveBool = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((Boolean) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveFloat = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((Float) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveDouble = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((Double) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveByte = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((Byte) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveLong = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((Long) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveFile = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((File) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveByteArray = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((byte[]) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveChar = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((Character) vars[0]);
		}
	};
	
	public WifiDirectFacade(Activity activity)
	{

		JniJavaFacade._wifiDirectFacade = this;

		WifiDirectManager.registerCallBackReceiver(cmOnReceiveString,
				cmOnReceiveInt, cmOnReceiveBool, cmOnReceiveFloat,
				cmOnReceiveDouble, cmOnReceiveByte, cmOnReceiveLong,
				cmOnReceiveFile, cmOnReceiveByteArray, cmOnReceiveChar);

		_manager = new WifiDirectManager(activity);
		//If true, wifi manager will automatically reconnect when is deconnected
		_manager.autoReconnect = true;
		//allow wifi manager to relaunch discovery service when the list of found devices
		//decrease
		_manager.enabledAutoRelanchingServiceDiscoverPeers = false;
		//If true, it will launch a connection request, even if a connection was already launched
		//(but still not answered)
		_manager.forceConnectionRequest = false;
		_manager.initialize();
		//should be done in the exit method of the application
		_manager.clear();
//		LudoMuseThread.stopAllLudoMuseThread();
		
		DebugManager.print("WifiDirectFacade is created !",
				WifiDirectManager.DEBUGGER_CHANNEL);
		
		
		DebugManager.print("tablet name is",
				WifiDirectManager.DEBUGGER_CHANNEL);
	}

	public void onGettingPeers(List<String> peers)
	{
		DebugManager.print("facade has got peers !",
				WifiDirectManager.DEBUGGER_CHANNEL);
		JniCppFacade.onGettingPeers(peers);
		DebugManager.print("facade has got peers ! - end",
				WifiDirectManager.DEBUGGER_CHANNEL);
	}

	public void onReceiving(String s)
	{
		JniCppFacade.onReceivingString(s);
	}

	public void onReceiving(int i)
	{
		DebugManager.print("Java WifiDirectFacade received int",
				WifiDirectManager.DEBUGGER_CHANNEL);
		JniCppFacade.onReceivingInt(i);
	}

	public void onReceiving(boolean b)
	{
		JniCppFacade.onReceivingBool(b);
	}

	public void onReceiving(long l)
	{
		JniCppFacade.onReceivingLong(l);
	}

	public void onReceiving(File f)
	{
		JniCppFacade.onReceivingFile(f.getAbsolutePath());
	}

	public void onReceiving(double d)
	{
		JniCppFacade.onReceivingDouble(d);
	}

	public void onReceiving(float f)
	{
		JniCppFacade.onReceivingFloat(f);
	}

	public void onReceiving(char c)
	{
		JniCppFacade.onReceivingChar(c);
	}

	public void onReceiving(byte b)
	{
		JniCppFacade.onReceivingByte(b);
	}

	public void onReceiving(byte[] bytes)
	{
		JniCppFacade.onReceivingBytes(bytes);
	}

	public void discoverPeers()
	{
		_manager.launchServicePeersDiscovering(cmOnGettingsPeers);
	}

	public void connectTo(String deviceName)
	{
		DebugManager.print("peer is: " + deviceName, WifiDirectManager.DEBUGGER_CHANNEL);
		_manager.setPeerName(deviceName);
	}

	public void send(String s)
	{
		_manager.send(s);
	}

	public void send(int i)
	{
		_manager.send(i);
	}

	public void send(boolean b)
	{
		_manager.send(b);
	}

	public void send(long l)
	{
		_manager.send(l);
	}

	public void send(File f)
	{
		_manager.send(f);
	}

	public void send(double d)
	{
		_manager.send(d);
	}

	public void send(float f)
	{
		_manager.send(f);
	}

	public void send(char c)
	{
		_manager.send(c);
	}

	public void send(byte b)
	{
		_manager.send(b);
	}

	public void send(byte[] bytes)
	{
		_manager.send(bytes);
	}

	public void pause()
	{
		_manager.pause();
	}

	public void resume()
	{
		_manager.resume();
	}

	public void clear()
	{
		_manager.clear();	
	}
	
	public Activity getActivity()
	{
		return _manager.getActivity();
	}
	
	public String getThisDeviceName()
	{
		return _manager.getThisDeviceName();
	}
	
	public static void setServerTempFileName(String name)
	{
		WifiDirectManager.setServerTempFileName(name);
	}
}
