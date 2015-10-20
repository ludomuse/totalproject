package org.cocos2dx.cpp.sockets;

import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.text.SimpleDateFormat;
import java.util.Calendar;

import org.cocos2dx.cpp.DebugManager;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;

import android.os.Handler;

class ConnectTask2 implements Runnable {
	private String host;
	private int port;
	private Socket socket;
	private CallBackMethod cm;
	
	public ConnectTask2(String host, int port, Socket socket, CallBackMethod cm)
	{
		this.host = host;
		this.port = port;
		this.socket = socket;
		this.cm = cm;
	}

	public void execute()
	{
		new Thread(this).start();
	}

	public void run()
	{

		try
		{
			socket.bind(null);
			DebugManager.print(ClientSocketHandler.GetTag() + "Trying to reach address " + host + ":" + port,
					WifiDirectManager.DEBUGGER_CHANNEL);
			socket.connect(new InetSocketAddress(host, port), 500);
			// socket.connect((new InetSocketAddress(host, port)), 500);
			// DebugManager.print(socket.isConnected() ? "connected to server" :
			// "connection to server fail", WifiDirectManager.DEBUGGER_CHANNEL);
			if (cm != null)
				cm.Do();
		}
		catch (IllegalArgumentException e)
		{
			// Log.d("A", e.getMessage() + "");
			DebugManager.print(ClientSocketHandler.GetTag() + 
					"argument exception occure during connection to server at "
							+ host + ":" + port,
					WifiDirectManager.DEBUGGER_CHANNEL);
		}
		catch (IOException e)
		{
			// Log.d("A", e.getMessage() + "");
			DebugManager.print(ClientSocketHandler.GetTag() + 
					"IO Exception occure during connection to server at "
							+ host + ":" + port + ". Error: "
							+ e.getLocalizedMessage(),
					WifiDirectManager.DEBUGGER_CHANNEL);
		}
	}
}

public class ClientSocketHandler {

	private Socket socket;

	private byte[] buf;

	private static long id = 0;
	
	private byte[] getAnId()
	{
		return toByte(id++);
	}
	
	public ClientSocketHandler(int len)
	{
		super();
		buf = new byte[len];
		// socket = new Socket();

	}
	
	public static String GetTag()
	{
		Calendar cal = Calendar.getInstance();
        SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
		return "[LUDOCLIENT][" + sdf.format(cal.getTime()) + "]";
	}

	public boolean isConnected()
	{
		return socket != null && socket.isConnected() && remoteIp != null;
	}

	private Handler handler = new Handler();
	private Runnable worker;

	private String remoteIp = null;
	private int remotePort = -1;

	public void setRemoteHost(String ip, int port)
	{
		remoteIp = ip;
		remotePort = port;
	}

	private void closeOutputStream(OutputStream os)
	{
		try
		{
			if (os != null)
				os.close();
		}
		catch (IOException e)
		{
			DebugManager.print(ClientSocketHandler.GetTag() + "error while closing outputstream",
					WifiDirectManager.DEBUGGER_CHANNEL);
		}
	}

	public boolean isDettachedFromRemoteHost()
	{
		return remoteIp == null;
	}

	public void dettachFromRemoteHost()
	{
		remoteIp = null;
	}

	private void connect(final CallBackMethod cm)
	{
		if (remoteIp == null)
		{
			DebugManager.print(ClientSocketHandler.GetTag() + "Not connected to host",
					WifiDirectManager.DEBUGGER_CHANNEL);

			return;
		}

		worker = new Runnable() {

			@Override
			public void run()
			{
				if (remoteIp == null)
				{
					DebugManager
							.print("Ip from remost host is null. Stoping connection now.",
									WifiDirectManager.DEBUGGER_CHANNEL);
					return;
				}
				if (socket != null && socket.isConnected()
						&& !socket.isClosed())
				{
					DebugManager.print(ClientSocketHandler.GetTag() + "Connected to server !",
							WifiDirectManager.DEBUGGER_CHANNEL);
					cm.Do();
				}
				else
				{
					DebugManager.print(ClientSocketHandler.GetTag() + 
							"Connection to server fail. Trying again...",
							WifiDirectManager.DEBUGGER_CHANNEL);
					socket = new Socket();
					ConnectTask2 connectTask = new ConnectTask2(remoteIp,
							remotePort, socket, null);
					connectTask.execute();
					handler.postDelayed(worker, 2000);
				}
			}

		};
		handler.post(worker);
	}

	public OutputStream openOutputStream()
	{
		try
		{
			return socket.getOutputStream();
		}
		catch (IOException e)
		{
			DebugManager.print(ClientSocketHandler.GetTag() + 
					"error while openning outputstream: "
							+ e.getLocalizedMessage(),
					WifiDirectManager.DEBUGGER_CHANNEL);
			return null;
		}
	}

	private byte[] toByte(String str)
	{
		int len = str.length();
		byte[] res = new byte[len + 1];
		for (int i = 0; i < len; i++)
		{
			res[i] = (byte) str.charAt(i);
		}
		res[len] = (byte) '\0';
		return res;
	}

	private byte[] toByte(boolean b)
	{
		byte byt = (byte) (b ? 1 : 0);
		return new byte[] { byt };
	}

	private byte[] toByte(int i)
	{
		return new byte[] { (byte) i };
	}

	private byte[] toByte(float f)
	{
		return ByteBuffer.allocate(4).putFloat(f).array();
	}

