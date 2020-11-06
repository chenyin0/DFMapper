#pragma once

#include "util.hpp"
#include <iomanip>
#include <time.h>
#include <sstream>

namespace Simulator::Mpr
{
	class Timer
	{
	public:
		Timer()
			: _startTime(std::chrono::system_clock::now())
		{
		}
		
		void start()
		{
			_startTime = std::chrono::system_clock::now();
		}

		string getCountTime() const
		{
			auto curTime = std::chrono::system_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - _startTime);
			string result = std::to_string(duration.count()) + "ms";
			return result;
		}
		
	public:
		static string getNow()
		{
			std::chrono::system_clock::time_point curTime = std::chrono::system_clock::now();
			std::time_t nowTime = std::chrono::system_clock::to_time_t(curTime);

			std::stringstream ss;
			ss << std::put_time(std::localtime(&nowTime), "%Y-%m-%d %X");

			string str = ss.str();

			return "[" + ss.str() + "] ";
		}

	private:
		std::chrono::system_clock::time_point _startTime;
	};
}
