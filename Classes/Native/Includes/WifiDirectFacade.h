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
			SEND_BYTES
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

		void onReceivingAccuse();

		void discoverPeers();

		void connectTo(std::string deviceName);

		bool send(std::string s);

		bool send(int i);

		bool send(bool b);

		bool send(long l);

		bool sendFile(std::string filePath);

		bool send(double d);

		bool send(float f);

		bool send(char c);

		bool sendByte(byte b);

		bool sendBytes(bytes bytes);

		bool group(int size, SEND_F* send_functions, void** params);

		int addObserver(WifiObserver* wo);

		void removeObserver(int index);

		bool sendEvent(event e, WifiDirectFacade::SEND_F method, void* arg);

		bool sendEvent(event e, int size, const WifiDirectFacade::SEND_F* method,
				const void** args);

	private:

		inline bool canSend()
		{
			return !_isSendingGroup && !_isSending;
		}

		void sendNextInGroup();

		bool _isSendingGroup = false;
		int _groupSize = -1;
		int _currentGroupIndex = -1;
		SEND_F* _groupMethod;
		void** _params;

		bool _isSending = false;
		std::list<WifiObserver*> _observers;
};

#endif /* WIFIDIRECTFACADE_H_ */