	private byte[] toByte(long l)
	{
		return ByteBuffer.allocate(Long.SIZE).putLong(l).array();
	}

	private byte[] toByte(double d)
	{
		return ByteBuffer.allocate(8).putDouble(d).array();
	}

	public void send(File f)
	{
		DebugManager.print(ClientSocketHandler.GetTag() + "sending file " + f.getName(),
				WifiDirectManager.DEBUGGER_CHANNEL);
		try
		{
			send(new BufferedInputStream(new FileInputStream(f)),
					PACKET_TYPE.FILE);
		}
		catch (FileNotFoundException e)
		{
			DebugManager.print(ClientSocketHandler.GetTag() + "error occured while sending file",
					WifiDirectManager.DEBUGGER_CHANNEL);
		}

	}

	public void send(double d)
	{
		DebugManager.print(ClientSocketHandler.GetTag() + "sending double " + d,
				WifiDirectManager.DEBUGGER_CHANNEL);

		send(toByte(d), PACKET_TYPE.DOUBLE);
	}

	public void sendBytes(byte[] bytes)
	{
		int len = bytes.length;
		/* debug */
		String bytesStr = "";
		for (int i = 0; i < len; i++)
		{
			bytesStr += bytes[i] + "-";
		}

		DebugManager.print(ClientSocketHandler.GetTag() + "sending bytes " + bytesStr,
				WifiDirectManager.DEBUGGER_CHANNEL);
		/* end debug */

		send(bytes, PACKET_TYPE.BYTE_ARRAY);
	}

	public void send(long l)
	{
		DebugManager.print(ClientSocketHandler.GetTag() + "sending long " + l,
				WifiDirectManager.DEBUGGER_CHANNEL);
		send(toByte(l), PACKET_TYPE.LONG);
	}

	public void send(float f)
	{
		DebugManager.print(ClientSocketHandler.GetTag() + "sending float " + f,
				WifiDirectManager.DEBUGGER_CHANNEL);
		send(toByte(f), PACKET_TYPE.FLOAT);
	}

	public void send(byte b)
	{
		DebugManager.print(ClientSocketHandler.GetTag() + "sending byte " + b,
				WifiDirectManager.DEBUGGER_CHANNEL);
		send(toByte(b), PACKET_TYPE.BYTE);
	}

	public void send(char c)
	{
		DebugManager.print(ClientSocketHandler.GetTag() + "sending char " + c,
				WifiDirectManager.DEBUGGER_CHANNEL);
		send(toByte(c), PACKET_TYPE.CHAR);
	}

	public void send(int i)
	{
		DebugManager.print(ClientSocketHandler.GetTag() + "sending int " + i,
				WifiDirectManager.DEBUGGER_CHANNEL);
		send(toByte(i), PACKET_TYPE.INT);
	}

	public void send(boolean b)
	{
		DebugManager.print(ClientSocketHandler.GetTag() + "sending boolean " + b,
				WifiDirectManager.DEBUGGER_CHANNEL);
		send(toByte(b), PACKET_TYPE.BOOL);
	}

	public void send(String str)
	{
		DebugManager.print(ClientSocketHandler.GetTag() + "sending string " + str,
				WifiDirectManager.DEBUGGER_CHANNEL);
		send(toByte(str), PACKET_TYPE.STRING);
	}

	public void sendAccuse()
	{
		DebugManager.print(ClientSocketHandler.GetTag() + "sending accuse...",
				WifiDirectManager.DEBUGGER_CHANNEL);
		send(new byte[] {}, PACKET_TYPE.ACCUSE);
	}

	public void sendIP(int clientListenningPort)
	{
		DebugManager.print(ClientSocketHandler.GetTag() + "sending IP...", WifiDirectManager.DEBUGGER_CHANNEL);
		String address = getClientIpAddress() + "!" + clientListenningPort;
		send(toByte(address), PACKET_TYPE.IP);
	}

	public void notifyServer()
	{
		DebugManager.print(ClientSocketHandler.GetTag() + "Sending alive packet...",
				WifiDirectManager.DEBUGGER_CHANNEL);
		send(new byte[] {}, PACKET_TYPE.KEEP_ALIVE);
	}

	public String getClientIpAddress()
	{
		return SocketHandler.getThisDeviceIpAddress();
	}

	private void send(byte[] bytes, PACKET_TYPE type)
	{
		send(new ByteArrayInputStream(bytes), type);
	}

	private void send(final InputStream inputStream, final PACKET_TYPE type)
	{
		CallBackMethod cm = new CallBackMethod() {

			@Override
			public void Do(Object... vars)
			{
				try
				{
					// stopServerNotificator();

					OutputStream outputStream = openOutputStream();
					// InputStream inputStream = new
					// ByteArrayInputStream(bytes);
					outputStream.write(type.toInt());
					outputStream.write(getAnId());
					int len;
					while ((len = inputStream.read(buf)) != -1)
					{
						outputStream.write(buf, 0, len);
					}
					// outputStream.close();
					inputStream.close();
					closeOutputStream(outputStream);
					socket.close();

					// rearmServerNotificator();
				}
				catch (Exception e)
				{
					DebugManager.print(ClientSocketHandler.GetTag() + "error occure while sending stream : "
							+ e.getLocalizedMessage(),
							WifiDirectManager.DEBUGGER_CHANNEL);
				}
			}

		};

		connect(cm);

	}

	public void stopHandlers()
	{
		handler.removeCallbacks(worker);

	}

}