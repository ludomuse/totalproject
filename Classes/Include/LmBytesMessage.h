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

class LmBytesMessage {

	private:
		byte* data;
		int writeCursor;
		int readCursor;

		void allocate(int size)
		{
			int len = getLen();
			byte* newBytes = new byte[size];
			memcpy((void*) &newBytes[0], (void*) &data[0], size);
			data = newBytes;
		}

		void increase(int size)
		{
			allocate(getLen() + size);
		}

	public:

		LmBytesMessage(bytes dataBytes)
		{
			data = dataBytes.data;
			writeCursor = dataBytes.len;
			readCursor = 0;
		}

		LmBytesMessage()
		{
			data = new byte[0];
			writeCursor = 0;
			readCursor = 0;
		}

		int getLen()
		{
			return writeCursor;
		}

		void write(bytes array)
		{
			int arrayLen = array.len;
			increase(arrayLen);
			memcpy((void*) &data[writeCursor],(void*) &(array.data[0]), arrayLen);
			writeCursor += arrayLen;
		}

		byte* readBytes(int size)
		{
			byte* res = new byte[size];
			memcpy((void*)&res[0], (void*) &data[readCursor], size);
			readCursor += size;
			return res;
		}

		void write(byte b)
		{
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

		void write(std::string str)
		{
			int len = str.length();
			bytes res;
			res.len = len + 1;
			res.data = new byte[res.len];
			for (int i = 0; i < len; i++)
			{
				res.data[i] = (byte) str.at(i);
			}
			res.data[len] = (byte) '\0';
			write(res);
		}

		std::string readString()
		{
			std::string res = "";
			char tps;
			while ((tps = readChar()) != '\0')
			{
				res += tps;
			}
			return res;
		}

		void write(int i)
		{
			std::stringstream ss;
			ss << i;
			write(ss.str());
		}

		int readInt()
		{
			int i;
			std::istringstream(readString()) >> i;
			return i;
		}

		void write(long l)
		{
			std::stringstream ss;
			ss << l;
			write(ss.str());
		}

		long readLong()
		{
			long v;
			std::istringstream(readString()) >> v;
			return v;
		}

		void write(double d)
		{
			std::stringstream ss;
			ss << d;
			write(ss.str());
		}

		double readDouble()
		{
			double v;
			std::istringstream(readString()) >> v;
			return v;
		}

		void write(float v)
		{
			std::stringstream ss;
			ss << v;
			write(ss.str());
		}

		float readFloat()
		{
			float v;
			std::istringstream(readString()) >> v;
			return v;
		}

		bytes toBytes()
		{
			bytes res;
			res.len = writeCursor;
			return res;
		}

};

#endif /* BYTESMESSAGE_H_ */
