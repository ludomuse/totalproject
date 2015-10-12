#include "../Includes/WifiObserver.h"
#include "../Includes/WifiDirectFacade.h"
#include "../Includes/LmJniCppFacade.h"
#include "../Includes/LmJniJavaFacade.h"

using namespace std;

WifiDirectFacade::WifiDirectFacade()
{
	LmJniCppFacade::setWifiFacade(this);
}

void WifiDirectFacade::onGettingPeers(vector<string> peers)
{
	list<WifiObserver*>::const_iterator lit(_observers.begin());
	list<WifiObserver*>::const_iterator lend(_observers.end());
	for (; lit != lend; ++lit)
	{
		WifiObserver* tps = (*lit);
		tps->onGettingPeers(peers);
	}
}

void WifiDirectFacade::onReceiving(string s)
{
	list<WifiObserver*>::const_iterator lit(_observers.begin());
	list<WifiObserver*>::const_iterator lend(_observers.end());
	for (; lit != lend; ++lit)
	{
		WifiObserver* tps = (*lit);
		tps->onReceiving(s);
	}
}

void WifiDirectFacade::onReceiving(int i)
{
	list<WifiObserver*>::const_iterator lit(_observers.begin());
	list<WifiObserver*>::const_iterator lend(_observers.end());
	for (; lit != lend; ++lit)
	{
		WifiObserver* tps = (*lit);
		tps->onReceiving(i);
	}
}

void WifiDirectFacade::onReceiving(bool b)
{
	list<WifiObserver*>::const_iterator lit(_observers.begin());
	list<WifiObserver*>::const_iterator lend(_observers.end());
	for (; lit != lend; ++lit)
	{
		WifiObserver* tps = (*lit);
		tps->onReceiving(b);
	}
}

void WifiDirectFacade::onReceiving(long l)
{
	list<WifiObserver*>::const_iterator lit(_observers.begin());
	list<WifiObserver*>::const_iterator lend(_observers.end());
	for (; lit != lend; ++lit)
	{
		WifiObserver* tps = (*lit);
		tps->onReceiving(l);
	}
}

void WifiDirectFacade::onReceivingFile(string path)
{
	list<WifiObserver*>::const_iterator lit(_observers.begin());
	list<WifiObserver*>::const_iterator lend(_observers.end());
	for (; lit != lend; ++lit)
	{
		WifiObserver* tps = (*lit);
		tps->onReceivingFile(path);
	}
}

void WifiDirectFacade::onReceiving(double d)
{
	list<WifiObserver*>::const_iterator lit(_observers.begin());
	list<WifiObserver*>::const_iterator lend(_observers.end());
	for (; lit != lend; ++lit)
	{
		WifiObserver* tps = (*lit);
		tps->onReceiving(d);
	}
}

void WifiDirectFacade::onReceiving(float f)
{
	list<WifiObserver*>::const_iterator lit(_observers.begin());
	list<WifiObserver*>::const_iterator lend(_observers.end());
	for (; lit != lend; ++lit)
	{
		WifiObserver* tps = (*lit);
		tps->onReceiving(f);
	}
}

void WifiDirectFacade::onReceiving(char c)
{
	list<WifiObserver*>::const_iterator lit(_observers.begin());
	list<WifiObserver*>::const_iterator lend(_observers.end());
	for (; lit != lend; ++lit)
	{
		WifiObserver* tps = (*lit);
		tps->onReceiving(c);
	}
}

void WifiDirectFacade::onReceivingByte(byte b)
{
	list<WifiObserver*>::const_iterator lit(_observers.begin());
	list<WifiObserver*>::const_iterator lend(_observers.end());
	for (; lit != lend; ++lit)
	{
		WifiObserver* tps = (*lit);
		tps->onReceivingByte(b);
	}
}

