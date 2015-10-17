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

class LmBytesMessage {

	private:
		byte* data;
		int writeCursor;
		int readCursor;

		void allocate(int size)
		{

			byte* newBytes = new byte[size];
			if(data != 0)
				memcpy(newBytes, data, getLen());
			data = newBytes;
		}

		void increase(int size)
		{
			allocate(getLen() + size);
		}

	public:


		/*LmBytesMessage(int size)
		{
			LmBytesMessage();
			allocate(size);
		}*/

		LmBytesMessage(bytes* dataBytes = 0)
		{
			data = 0;
			writeCursor = 0;
			readCursor = 0;
			if(dataBytes != 0)
				write(*dataBytes);
		}

		int getLen()
		{
			return writeCursor;
		}

		void write(bytes array)
		{
			int arrayLen = array.len;
			increase(arrayLen);
			memcpy((void*) &(data[writeCursor]),array.data, arrayLen);
			writeCursor += arrayLen;
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

		void write(byte b)
		{
			CCLOG("writting byte %d", b);
			increase(1);
			data[writeCursor] = b;
			writeCursor++;
		}

		byte readByte()
		{
			byte res = data[readCursor];
			readCursor++;
			return res;
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

		bool readBool()
		{
			return readByte() == 0 ? false : true;
		}

		void write(const char* str)
		{
			write(std::string(str));
		}

		void write(std::string str)
		{
			CCLOG("Writting a string");
			bytes rep;
			rep.len = str.length();

			std::vector<char> vect(str.begin(), str.end());
			rep.data = &vect[0];

			CCLOG("Writting an int");
			write(rep.len);

			CCLOG("Writting a char*");
			write(rep);
		}

		std::string readString()
		{
			int len = readInt();
			CCLOG("string size is: %d", len);
			bytes array = readBytes(len);
			return std::string(array.data, len);
		}


		void write(int i)
		{
			bytes rep;
			rep.len = sizeof(int);
			rep.data = new byte[sizeof(int)];
			memcpy(rep.data, &i, sizeof(int));
			write(rep);
		}

		int readInt()
		{
			bytes rep = readBytes(sizeof(int));
			int res = 0;
			memcpy(&res, rep.data, sizeof(int));
			return res;
		}

		void write(long l)
		{
			bytes rep;
			rep.len = sizeof(long);
			rep.data = new byte[sizeof(long)];
			memcpy(rep.data, &l, sizeof(long));
			write(rep);
		}

		long readLong()
		{
			bytes rep = readBytes(sizeof(long));
			long res = 0;
			memcpy(&res, rep.data, sizeof(long));
			return res;
		}

		void write(double d)
		{
			bytes rep;
			rep.len = sizeof(double);
			rep.data = new byte[sizeof(double)];
			memcpy(rep.data, &d, sizeof(double));
			write(rep);
		}

		double readDouble()
		{
			bytes rep = readBytes(sizeof(double));
			double res = 0;
			memcpy(&res, rep.data, sizeof(double));
			return res;
		}

		void write(float v)
		{
			bytes rep;
			rep.len = sizeof(float);
			rep.data = new byte[sizeof(float)];
			memcpy(rep.data, &v, sizeof(float));
			write(rep);
		}

		float readFloat()
		{
			bytes rep = readBytes(sizeof(float));
			float res = 0;
			memcpy(&res, rep.data, sizeof(float));
			return res;
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

		void print()
		{
			std::string res = "";
			for(int i = 0; i < getLen(); i++)
			{
				res.append(1, data[i]);
				res.append(1, '-');
			}
			CCLOG("[BytesMessage] %s, len: %d", res.c_str(), getLen());
		}

};

#endif /* BYTESMESSAGE_H_ */
