package org.cocos2dx.cpp.sockets;

import java.io.File;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;


public class SocketHandler {

	private ClientSocketHandler client;
	private ServerSocketHandler server;


	private int listenningPort;
	private MailBox mailBox;

	public SocketHandler(int bufferLen, int listenningPort)
	{
		client = new ClientSocketHandler(bufferLen);
		this.listenningPort = listenningPort;
		mailBox = new MailBox(client);
	}

	public static void registerCallBackReceiver(CallBackMethod onReceiveString,
			CallBackMethod onReceiveInt, CallBackMethod onReceiveBool,
			CallBackMethod onReceiveFloat, CallBackMethod onReceiveDouble,
			CallBackMethod onReceiveByte, CallBackMethod onReceiveLong,
			CallBackMethod onReceiveFile, CallBackMethod onReceiveByteArray,
			CallBackMethod onReceiveChar)
	{
		ServerSocketHandler.registerCallBackReceiver(onReceiveString,
				onReceiveInt, onReceiveBool, onReceiveFloat, onReceiveDouble,
				onReceiveByte, onReceiveLong, onReceiveFile,
				onReceiveByteArray, onReceiveChar);
	}


	public boolean isConnected()
	{
		return client.isConnected();
	}

	public void stop()
	{
		if (server != null)
			server.stop();
	}

	public boolean isDettachedFromRemoteHost()
	{
		return client.isDettachedFromRemoteHost();
	}

	public void dettachFromRemoteHost()
	{
		client.dettachFromRemoteHost();
	}

	public void notifyIsDisconnectedFromNetwork()
	{
		stop();
		dettachFromRemoteHost();
	}

	public void listen(int port)
	{
		server = new ServerSocketHandler(port, this);
		server.listen();
	}

	public void send(File f)
	{
		mailBox.post("send", f, File.class);
	}
	
	public void send(byte[] bytes)
	{
		mailBox.post("sendBytes", bytes, byte[].class);
	}
	
	public void send(double d)
	{
		mailBox.post("send", d, double.class);
	}
	
	public void send(long l)
	{
		mailBox.post("send", l, long.class);
	}
	
	public void send(float f)
	{
		mailBox.post("send", f, float.class);
	}

	public void send(byte b)
	{
		mailBox.post("send", b, byte.class);
	}

	public void send(char c)
	{
		mailBox.post("send", c, char.class);
	}

	public void send(int i)
	{
		mailBox.post("send", i, int.class);
	}

	public void send(boolean b)
	{
		mailBox.post("send", b, boolean.class);
	}

	public void send(String str)
	{
		mailBox.post("send", str, String.class);
	}

	private void sendIP()
	{
		mailBox.post("sendIP", listenningPort, int.class, 0);
	}

	public void sendAccuse(long idReceived)
	{
		
		if(mailBox.isEmpty())
		{
			ClientSocketHandler.setId(-idReceived);
			client.notifyServer();
		}
		else
		{
			client.concatAccuseInNextSend = true;
			client.accuseId = -idReceived;
		}
	}

	

	public static String getThisDeviceIpAddress()
	{
		return getDottedDecimalIP(getLocalIPAddress());
	}

	private static byte[] getLocalIPAddress()
	{
		try
		{
			for (Enumeration<NetworkInterface> en = NetworkInterface
					.getNetworkInterfaces(); en.hasMoreElements();)
			{
				NetworkInterface intf = en.nextElement();
				for (Enumeration<InetAddress> enumIpAddr = intf
						.getInetAddresses(); enumIpAddr.hasMoreElements();)
				{
					InetAddress inetAddress = enumIpAddr.nextElement();
					if (!inetAddress.isLoopbackAddress())
					{
						if (inetAddress instanceof Inet4Address)
						{ // fix for Galaxy Nexus. IPv4 is easy to use :-)
							return inetAddress.getAddress();
						}
						// return inetAddress.getHostAddress().toString(); //
						// Galaxy Nexus returns IPv6
					}
				}
			}
		}
		catch (SocketException ex)
		{
			// Log.e("AndroidNetworkAddressFactory", "getLocalIPAddress()", ex);
		}
		catch (NullPointerException ex)
		{
			// Log.e("AndroidNetworkAddressFactory", "getLocalIPAddress()", ex);
		}
		return null;
	}

	private static String getDottedDecimalIP(byte[] ipAddr)
	{
		// convert to dotted decimal notation:
		String ipAddrStr = "";
		for (int i = 0; i < ipAddr.length; i++)
		{
			if (i > 0)
			{
				ipAddrStr += ".";
			}
			ipAddrStr += ipAddr[i] & 0xFF;
		}
		return ipAddrStr;
	}

	public void setOnReceiveIPCallBack(CallBackMethod cm)
	{
		server.setOnReceiveIPCallBack(cm);
	}

	void setRemoteHost(String hostAddress, int listenningPort)
	{
		this.listenningPort = listenningPort;
		client.setRemoteHost(hostAddress, listenningPort);
	}

	public void connectTo(String hostAddress)
	{
		setRemoteHost(hostAddress, listenningPort);
		sendIP();
		send();
	}
	
	public void stopHandlers()
	{
		if (server != null)
			server.stopHandlers();
		if (client != null)
			client.stopHandlers();
		mailBox.stopHandlers();
	}

	void send()
	{
		mailBox.send();
	}
	
	void sendNext()
	{
		mailBox.sendNext();
	}
}
