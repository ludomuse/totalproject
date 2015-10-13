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
	_isSending = false;
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

bool WifiDirectFacade::send(string s)
{
	if (canSend())
	{
		LmJniJavaFacade::send(s);
		_isSending = true;
		return true;
	}
	return false;
}

bool WifiDirectFacade::send(int i)
{
	if (canSend())
	{
		LmJniJavaFacade::send(i);
		_isSending = true;
		return true;
	}
	return false;
}

bool WifiDirectFacade::send(bool b)
{
	if (canSend())
	{
		LmJniJavaFacade::send(b);
		_isSending = true;
		return true;
	}
	return false;
}

bool WifiDirectFacade::send(long l)
{
	if (canSend())
	{
		LmJniJavaFacade::send(l);
		_isSending = true;
		return true;
	}
	return false;
}

bool WifiDirectFacade::sendFile(string filePath)
{
	if (canSend())
	{
		LmJniJavaFacade::sendFile(filePath);
		_isSending = true;
		return true;
	}
	return false;
}

bool WifiDirectFacade::send(double d)
{
	if (canSend())
	{
		LmJniJavaFacade::send(d);
		_isSending = true;
		return true;
	}
	return false;
}

bool WifiDirectFacade::send(float f)
{
	if (canSend())
	{
		LmJniJavaFacade::send(f);
		_isSending = true;
		return true;
	}
	return false;
}

bool WifiDirectFacade::send(char c)
{
	if (canSend())
	{
		LmJniJavaFacade::send(c);
		_isSending = true;
		return true;
	}
	return false;
}

bool WifiDirectFacade::sendByte(byte b)
{
	if (canSend())
	{
		LmJniJavaFacade::sendByte(b);
		_isSending = true;
		return true;
	}
	return false;
}

bool WifiDirectFacade::sendBytes(bytes bytes)
{
	if (canSend())
	{
		LmJniJavaFacade::sendBytes(bytes);
		_isSending = true;
		return true;
	}
	return false;
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
	case SEND_STRING:
		send(PTR_TO_OBJ(param, std::string));
		break;
	}

}

bool WifiDirectFacade::group(int size, SEND_F* send_functions, void** params)
{
	if (canSend())
	{
		_isSendingGroup = true;
		_groupSize = size;
		_currentGroupIndex = 0;
		_groupMethod = send_functions;
		_params = params;
		sendNextInGroup();
		return true;
	}
	return false;
}

bool WifiDirectFacade::sendEvent(event e, WifiDirectFacade::SEND_F method,
		void* arg)
{
	WifiDirectFacade::SEND_F* functions = new WifiDirectFacade::SEND_F[2]
	{ WifiDirectFacade::SEND_F::SEND_BYTE, method };
	void** params = new void*[2]
	{ &e, arg };
	return group(2, functions, params);
}

bool WifiDirectFacade::sendEvent(event e, int size,
		const WifiDirectFacade::SEND_F* methods, const void** args)
{
	WifiDirectFacade::SEND_F* functions = new WifiDirectFacade::SEND_F[size + 1];
	functions[0] = WifiDirectFacade::SEND_F::SEND_BYTE;
	memcpy((void*) functions[1], (const void*) methods[0], size);

	void** params = new void*[size + 1];
	params[0] = &e;
	memcpy(params[1], args[0], size);
	return group(size + 1, functions, params);
}
