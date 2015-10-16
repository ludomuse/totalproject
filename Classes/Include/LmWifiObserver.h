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

class LmWifiObserver {

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

		virtual void onReceivingEvent()
		{
		}
		;

		virtual void onReceiving(bytes byteArray)
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
