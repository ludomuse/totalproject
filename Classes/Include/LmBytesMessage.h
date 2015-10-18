/*
 * BYTESMESSAGE_H_.h
 *
 *  Created on: 7 oct. 2015
 *      Author: Gregoire, IHMTEK
 */

#ifndef BYTESMESSAGE_H_
#define BYTESMESSAGE_H_

#include <stdio.h>
#include <string>
#include <sstream>
#include "cocos2d.h"
#include <vector>

typedef char byte;

class bytes;
class LmSerializable {
	public:
		virtual void writeOn(bytes* msg) = 0;
		virtual void readOn(bytes* msg) = 0;
};

class bytes {

	private:
		byte* data;
		int writeCursor;
		int readCursor;
		int size;

		void allocate(int size)
		{
			if (this->size >= size || size == 0)
				return;
			this->size = size;
			byte* newBytes = new byte[size];
			if (data != 0)
				memcpy(newBytes, data, getLen());
			data = newBytes;
		}

		void increase(int size)
		{
			allocate(getLen() + size);
		}

		void init()
		{
			data = 0;
			writeCursor = 0;
			readCursor = 0;
			size = 0;
		}

	public:

		bytes()
		{
			init();
		}

		bytes(int size)
		{
			init();
			allocate(size);
		}

		bytes(bytes* dataBytes)
		{
			init();
			if (dataBytes != 0)
				write(*dataBytes);
		}

		bytes(byte* dataBytes, int len)
		{
			init();
			write(dataBytes, len);
		}

		int getLen()
		{
			return writeCursor;
		}

		int getSize()
		{
			return size;
		}

		int getFreeCount()
		{
			return getSize() - getLen();
		}

		void write(bytes array)
		{
			write(array.data, array.getLen());
		}

		void write(const byte* str, int len)
		{
			if(len == 0)
				return;
			CCLOG("writting a byte array of size %d", len);
			increase(len);
			memcpy((void*) &(data[writeCursor]), str, len);
			writeCursor += len;
		}

		bytes& operator<<(bytes& value)
		{
			write(value);
			return *this;
		}

		byte* readBytes(int size)
		{
			byte* res = new byte[size];
			memcpy(res, (void*) &(data[readCursor]), size);
			readCursor += size;
			return res;
		}

		bytes& operator>>(bytes& value)
		{
			value.write(readBytes(value.getLen()), getLen());
			return *this;
		}

		void write(byte b)
		{
			increase(1);
			data[writeCursor] = b;
			writeCursor++;
		}

		bytes& operator<<(byte value)
		{
			CCLOG("writ byte");
			write(value);
			return *this;
		}

		byte readByte()
		{
			byte res = data[readCursor];
			readCursor++;
			return res;
		}

		bytes& operator>>(byte &value)
		{
			value = readByte();
			return *this;
		}

		void writeChar(char c)
		{
			write((byte) c);
		}

		char readChar()
		{
			return (char) readByte();
		}

		void write(bool b)
		{
			write(b ? (byte) 1 : (byte) 0);
		}

		bytes& operator<<(bool &value)
		{
			write(value);
			return *this;
		}

		bool readBool()
		{
			return readByte() == 0 ? false : true;
		}

		bytes& operator>>(bool& value)
		{
			value = readBool();
			return *this;
		}

		void write(const char* str)
		{
			write(std::string(str));
		}

		bytes& operator<<(const char* value)
		{
			write(value);
			return *this;
		}

		void write(std::string str)
		{
			CCLOG("writ string");
			int len = str.length();
			std::vector<char> vect(str.begin(), str.end());
			byte* data = &vect[0];
			CCLOG("writ string size");
			write(len);
			CCLOG("writ string content");
			write(data, len);
		}

		bytes& operator<<(std::string &value)
		{
			write(value);
			return *this;
		}

		std::string readString()
		{
			int len = readInt();
			CCLOG("size of string = %d", len);
			byte* array = readBytes(len);
			return std::string(array, len);
		}

		bytes& operator>>(std::string& value)
		{
			value = readString();
			return *this;
		}

		void write(int v)
		{
			CCLOG("writ int");
			byte* rep = new byte[sizeof(int)];
			memcpy(rep, &v, sizeof(int));
			write(rep, sizeof(int));
		}

		bytes& operator<<(int &value)
		{
			write(value);
			return *this;
		}

		int readInt()
		{
			byte* rep = readBytes(sizeof(int));
			int res = 0;
			memcpy(&res, rep, sizeof(int));
			return res;
		}

		bytes& operator>>(int& value)
		{
			value = readInt();
			return *this;
		}

		void write(long v)
		{
			byte* rep = new byte[sizeof(long)];
			memcpy(rep, &v, sizeof(long));
			write(rep, sizeof(long));
		}

		bytes& operator<<(long &value)
		{
			write(value);
			return *this;
		}

		long readLong()
		{
			byte* rep = readBytes(sizeof(long));
			long res = 0;
			memcpy(&res, rep, sizeof(long));
			return res;
		}

		bytes& operator>>(long& value)
		{
			value = readLong();
			return *this;
		}

		void write(double v)
		{
			byte* rep = new byte[sizeof(double)];
			memcpy(rep, &v, sizeof(double));
			write(rep, sizeof(double));
		}

		bytes& operator<<(double &value)
		{
			write(value);
			return *this;
		}

		double readDouble()
		{
			byte* rep = readBytes(sizeof(double));
			double res = 0;
			memcpy(&res, rep, sizeof(double));
			return res;
		}

		bytes& operator>>(double &value)
		{
			value = readDouble();
			return *this;
		}

		void write(float v)
		{
			byte* rep = new byte[sizeof(float)];
			memcpy(rep, &v, sizeof(float));
			write(rep, sizeof(float));
		}

		bytes& operator<<(float &value)
		{
			write(value);
			return *this;
		}

		float readFloat()
		{
			byte* rep = readBytes(sizeof(float));
			float res = 0;
			memcpy(&res, rep, sizeof(float));
			return res;
		}

		bytes& operator>>(float& value)
		{
			value = readFloat();
			return *this;
		}

		void write(LmSerializable* obj)
		{
			obj->writeOn(this);
		}

		bytes& operator<<(LmSerializable &value)
		{
			write(&value);
			return *this;
		}

		template<typename Serializable>
		Serializable* read()
		{
			Serializable* res = new Serializable();
			res->readOn(this);
			return res;
		}

		template<typename Serializable>
		bytes& operator>>(Serializable** value)
		{
			*value = read<Serializable>();
			return *this;
		}

		const byte* toByteArray()
		{
			return data;
		}

};

#endif /* BYTESMESSAGE_H_ */
