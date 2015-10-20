/*
 * LmWifiObserver.h
 *
 *  Created on: 7 oct. 2015
 *      Author: Gregoire
 */

#ifndef WIFIOBSERVER_H_
#define WIFIOBSERVER_H_

#include <vector>
#include <string>
#include "../Include/LmHelpers.h"
#include "LmWifiDirectFacade.h"
#include "LmJniCppFacade.h"
#include "LmBytesMessage.h"

class LmWifiObserver {

private:
	int _observerIndex;

	bytes lastMsg;

	protected:
	event _event;
		LmWifiDirectFacade* _wifiFacade;

	public:
		//attributs

		LmWifiObserver()
		{
			_wifiFacade = WIFIFACADE;
		}


		virtual void onGettingPeers(std::vector<std::string> peers)
		{
		}
		;

		virtual void onReceiving(std::string s)
		{
		}
		;

		virtual void onReceiving(int i)
		{
		}
		;

		virtual void onReceiving(bool b)
		{
		}
		;

		virtual void onReceiving(long l)
		{
		}
		;

		virtual void onReceivingFile(std::string path)
		{
		}
		;

		virtual void onReceiving(double d)
		{
		}
		;

		virtual void onReceiving(float f)
		{
		}
		;

		virtual void onReceiving(char c)
		{
		}
		;

		void onReceivingByte(event byte)
		{
			_event = byte;
			onReceivingEvent();
		}

		void listenWifiFacade()
		{
			_observerIndex = _wifiFacade->addObserver(this);
		}

		void stopListenWifiFacade()
		{
			_wifiFacade->removeObserver(_observerIndex);
		}

		virtual void onReceivingEvent()
		{
		}
		;

		void clearLastMsg()
		{
			lastMsg = bytes();
		}

		void onReceiving(bytes msg)
		{
			lastMsg.rewind();
			event lastEvent = lastMsg.readByte();
			event currEvent = msg.readByte();
			CCLOG("(lastMsg = %d, msg = %d).", lastEvent, currEvent);
			msg.rewind();

			if(lastMsg == msg /*|| msg.getLen() == 0*/)
			{
				CCLOG("msg received two times or msg null");
				return;
			}
			lastMsg = msg;
			onReceivingByte(msg.readByte());
			onReceivingMsg(msg);
		}
		;

		virtual void onReceivingMsg(bytes msg)
		{

		}
		;


		virtual void discoverPeers()
		{
		}
		;

		virtual ~LmWifiObserver()
		{

		}

};

#endif /* WIFIOBSERVER_H_ */
