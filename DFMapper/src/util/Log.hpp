#pragma once

#include "util.hpp"
#include "Timer.hpp"
#include <fstream>

namespace DFMpr
{
	class Log
	{
	public:
		explicit Log(const string& addr)
		{
			_file.open(addr);
			if (!_file.is_open())
				DEBUG_ASSERT(false);
		}

		~Log()
		{
			_file.close();
		}
		
	public:

		void write(const string& str)
		{
			string s = Timer::getNow() + str;
			_file << s << std::endl;
		}

		void writeNoReturn(const string& str)
		{
			string s = Timer::getNow() + str;
			_file << s;
		}

		void writeNoTimeNoReturn(const string& str)
		{
			_file << str;
		}

		void writeNoTime(const string& str)
		{
			_file << str << std::endl;
		}
		
	private:
		std::ofstream _file;
	};
	
}