void WifiDirectFacade::onReceiving(bytes byteArray)
{
	list<WifiObserver*>::const_iterator lit(_observers.begin());
	list<WifiObserver*>::const_iterator lend(_observers.end());
	for (; lit != lend; ++lit)
	{
		WifiObserver* tps = (*lit);
		tps->onReceiving(byteArray);
	}
}

void WifiDirectFacade::onReceivingAccuse()
{
	if (_isSendingGroup)
	{
		sendNextInGroup();
		return;
	}
	list<WifiObserver*>::const_iterator lit(_observers.begin());
	list<WifiObserver*>::const_iterator lend(_observers.end());
	for (; lit != lend; ++lit)
	{
		WifiObserver* tps = (*lit);
		tps->onReceivingAccuse();
	}
}

void WifiDirectFacade::discoverPeers()
{

	LmJniJavaFacade::discoverPeers();
}

void WifiDirectFacade::connectTo(string deviceName)
{
	LmJniJavaFacade::connectTo(deviceName);
}

void WifiDirectFacade::send(string s)
{
	LmJniJavaFacade::send(s);
}

void WifiDirectFacade::send(int i)
{
	LmJniJavaFacade::send(i);
}

void WifiDirectFacade::send(bool b)
{
	LmJniJavaFacade::send(b);
}

void WifiDirectFacade::send(long l)
{
	LmJniJavaFacade::send(l);
}

void WifiDirectFacade::sendFile(string filePath)
{
	LmJniJavaFacade::sendFile(filePath);
}

void WifiDirectFacade::send(double d)
{
	LmJniJavaFacade::send(d);
}

void WifiDirectFacade::send(float f)
{
	LmJniJavaFacade::send(f);
}

void WifiDirectFacade::send(char c)
{
	LmJniJavaFacade::send(c);
}

void WifiDirectFacade::sendByte(byte b)
{
	LmJniJavaFacade::sendByte(b);
}

void WifiDirectFacade::sendBytes(bytes bytes)
{
	LmJniJavaFacade::sendBytes(bytes);
}

int WifiDirectFacade::addObserver(WifiObserver* wo)
{
	_observers.push_back(wo);
	return _observers.size() - 1;
}

void WifiDirectFacade::removeObserver(int index)
{
	list<WifiObserver*>::iterator it = _observers.begin();
	advance(it, index);
	_observers.erase(it);
}

void WifiDirectFacade::sendNextInGroup()
{
	SEND_F function = _groupMethod[_currentGroupIndex];
	void* param = _params[_currentGroupIndex];
	_currentGroupIndex++;
	if (_currentGroupIndex >= _groupSize)
	{
		_isSendingGroup = false;
	}
	switch (function)
	{
	case SEND_INT:
		send(PTR_TO_OBJ(param, int));
		break;
		case SEND_FLOAT:
		send(PTR_TO_OBJ(param, float));
		break;
		case SEND_DOUBLE:
		send(PTR_TO_OBJ(param, int));
		break;
		case SEND_FILE:
		sendFile(PTR_TO_OBJ(param, std::string));
		break;
		case SEND_CHAR:
		send(PTR_TO_OBJ(param, char));
		break;
		case SEND_BYTE:
		sendByte(PTR_TO_OBJ(param, byte));
		break;
		case SEND_LONG:
		send(PTR_TO_OBJ(param, long));
		break;
		case SEND_BYTES:
		sendBytes(PTR_TO_OBJ(param, bytes));
		break;
	}

}

void WifiDirectFacade::group(int size, SEND_F* send_functions, void** params)
{
	_isSendingGroup = true;
	_groupSize = size;
	_currentGroupIndex = 0;
	_groupMethod = send_functions;
	_params = params;
	sendNextInGroup();
}

void WifiDirectFacade::sendEvent(event e, WifiDirectFacade::SEND_F method,
		void* arg)
{
	WifiDirectFacade::SEND_F* functions = new WifiDirectFacade::SEND_F[2]
	{ WifiDirectFacade::SEND_F::SEND_BYTE, method };
	void** params = new void*[2]
	{ &e, arg };
	group(2, functions, params);
}
