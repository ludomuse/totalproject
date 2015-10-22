package org.cocos2dx.cpp.sockets;

import java.io.File;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Enumeration;

/**
 * Socket Handler is a wrapper class that allow to manager easily a TCP
 * connection, by exposing simple interface to access server and client method
 * on this device
 * 
 * @author Gregoire, IHMTEK
 * 
 */
public class SocketHandler {

	private ClientSocketHandler client;
	private ServerSocketHandler server;

	private int listenningPort;
	private MailBox mailBox;

	public SocketHandler(int bufferLen, int listenningPort)
	{
		client = new ClientSocketHandler(bufferLen, this);
		this.listenningPort = listenningPort;
		mailBox = new MailBox(client);
	}

	/**
	 * Register all methods you want to be call when server is receiving data
	 * 
	 * @param onReceiveString
	 * @param onReceiveInt
	 * @param onReceiveBool
	 * @param onReceiveFloat
	 * @param onReceiveDouble
	 * @param onReceiveByte
	 * @param onReceiveLong
	 * @param onReceiveFile
	 * @param onReceiveByteArray
	 * @param onReceiveChar
	 */
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

	/**
	 * ask server to stop listenning
	 */
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

	/**
	 * Stop server and client
	 */
	public void notifyIsDisconnectedFromNetwork()
	{
		stop();
		dettachFromRemoteHost();
	}

	/**
	 * Ask server to listen on port "port" and address "address"
	 * 
	 * @param port
	 * @param address
	 */
	public void listen(int port, String address)
	{
		server = new ServerSocketHandler(port, address, this);
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

	public String getIP()
	{
		return server.getServerIpAddress();
	}

	public void sendAccuse(long idReceived)
	{

		if (mailBox.isEmpty())
		{
			/*
			 * Accuse id are the negatif of the id from the message they accuse.
			 * The reason that accuse do not generate id, is that another
			 * message can arrive and be send by the time this accuse is send
			 * (because it's asynchronous). Exemple : id = 1 maiBox.isEmpty
			 * ->msgArrive ->msgGenerateId (static id = id ++ = 2)
			 * ->AccuseGenerateID (static id = id ++ = 3) ->Accuse sent with
			 * previously generated id (3) ->msg sent with previously generated
			 * id (3) --> msg and accuse are send with same id, so one of them
			 * will be ignored (and for all time, because considered like a
			 * doublon)
			 */
			ClientSocketHandler.setId(-idReceived);
			/*
			 * In order to not mask other message, accuse are concatenate with
			 * next message. But, if there is no next message to be send ( =
			 * mailbox is empty and can stay empty) we will send an asynchronous
			 * accuse
			 */
			client.sendAsyncAccuse();
		}
		else
		{
			/*
			 * if mailbox is not empty, we need to concatenate accuse to the
			 * next sending (so send it in the same time that the next message)
			 * in order to not make interferate both message
			 */
			client.concatAccuseInNextSend = true;
			client.accuseId = -idReceived;
		}
	}

	public static String getAnIpAddresForThisDevice()
	{
		return getStringLocalIPAddress();
		// return getDottedDecimalIP(getLocalIPAddress());
	}

	// private static String getLocalIPAddress()
	// {
	// try
	// {
	// for (Enumeration<NetworkInterface> en = NetworkInterface
	// .getNetworkInterfaces(); en.hasMoreElements();)
	// {
	// NetworkInterface intf = en.nextElement();
	// for (Enumeration<InetAddress> enumIpAddr = intf
	// .getInetAddresses(); enumIpAddr.hasMoreElements();)
	// {
	// InetAddress inetAddress = enumIpAddr.nextElement();
	// if (!inetAddress.isLoopbackAddress())
	// {
	// if (inetAddress instanceof Inet4Address)
	// { // fix for Galaxy Nexus. IPv4 is easy to use :-)
	// return getDottedDecimalIP(inetAddress.getAddress());
	// }
	// else
	// {
	// return inetAddress.getHostAddress().toString();
	// }
	// }
	// }
	// }
	// }
	// catch (SocketException ex)
	// {
	// // Log.e("AndroidNetworkAddressFactory", "getLocalIPAddress()", ex);
	// }
	// catch (NullPointerException ex)
	// {
	// // Log.e("AndroidNetworkAddressFactory", "getLocalIPAddress()", ex);
	// }
	// return null;
	// }

	private static String getStringLocalIPAddress()
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
						return inetAddress.getHostAddress();
					}
				}
			}
		}
		catch (Exception ex)
		{

		}
		return null;
	}

	// private static String getDottedDecimalIP(byte[] ipAddr)
	// {
	// // convert to dotted decimal notation:
	// String ipAddrStr = "";
	// for (int i = 0; i < ipAddr.length; i++)
	// {
	// if (i > 0)
	// {
	// ipAddrStr += ".";
	// }
	// ipAddrStr += ipAddr[i] & 0xFF;
	// }
	// return ipAddrStr;
	// }

	/**
	 * Set the client remote host = the address and port of the remote server
	 * which the client will try to contact
	 * 
	 * @param hostAddress
	 * @param listenningPort
	 */
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

	/**
	 * clean properly / terminate all thread and timer
	 */
	public void stopHandlers()
	{
		if (server != null)
			server.stopHandlers();
		if (client != null)
			client.stopHandlers();
		mailBox.stopHandlers();
	}

	/**
	 * send actual message in mailbox
	 */
	void send()
	{
		mailBox.send();
	}

	/**
	 * send next message in mailbox
	 */
	void sendNext()
	{
		mailBox.sendNext();
	}
}
