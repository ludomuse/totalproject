package org.cocos2dx.cpp.sockets;

import java.util.ArrayList;
import java.util.List;

import org.cocos2dx.cpp.DebugManager;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;

import android.os.Handler;

class Message {
	public String methodName;
	public Object[] args;
	public Class<?>[] parameterTypes;

	private ClientSocketHandler client;

	public Message(ClientSocketHandler client, String methodName,
			Object[] args, Class<?>[] parameterTypes)
	{
		super();
		this.client = client;
		this.methodName = methodName;
		this.args = args;
		this.parameterTypes = parameterTypes;
	}

	public void send()
	{
		try
		{
			ClientSocketHandler.setId(ClientSocketHandler.getId());
			ClientSocketHandler.class.getMethod(methodName, parameterTypes)
					.invoke(client, args);
		}
		catch (Exception e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}

public class MailBox {

	private ClientSocketHandler client;
	private List<Message> messages = new ArrayList<Message>();

	private boolean _sendFirstMessageWhenPosted = false;
	
	public MailBox(ClientSocketHandler client)
	{
		this.client = client;
	}

	public void post(String methodName, Object[] args, Class<?>[] argType, int position)
	{
		Message msg = new Message(client, methodName, args, argType);
		messages.add(position, msg);
		DebugManager.print(ClientSocketHandler.GetTag() + " MailBox = " + messages.size() + "/" + position, WifiDirectManager.DEBUGGER_CHANNEL);
		if(_sendFirstMessageWhenPosted && messages.size() == 1)
		{
			_sendFirstMessageWhenPosted = false;
			send();
		}
	}


	public void post(String methodName, Object arg, Class<?> argType)
	{
		DebugManager.print(ClientSocketHandler.GetTag() +  "Message.size() = " + messages.size(), WifiDirectManager.DEBUGGER_CHANNEL);
		post(methodName, arg, argType, messages.size());
	}
	
	public void post(String methodName, Object arg, Class<?> argType, int position)
	{
		post(methodName, new Object[] { arg }, new Class<?>[] { argType }, position);
	}


	
	public static int timeBeforeConsideringMessageLost = 1000;
	Handler timeout = new Handler();
	Runnable worker = new Runnable()
	{

		@Override
		public void run()
		{
			send();
		}
		
	};
	private void armTimeOut()
	{
		timeout.postDelayed(worker, timeBeforeConsideringMessageLost);
	}
	
	private void unarmTimeOut()
	{
		timeout.removeCallbacks(worker);
	}
	
	public void send()
	{
		unarmTimeOut();
		//If the list is empty, wait for the next message to be post and send it
		if(messages.size() == 0)
		{
			_sendFirstMessageWhenPosted = true;
		}
		else
		{
			messages.get(0).send();
			armTimeOut();
		}
	}
	
	public boolean isEmpty()
	{
		return messages.size() == 0;
	}
	
	public void sendNext()
	{
		if(!isEmpty())
			messages.remove(0);
		ClientSocketHandler.generateId();
		send();
	}

	public void stopHandlers()
	{
		unarmTimeOut();
	}
}
