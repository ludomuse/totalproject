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
#include <vector>

#define WR_PRIM(X) 		void write(X v)\
						{\
							byte* rep = new byte[sizeof(X)];\
							memcpy(rep, &v, sizeof(X));\
							write(rep, sizeof(X));\
						}

#define RD_PRIM(X, Y) 	X Y()\
						{\
							byte* rep = readBytes(sizeof(X));\
							X res = 0;\
							memcpy(&res, rep, sizeof(X));\
							return res;\
						}

#define OP_IN(X) 		bytes& operator<<(X &value)\
						{\
							write(value);\
							return *this;\
						}

#define OP_OUT(X, Y) 	bytes& operator>>(X& value)\
						{\
							value = Y;\
							return *this;\
						}

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
			if (len == 0)
				return;
			increase(len);
			memcpy((void*) &(data[writeCursor]), str, len);
			writeCursor += len;
		}

		void write(const char* str)
		{
			write(std::string(str));
		}

		void write(std::string str)
		{
			int len = str.length();
			std::vector<char> vect(str.begin(), str.end());
			byte* data = &vect[0];
			write(len);
			write(data, len);
		}

		WR_PRIM(int)

		WR_PRIM(long)

		WR_PRIM(double)

		WR_PRIM(float)

		void write(LmSerializable* obj)
		{
			obj->writeOn(this);
		}

		void writeChar(char c)
		{
			write((byte) c);
		}

		void write(byte b)
		{
			increase(1);
			data[writeCursor] = b;
			writeCursor++;
		}

		void write(bool b)
		{
			write(b ? (byte) 1 : (byte) 0);
		}

		byte* readBytes(int size)
		{
			byte* res = new byte[size];
			memcpy(res, (void*) &(data[readCursor]), size);
			readCursor += size;
			return res;
		}

		byte readByte()
		{
			byte res = data[readCursor];
			readCursor++;
			return res;
		}

		char readChar()
		{
			return (char) readByte();
		}

		bool readBool()
		{
			return readByte() == 0 ? false : true;
		}

		std::string readString()
		{
			int len = readInt();
			byte* array = readBytes(len);
			return std::string(array, len);
		}

		RD_PRIM(int, readInt)

		RD_PRIM(long, readLong)

		RD_PRIM(double, readDouble)

		RD_PRIM(float, readFloat)

		template<typename Serializable>
		Serializable* read()
		{
			Serializable* res = new Serializable();
			res->readOn(this);
			return res;
		}

		const byte* toByteArray()
		{
			return data;
		}

		OP_IN(long)

		OP_IN(int)

		OP_IN(std::string)

		OP_IN(bytes)

		OP_IN(float)

		OP_IN(double)

		OP_IN(bool)

		bytes& operator<<(byte value)
		{
			write(value);
			return *this;
		}

		bytes& operator<<(const char* value)
		{
			write(value);
			return *this;
		}

		bytes& operator<<(LmSerializable &value)
		{
			write(&value);
			return *this;
		}

		OP_OUT(int, readInt())

		OP_OUT(std::string, readString())

		OP_OUT(bool, readBool())

		OP_OUT(byte, readByte())

		OP_OUT(long, readLong())

		OP_OUT(double, readDouble())

		OP_OUT(float, readFloat())

		bytes& operator>>(bytes& value)
		{
			value.write(readBytes(value.getLen()), getLen());
			return *this;
		}

		template<typename Serializable>
		bytes& operator>>(Serializable** value)
		{
			*value = read<Serializable>();
			return *this;
		}

		/*~bytes()
		{
			delete [] data;
		}*/

};

#endif /* BYTESMESSAGE_H_ */