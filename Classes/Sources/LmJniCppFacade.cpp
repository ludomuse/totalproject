#include "../Include/LmJniCppFacade.h"
#include "cocos2d.h"

LmWifiDirectFacade* LmJniCppFacade::_wifiDirectFacade = NULL;

LmWifiDirectFacade* LmJniCppFacade::getWifiFacade()
{
	return _wifiDirectFacade;
}

void LmJniCppFacade::setWifiFacade(LmWifiDirectFacade* wifiFacade)
{
	_wifiDirectFacade = wifiFacade;
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onGettingPeers(
		JNIEnv* env, jobject thiz, jstring peers)
{
	CCLOG("cpp method onGettingPeers is called from Java");
	WIFIFACADE->onGettingPeers(
			LmJniCppFacade::toSTDStringList(peers, env));
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingString(
		JNIEnv* env, jobject thiz, jstring s)
{
	WIFIFACADE->onReceiving(
			LmJniCppFacade::toCObject(s, env));
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingInt(
		JNIEnv* env, jobject thiz, jint i)
{
	WIFIFACADE->onReceiving(LmJniCppFacade::toCObject(i));
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingBool(
		JNIEnv* env, jobject thiz, jboolean b)
{
	WIFIFACADE->onReceiving(LmJniCppFacade::toCObject(b));
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingLong(
		JNIEnv* env, jobject thiz, jlong l)
{
	WIFIFACADE->onReceiving(LmJniCppFacade::toCObject(l));
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingFloat(
		JNIEnv* env, jobject thiz, jfloat f)
{
	WIFIFACADE->onReceiving(LmJniCppFacade::toCObject(f));
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingDouble(
		JNIEnv* env, jobject thiz, jdouble d)
{
	WIFIFACADE->onReceiving(LmJniCppFacade::toCObject(d));
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingBytes(
		JNIEnv* env, jobject thiz, jbyteArray bytes)
{
	WIFIFACADE->onReceiving(
			LmJniCppFacade::toCObject(bytes, env));
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingFile(
		JNIEnv* env, jobject thiz, jstring path)
{
	WIFIFACADE->onReceivingFile(
			LmJniCppFacade::toCObject(path, env));
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingByte(
		JNIEnv* env, jobject thiz, jbyte byte)
{
	WIFIFACADE->onReceivingByte(
			LmJniCppFacade::toCObject(byte));
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingChar(
		JNIEnv* env, jobject thiz, jchar c)
{
	WIFIFACADE->onReceiving(LmJniCppFacade::toCObject(c));
}

