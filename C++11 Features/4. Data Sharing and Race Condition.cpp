// https://thispointer.com//c11-multithreading-part-4-data-sharing-and-race-conditions/
#include "pch.h"
#include "CppUnitTest.h"
#include <thread>
#include <mutex>
#include <string.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace C11Features
{
	TEST_CLASS(C11ThreadDataSharaingRaceCondition)
	{
	public:
		static void threadCallback(int x, std::string str)
		{
			Logger::WriteMessage(std::string("Passed Number = ").append(std::to_string(x)).c_str());
			Logger::WriteMessage(std::string("Passed String = ").append(str).c_str());
		}
		TEST_METHOD(PassSimpleArguments)  // 传递简单参数
		{
			int x = 10;
			std::string str = "Sample String";
			std::thread threadObj(threadCallback, x, str);
			threadObj.join();
		}
	};
}