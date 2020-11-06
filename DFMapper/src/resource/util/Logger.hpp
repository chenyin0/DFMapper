#pragma once

#include "util.hpp"
#include "Timer.hpp"
#include <fstream>

namespace Simulator::Mpr
{
	class Logger
	{
	public:
		explicit Logger(const string& addr)
		{
			_file.open(addr);
			if (!_file.is_open())
				DEBUG_ASSERT(false);
		}

		~Logger()
		{
			_file.close();
		}
		
	public:

		void write(const string& str)
		{
			string s = Timer::getNow() + str;
			_file << s << endl;
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
			_file << str << endl;
		}
		
	private:
		std::ofstream _file;
	};
	
}