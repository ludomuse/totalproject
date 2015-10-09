/*
 * WifiObserver.h
 *
 *  Created on: 7 oct. 2015
 *      Author: Gregoire
 */

#ifndef WIFIOBSERVER_H_
#define WIFIOBSERVER_H_

#include <vector>
#include <string>
#include "../Includes/helpers.h"

class WifiObserver {

public:
	//attributs

	virtual void onGettingPeers(std::vector<std::string> peers){};

	virtual void onReceiving(std::string s){};

	virtual void onReceiving(int i){};

	virtual void onReceiving(bool b){};

	virtual void onReceiving(long l){};

	virtual void onReceivingFile(std::string path){};

	virtual void onReceiving(double d){};

	virtual void onReceiving(float f){};

	virtual void onReceiving(char c){};

	virtual void onReceivingByte(byte byte){};

	virtual void onReceiving(bytes byteArray){};

	virtual void discoverPeers(){};

	virtual ~WifiObserver()
	{

	}
};

#endif /* WIFIOBSERVER_H_ */
