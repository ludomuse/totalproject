/*
 * WifiDirectFacade.h
 *
 *  Created on: 7 oct. 2015
 *      Author: Gregoire
 */

#ifndef WIFIDIRECTFACADE_H_
#define WIFIDIRECTFACADE_H_

#include <vector>
#include <list>
#include "../Includes/helpers.h"
#include <string>

class WifiObserver;

class WifiDirectFacade {
	public:

		enum SEND_F
		{
			SEND_INT,
			SEND_FLOAT,
			SEND_LONG,
			SEND_DOUBLE,
			SEND_FILE,
			SEND_CHAR,
			SEND_BYTE,
			SEND_BYTES,
			SEND_STRING
		};

		WifiDirectFacade();

		void onGettingPeers(std::vector<std::string> peers);

		void onReceiving(std::string s);

		void onReceiving(int i);

		void onReceiving(bool b);

		void onReceiving(long l);

		void onReceivingFile(std::string path);

		void onReceiving(double d);

		void onReceiving(float f);

		void onReceiving(char c);

		void onReceivingByte(byte b);

		void onReceiving(bytes byteArray);

		void discoverPeers();

		void connectTo(std::string deviceName);

		void send(std::string s);

		void send(int i);

		void send(bool b);

		void send(long l);

		void sendFile(std::string filePath);

		void send(double d);

		void send(float f);

		void send(char c);

		void sendByte(byte b);

		void sendBytes(bytes bytes);

		void group(int size, SEND_F* send_functions, void** params);

		int addObserver(WifiObserver* wo);

		void removeObserver(int index);

		void sendEvent(event e, WifiDirectFacade::SEND_F method, void* arg);

		void sendEvent(event e, int size, const WifiDirectFacade::SEND_F* method,
				const void** args);

	private:

		std::list<WifiObserver*> _observers;
};

#endif /* WIFIDIRECTFACADE_H_ */
