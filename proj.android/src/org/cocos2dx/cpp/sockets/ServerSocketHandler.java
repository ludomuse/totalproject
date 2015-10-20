package org.cocos2dx.cpp.sockets;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;

import org.cocos2dx.cpp.DebugManager;
import org.cocos2dx.cpp.LudoMuseThread;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;

import android.os.AsyncTask;

/**
 * An instance of this class is created when the server socket accept a
 * connection with a new client. This class concretely handle client message,
 * while the server socket wait for another connection.
 * 
 * @author Gregoire
 * 
 */
class Communication implements Runnable {
	private Socket client;
	private SocketHandler master;

	public Communication(Socket client, SocketHandler master)
	{
		super();
		this.client = client;
		this.master = master;
	}

	/**
	 * read a character into the inputstream 'is'
	 * 
	 * @param is
	 * @return
	 */
	private char readChar(InputStream is)
	{
		try
		{
			return (char) is.read();
		}
		catch (IOException e)
		{
			DebugManager.print(ServerSocketHandler.GetTag()
					+ "error reading char in inputstream",
					WifiDirectManager.DEBUGGER_CHANNEL);
			return '#';
		}
	}

	/**
	 * read a string into the inputstream 'is'
	 * 
	 * @param is
	 * @return
	 */
	private String getStringFromInputStream(InputStream is)
	{

		String res = "";
		char tps;
		while ((tps = readChar(is)) != '\0')
		{
			res += tps;
		}
		return res;
	}

	/**
	 * read a float into the inputstream 'is'
	 * 
	 * @param is
	 * @return
	 */
	private Float getFloatFromInputStream(InputStream is)
	{
		byte[] bytes = new byte[] { (byte) read(is), (byte) read(is),
				(byte) read(is), (byte) read(is) };
		return ByteBuffer.wrap(bytes).getFloat();
	}

	/**
	 * read a double into the inputstream 'is'
	 * 
	 * @param is
	 * @return
	 */
	private Double getDoubleFromInputStream(InputStream is)
	{
		byte[] bytes = new byte[] { (byte) read(is), (byte) read(is),
				(byte) read(is), (byte) read(is), (byte) read(is),
				(byte) read(is), (byte) read(is), (byte) read(is) };
		return ByteBuffer.wrap(bytes).getDouble();
	}

	/**
	 * read a long into the inputstream 'is'
	 * 
	 * @param is
	 * @return
	 */
	private Long getLongFromInputStream(InputStream is)
	{
		byte[] bytes = new byte[Long.SIZE];
		for (int i = 0; i < Long.SIZE; i++)
		{
			bytes[i] = (byte) read(is);

		}
		return ByteBuffer.wrap(bytes).getLong();
	}

	/**
	 * read a byte array into the inputstream 'is'
	 * 
	 * @param is
	 * @return
	 */
	private byte[] getBytesFromInputStream(InputStream is)
	{
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		int tps;
		while ((tps = read(is)) != -1)
		{
			bos.write(tps);
		}

		return bos.toByteArray();
	}

	/**
	 * read a file into the inputstream 'is' The file will be created with the
	 * following path : "/sdcard/Screenshots/tps"
	 * 
	 * @param is
	 * @return
	 */
	private File getFileFromInputStream(InputStream is)
	{
		File f = new File("/sdcard/Screenshots/tps");
		try
		{
			f.createNewFile();
		}
		catch (IOException e1)
		{
			DebugManager.print(ServerSocketHandler.GetTag()
					+ "error while creating new file",
					WifiDirectManager.DEBUGGER_CHANNEL);
		}

		FileOutputStream fos;
		try
		{
			fos = new FileOutputStream(f);
			int tps;
			while ((tps = read(is)) != -1)
			{
				fos.write(tps);
			}
			fos.close();
			return f;
		}
		catch (Exception e)
		{
			DebugManager.print(ServerSocketHandler.GetTag()
					+ "Error while reading file. Default directory is: "
					+ new File(".").getAbsolutePath(),
					WifiDirectManager.DEBUGGER_CHANNEL);
			return null;
		}
	}

