package org.cocos2dx.cpp;

import java.util.Set;

import org.cocos2dx.cpp.sockets.ServerSocketHandler;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;

public class LudoMuseThread {

	public static void start(Runnable runnable)
	{
		Thread t = new Thread(runnable);
		t.setName("LudoMuseThread");
		t.start();
	}
	
	public static void stopAllLudoMuseThread()
	{
		Set<Thread> threadSet = Thread.getAllStackTraces().keySet();
		Thread[] threadArray = threadSet.toArray(new Thread[threadSet.size()]);
		for(Thread t : threadArray)
		{
			DebugManager.print(ServerSocketHandler.GetTag() + " " + t.getName(), WifiDirectManager.DEBUGGER_CHANNEL);
			if(t.getName().equals("LudoMuseThread"))
			{
				DebugManager.print(ServerSocketHandler.GetTag() + " killing server + " + t.getId() + " " + t.getName(), WifiDirectManager.DEBUGGER_CHANNEL);
				//t.destroy();
				//t.interrupt();
			}
		}
	}
}
