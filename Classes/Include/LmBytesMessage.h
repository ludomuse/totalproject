/*
 * BYTESMESSAGE_H_.h
 *
 *  Created on: 7 oct. 2015
 *      Author: Gregoire, IHMTEK
 */

#ifndef BYTESMESSAGE_H_
#define BYTESMESSAGE_H_

#include "LmHelpers.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include "cocos2d.h"
#include <vector>

class LmBytesMessage;
class LmSerializable
{
	public:
		virtual void writeOn(LmBytesMessage* msg) = 0;
		virtual void readOn(LmBytesMessage* msg) = 0;
};

class LmBytesMessage {

	private:
		byte* data;
		int writeCursor;
		int readCursor;
		int size;

		void allocate(int size)
		{
			if(this->size >= size || size == 0)
				return;
			this->size = size;
			byte* newBytes = new byte[size];
			if(data != 0)
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


		LmBytesMessage()
		{
			init();
		}

		LmBytesMessage(int size)
		{
			init();
			allocate(size);
		}

		LmBytesMessage(bytes* dataBytes)
		{
			init();
			if(dataBytes != 0)
				write(*dataBytes);
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
			int arrayLen = array.len;
			increase(arrayLen);
			memcpy((void*) &(data[writeCursor]),array.data, arrayLen);
			writeCursor += arrayLen;
		}

		LmBytesMessage& operator<< (bytes& value)
		{
		    write(value);
		    return *this;
		}

		bytes readBytes(int size)
		{
			bytes res;
			res.len = size;
			res.data = new byte[size];
			memcpy(res.data, (void*) &(data[readCursor]), size);
			readCursor += size;
			return res;
		}

		LmBytesMessage& operator>> (bytes& value)
		{
		    value = readBytes(value.len);
		    return *this;
		}

		void write(byte b)
		{
			increase(1);
			data[writeCursor] = b;
			writeCursor++;
		}

		LmBytesMessage& operator<< (byte value)
		{
		    write(value);
		    return *this;
		}


		byte readByte()
		{
			byte res = data[readCursor];
			readCursor++;
			return res;
		}

		LmBytesMessage& operator>> (byte &value)
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

		LmBytesMessage& operator<< (bool &value)
		{
		    write(value);
		    return *this;
		}


		bool readBool()
		{
			return readByte() == 0 ? false : true;
		}

		LmBytesMessage& operator>> (bool& value)
		{
		    value = readBool();
		    return *this;
		}

		void write(const char* str)
		{
			write(std::string(str));
		}

		LmBytesMessage& operator<< (const char* value)
		{
		    write(value);
		    return *this;
		}


		void write(std::string str)
		{
			bytes rep;
			rep.len = str.length();
			std::vector<char> vect(str.begin(), str.end());
			rep.data = &vect[0];
			write(rep.len);
			write(rep);
		}

		LmBytesMessage& operator<< (std::string &value)
		{
		    write(value);
		    return *this;
		}


		std::string readString()
		{
			int len = readInt();
			bytes array = readBytes(len);
			return std::string(array.data, len);
		}

		LmBytesMessage& operator>> (std::string& value)
		{
		    value = readString();
		    return *this;
		}

		void write(int i)
		{
			bytes rep;
			rep.len = sizeof(int);
			rep.data = new byte[sizeof(int)];
			memcpy(rep.data, &i, sizeof(int));
			write(rep);
		}

		LmBytesMessage& operator<< (int &value)
		{
		    write(value);
		    return *this;
		}


		int readInt()
		{
			bytes rep = readBytes(sizeof(int));
			int res = 0;
			memcpy(&res, rep.data, sizeof(int));
			return res;
		}

		LmBytesMessage& operator>> (int& value)
		{
		    value = readInt();
		    return *this;
		}

		void write(long l)
		{
			bytes rep;
			rep.len = sizeof(long);
			rep.data = new byte[sizeof(long)];
			memcpy(rep.data, &l, sizeof(long));
			write(rep);
		}

		LmBytesMessage& operator<< (long &value)
		{
		    write(value);
		    return *this;
		}


		long readLong()
		{
			bytes rep = readBytes(sizeof(long));
			long res = 0;
			memcpy(&res, rep.data, sizeof(long));
			return res;
		}

		LmBytesMessage& operator>> (long& value)
		{
		    value = readLong();
		    return *this;
		}

		void write(double d)
		{
			bytes rep;
			rep.len = sizeof(double);
			rep.data = new byte[sizeof(double)];
			memcpy(rep.data, &d, sizeof(double));
			write(rep);
		}

		LmBytesMessage& operator<< (double &value)
		{
		    write(value);
		    return *this;
		}


		double readDouble()
		{
			bytes rep = readBytes(sizeof(double));
			double res = 0;
			memcpy(&res, rep.data, sizeof(double));
			return res;
		}

		LmBytesMessage& operator>> (double &value)
		{
		    value = readDouble();
		    return *this;
		}

		void write(float v)
		{
			bytes rep;
			rep.len = sizeof(float);
			rep.data = new byte[sizeof(float)];
			memcpy(rep.data, &v, sizeof(float));
			write(rep);
		}

		LmBytesMessage& operator<< (float &value)
		{
		    write(value);
		    return *this;
		}


		float readFloat()
		{
			bytes rep = readBytes(sizeof(float));
			float res = 0;
			memcpy(&res, rep.data, sizeof(float));
			return res;
		}

		LmBytesMessage& operator>> (float& value)
		{
		    value = readFloat();
		    return *this;
		}

		void write(LmSerializable* obj)
		{
			obj->writeOn(this);
		}

		LmBytesMessage& operator<< (LmSerializable &value)
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
		LmBytesMessage& operator>> (Serializable** value)
		{
		    *value = read<Serializable>();
		    return *this;
		}

		bytes toBytes()
		{
			bytes res;
			res.len = writeCursor;
			if(res.len > 0)
			{
				res.data = new byte[res.len];
				memcpy(res.data, data, res.len);
			}
			else
			{
				res.data = 0;
			}

			return res;
		}


};

#endif /* BYTESMESSAGE_H_ */