	/**
	 * 
	 * @param client
	 * @return client's inputstream
	 */
	private InputStream getClientData(Socket client)
	{
		InputStream is = null;
		try
		{
			is = client.getInputStream();
		}
		catch (IOException e)
		{
			DebugManager.print(ServerSocketHandler.GetTag()
					+ "error while getting client data",
					WifiDirectManager.DEBUGGER_CHANNEL);
		}
		return is;
	}

	/**
	 * close the inputstream is (handle exception for you)
	 * 
	 * @param is
	 */
	private void closeInputStream(InputStream is)
	{
		try
		{
			if (is == null)
			{
				DebugManager.print(ServerSocketHandler.GetTag()
						+ "Inputstream is null",
						WifiDirectManager.DEBUGGER_CHANNEL);
			}
			else
			{
				is.close();
			}
		}
		catch (IOException e)
		{
			DebugManager.print(ServerSocketHandler.GetTag()
					+ "error while closing inputstream",
					WifiDirectManager.DEBUGGER_CHANNEL);
		}
	}

	/**
	 * Called when the message is a "IP" message.
	 * 
	 * @param stream
	 */
	private void receiveIP(InputStream stream)
	{
		// get the client's server ip and port in the string
		String clientIP = getStringFromInputStream(stream);

		DebugManager.print(ServerSocketHandler.GetTag()
				+ "client address is : " + clientIP + ". i'm the host."
				+ "onReceiveIpCallBack is null = "
				+ (onReceiveIPCallBack == null),
				WifiDirectManager.DEBUGGER_CHANNEL);

		// notify SocketHandler that this device client should
		// connect to the ip and port previously got.
		master.setRemoteHost(clientIP.split("!")[0],
				Integer.parseInt(clientIP.split("!")[1]));
		master.send();
		// send an accuse message with the client newly created and
		// connected
		// to the remost server.

		if (onReceiveIPCallBack != null)
		{
			onReceiveIPCallBack.Do();
		}
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "STRING" message.
	 * 
	 * @param stream
	 */
	private void receiveString(InputStream stream)
	{
		// get the string into the message
		String res = getStringFromInputStream(stream);
		DebugManager.print(ServerSocketHandler.GetTag()
				+ "we receive string : " + res,
				WifiDirectManager.DEBUGGER_CHANNEL);
		// call the callback if set
		if (onReceiveString != null)
			onReceiveString.Do(res);
		// send accuse to client
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "ACCUSE" message.
	 * 
	 * @param stream
	 */
	private void receiveAccuse()
	{
		DebugManager.print(ServerSocketHandler.GetTag() + "Accuse received !",
				WifiDirectManager.DEBUGGER_CHANNEL);
		master.sendNext();
	}

	/**
	 * Called when the message is a "BOOL" message.
	 * 
	 * @param stream
	 */
	private void receiveBool(InputStream is)
	{
		// read a bool from the stream
		boolean res = read(is) == 0 ? false : true;
		DebugManager.print(ServerSocketHandler.GetTag() + "we received bool: "
				+ res, WifiDirectManager.DEBUGGER_CHANNEL);
		// call the appropriate callback if set
		if (onReceiveBool != null)
			onReceiveBool.Do(res);
		// send accuse to client
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "INT" message.
	 * 
	 * @param stream
	 */
	private void receiveInt(InputStream is)
	{
		// get an int from the stream
		int res = read(is);
		DebugManager.print(ServerSocketHandler.GetTag() + "we received int: "
				+ res, WifiDirectManager.DEBUGGER_CHANNEL);
		// call the appropriate callback if set
		if (onReceiveInt != null)
			onReceiveInt.Do(res);
		// send accuse to client
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "BYTE" message.
	 * 
	 * @param stream
	 */
	private void receiveByte(InputStream is)
	{
		byte res = (byte) read(is);
		DebugManager.print(ServerSocketHandler.GetTag() + "we received byte: "
				+ res, WifiDirectManager.DEBUGGER_CHANNEL);
		if (onReceiveByte != null)
			onReceiveByte.Do(res);
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "CHAR" message.
	 * 
	 * @param stream
	 */
	private void receiveChar(InputStream is)
	{
		char res = (char) read(is);
		DebugManager.print(ServerSocketHandler.GetTag() + "we received char: "
				+ res, WifiDirectManager.DEBUGGER_CHANNEL);
		if (onReceiveChar != null)
			onReceiveChar.Do(res);
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "FLOAT" message.
	 * 
	 * @param stream
	 */
	private void receiveFloat(InputStream is)
	{
		float res = getFloatFromInputStream(is);
		DebugManager.print(ServerSocketHandler.GetTag() + "we received float: "
				+ res, WifiDirectManager.DEBUGGER_CHANNEL);
		if (onReceiveFloat != null)
			onReceiveFloat.Do(res);
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "DOUBLE" message.
	 * 
	 * @param stream
	 */
	private void receiveDouble(InputStream is)
	{
		double res = getDoubleFromInputStream(is);
		DebugManager.print(ServerSocketHandler.GetTag()
				+ "we received double: " + res,
				WifiDirectManager.DEBUGGER_CHANNEL);
		if (onReceiveDouble != null)
			onReceiveDouble.Do(res);
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "LONG" message.
	 * 
	 * @param stream
	 */
	private void receiveLong(InputStream is)
	{
		long res = getLongFromInputStream(is);
		DebugManager.print(ServerSocketHandler.GetTag() + "we received long: "
				+ res, WifiDirectManager.DEBUGGER_CHANNEL);
		if (onReceiveLong != null)
			onReceiveLong.Do(res);
		master.sendAccuse(id);
	}

	/**
	 * Called if the message contains an array of byte
	 * 
	 * @param stream
	 */
	private void receiveBytes(InputStream is)
	{
		byte[] res = getBytesFromInputStream(is);
		/* debug */
		String bytesStr = "";
		for (int i = 0; i < res.length; i++)
		{
			bytesStr += res[i] + "-";
		}

		DebugManager.print(ServerSocketHandler.GetTag() + "we received bytes: "
				+ bytesStr, WifiDirectManager.DEBUGGER_CHANNEL);
		/* end debug */
		if (onReceiveByteArray != null)
			onReceiveByteArray.Do(res);
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "FILE" message.
	 * 
	 * @param stream
	 */
	private void receiveFile(InputStream is)
	{
		File res = getFileFromInputStream(is);
		/* debug */
		DebugManager.print(ServerSocketHandler.GetTag() + "new file received "
				+ res, WifiDirectManager.DEBUGGER_CHANNEL);
		/* end debug */
		if (onReceiveFile != null)
			onReceiveFile.Do(res);
		master.sendAccuse(id);
	}

	/**
	 * 
	 * @param is
	 * @return an int read from the inpustream 'is'
	 */
	private int read(InputStream is)
	{
		if (is != null)
		{
			try
			{
				return is.read();
			}
			catch (IOException e)
			{
				DebugManager.print(
						ServerSocketHandler.GetTag()
								+ "error when reading inputstream "
								+ e.getLocalizedMessage(),
						WifiDirectManager.DEBUGGER_CHANNEL);
				return -1;
			}
		}
		return -1;
	}

	private PACKET_TYPE getPacketType(InputStream is)
	{
		int value = -1;
		int available = -1;
		try
		{
			available = is.available();
			value = read(is);
			return PACKET_TYPE.get(value);
		}
		catch (IOException e)
		{
			DebugManager.print(ServerSocketHandler.GetTag()
					+ "error while getting packet type " + available,
					WifiDirectManager.DEBUGGER_CHANNEL);
			return PACKET_TYPE.DEFAULT;
		}
		catch (Exception e)
		{
			DebugManager.print(ServerSocketHandler.GetTag()
					+ "unknow exception : " + value,
					WifiDirectManager.DEBUGGER_CHANNEL);
			return PACKET_TYPE.DEFAULT;
		}
	}

	private static ArrayList<Long> alreadyTreated = new ArrayList<Long>();

	long id;
	private boolean isMessageAlreadyReceived(InputStream is)
	{
		id = getLongFromInputStream(is);
		DebugManager.print(ServerSocketHandler.GetTag() + "message id = " + id,
				WifiDirectManager.DEBUGGER_CHANNEL);
		if (alreadyTreated.contains(id))
		{
			return true;
		}
		else
		{
			alreadyTreated.add(id);
			return false;
		}
	}

	@Override
	public void run()
	{
		InputStream is = getClientData(client);

		PACKET_TYPE type = getPacketType(is);

		if(type == PACKET_TYPE.ACCUSE)//Accuse always come concatenated with other message
		{
			if(!isMessageAlreadyReceived(is))
				receiveAccuse();
			type = getPacketType(is);
		}
		
		boolean msgAlreadyReceived = isMessageAlreadyReceived(is);
		
		DebugManager.print(ServerSocketHandler.GetTag()
				+ "We received a packet of type " + type + ". was already received ? " + msgAlreadyReceived,
				WifiDirectManager.DEBUGGER_CHANNEL);
		

		
		if (!msgAlreadyReceived)
		{

			switch (type)
			{
				case KEEP_ALIVE://KEEP_ALIVE is aynchrone accuse
					DebugManager.print(ServerSocketHandler.GetTag()
							+ "Client is alive !",
							WifiDirectManager.DEBUGGER_CHANNEL);
					receiveAccuse();
					break;
				case DEFAULT:
					break;
				case BOOL:
					receiveBool(is);
					break;
				case BYTE:
					receiveByte(is);
					break;
				case BYTE_ARRAY:
					receiveBytes(is);
					break;
				case CHAR:
					receiveChar(is);
					break;
				case DOUBLE:
					receiveDouble(is);
					break;
				case FILE:
					receiveFile(is);
					break;
				case FLOAT:
					receiveFloat(is);
					break;
				case INT:
					receiveInt(is);
					break;
				case IP:
					receiveIP(is);
					break;
				case LONG:
					receiveLong(is);
					break;
				case STRING:
					receiveString(is);
					break;
				case ACCUSE:
//					receiveAccuse(); // already treated
					break;
				default:
					break;
			}
		}
		else
		{
			if (type != PACKET_TYPE.KEEP_ALIVE)
				master.sendAccuse(id);//remote pair should not have receive accuse, so it keeps sending it -> so we send the accuse again
		}

		closeInputStream(is);

		DebugManager.print(
				ServerSocketHandler.GetTag() + "Request is answered",
				WifiDirectManager.DEBUGGER_CHANNEL);
	}

	private static CallBackMethod onReceiveIPCallBack = null;

	public static void setOnReceiveIPCallBack(CallBackMethod cm)
	{
		onReceiveIPCallBack = cm;
	}

	private static CallBackMethod onReceiveString = null;
	private static CallBackMethod onReceiveInt = null;
	private static CallBackMethod onReceiveBool = null;
	private static CallBackMethod onReceiveFloat = null;
	private static CallBackMethod onReceiveDouble = null;
	private static CallBackMethod onReceiveByte = null;
	private static CallBackMethod onReceiveLong = null;
	private static CallBackMethod onReceiveFile = null;
	private static CallBackMethod onReceiveByteArray = null;
	private static CallBackMethod onReceiveChar = null;

	public static void registerCallBackReceiver(CallBackMethod onReceiveString,
			CallBackMethod onReceiveInt, CallBackMethod onReceiveBool,
			CallBackMethod onReceiveFloat, CallBackMethod onReceiveDouble,
			CallBackMethod onReceiveByte, CallBackMethod onReceiveLong,
			CallBackMethod onReceiveFile, CallBackMethod onReceiveByteArray,
			CallBackMethod onReceiveChar)
	{
		Communication.onReceiveString = onReceiveString;
		Communication.onReceiveInt = onReceiveInt;
		Communication.onReceiveBool = onReceiveBool;
		Communication.onReceiveFloat = onReceiveFloat;
		Communication.onReceiveDouble = onReceiveDouble;
		Communication.onReceiveByte = onReceiveByte;
		Communication.onReceiveLong = onReceiveLong;
		Communication.onReceiveFile = onReceiveFile;
		Communication.onReceiveByteArray = onReceiveByteArray;
		Communication.onReceiveChar = onReceiveChar;
	}

}

/**
 * This class define a server socket for android, that can handle message as
 * int, float, long, file, ... All message type are declared in the PACKET_TYPE
 * interface.
 * 
 * @see PACKET_TYPE
 * 
 * @author Gregoire
 * 
 */
public class ServerSocketHandler extends AsyncTask<Void, String, Void> {

	private int port;
	private SocketHandler master;

	public static String GetTag()
	{
		Calendar cal = Calendar.getInstance();
		SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
		return "[LUDOSERVER][" + sdf.format(cal.getTime()) + "]";
	}

	public ServerSocketHandler(int port, SocketHandler master)
	{
		this.port = port;
		this.master = master;

	}

	private boolean run = true;

	public void stop()
	{
		run = false;
		this.cancel(true);
	}

	public void listen()
	{
		execute();
	}

	public String getServerIpAddress()
	{
		return SocketHandler.getThisDeviceIpAddress();
	}

	private void openServerSocket()
	{
		/**
		 * Create a server socket and wait for client connections. This call
		 * blocks until a connection is accepted from a client
		 */

		try
		{
			serverSocket = new ServerSocket(port);
		}
		catch (Exception e)
		{
			try
			{
				serverSocket = new ServerSocket(0);
			}
			catch (IOException e1)
			{
				DebugManager.print(ServerSocketHandler.GetTag()
						+ "Error when openning server socket",
						WifiDirectManager.DEBUGGER_CHANNEL);
			}
		}

		String debugText = "launching server on " + getServerIpAddress() + ":";
		if (port != serverSocket.getLocalPort())
		{
			DebugManager.print(
					ServerSocketHandler.GetTag() + "Port " + port
							+ " was already in use. " + debugText
							+ serverSocket.getLocalPort(),
					WifiDirectManager.DEBUGGER_CHANNEL);
			port = serverSocket.getLocalPort();
		}
		else
		{
			DebugManager.print(ServerSocketHandler.GetTag() + debugText + port,
					WifiDirectManager.DEBUGGER_CHANNEL);
		}

	}

	private Socket waitForClient()
	{
		Socket client = null;

		try
		{
			client = serverSocket.accept();
		}
		catch (IOException e1)
		{
			DebugManager.print(ServerSocketHandler.GetTag()
					+ "server.accept() failed",
					WifiDirectManager.DEBUGGER_CHANNEL);
		}
		DebugManager.print(ServerSocketHandler.GetTag()
				+ "new client connected", WifiDirectManager.DEBUGGER_CHANNEL);

		run = true;

		return client;
	}

	private void closeServerSocket()
	{
		try
		{
			serverSocket.close();
		}
		catch (IOException e)
		{
			DebugManager.print(ServerSocketHandler.GetTag()
					+ "server.close() failed",
					WifiDirectManager.DEBUGGER_CHANNEL);
		}
		DebugManager.print(ServerSocketHandler.GetTag() + "server is closed",
				WifiDirectManager.DEBUGGER_CHANNEL);
	}

	public boolean isRunning()
	{
		return run;
	}

	ServerSocket serverSocket;

	@Override
	protected Void doInBackground(Void... params)
	{
		Thread.currentThread().setName("LudoMuseThread");
		openServerSocket();

		while (isRunning())
		{
			DebugManager.print(ServerSocketHandler.GetTag()
					+ "server is waiting for client...",
					WifiDirectManager.DEBUGGER_CHANNEL);
			Socket client = waitForClient();
			DebugManager.print(ServerSocketHandler.GetTag()
					+ "new client request...",
					WifiDirectManager.DEBUGGER_CHANNEL);
			LudoMuseThread.start(new Communication(client, master));
			//LudoMuseThread.stopAllLudoMuseThread();
		}

		closeServerSocket();

		return null;
	}

	public void setOnReceiveIPCallBack(CallBackMethod cm)
	{
		Communication.setOnReceiveIPCallBack(cm);
	}

	public static void registerCallBackReceiver(CallBackMethod onReceiveString,
			CallBackMethod onReceiveInt, CallBackMethod onReceiveBool,
			CallBackMethod onReceiveFloat, CallBackMethod onReceiveDouble,
			CallBackMethod onReceiveByte, CallBackMethod onReceiveLong,
			CallBackMethod onReceiveFile, CallBackMethod onReceiveByteArray,
			CallBackMethod onReceiveChar)
	{
		Communication
				.registerCallBackReceiver(onReceiveString, onReceiveInt,
						onReceiveBool, onReceiveFloat, onReceiveDouble,
						onReceiveByte, onReceiveLong, onReceiveFile,
						onReceiveByteArray, onReceiveChar);
	}

	public void stopHandlers()
	{
		// TODO Auto-generated method stub

	}

}