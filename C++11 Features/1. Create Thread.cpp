// https://thispointer.com/c-11-multithreading-part-1-three-different-ways-to-create-threads/
#include "pch.h"
#include "CppUnitTest.h"
#include <thread>
#include <mutex>
#include <string.h>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace C11Features
{
	class DisplayThread
	{
	public:
		void operator()()
		{
			for (int i = 1; i <= 1000; i++)
				Logger::WriteMessage(std::string("thread function Executing ").append(std::to_string(i)).c_str());
		}
	};


	TEST_CLASS(C11CreateThread)
	{
	public:
		static void	hello_thread()
		{
			Logger::WriteMessage("hello thread");
		}

		static void thread_function()
		{
			for (int i = 1; i <= 1000; i++)
				Logger::WriteMessage(std::string("thread function Executing ").append(std::to_string(i)).c_str());
		}


		TEST_METHOD(SimpleThread)  // 
		{
			std::thread t1(hello_thread);
			t1.detach();

			// 主线程代码
			Logger::WriteMessage("main thread");
		}


		TEST_METHOD(CreateWithFunctionPointer)  // 用函数指针创建
		{
			// 创建并启动线程
			std::thread threadObj(thread_function);

			for (int i = 1; i <= 1000; i++)
				Logger::WriteMessage( std::string("MainThread ").append(std::to_string(i)).c_str());

			// 主线程等待子线程执行完毕
			threadObj.join();

			Logger::WriteMessage("MainThread Exit");
		}

		TEST_METHOD(CreateWithFunctionObject)  // 用函数对象创建
		{
			// 创建并启动线程
			std::thread threadObj((DisplayThread()));

			for (int i = 1; i <= 1000; i++)
				Logger::WriteMessage( std::string("MainThread ").append(std::to_string(i)).c_str());

			// 主线程等待子线程执行完毕
			threadObj.join();

			Logger::WriteMessage("MainThread Exit");
		}
		
		TEST_METHOD(CreateWithLambdaFunctions)  // 用lambada functions创建
		{
			// 创建并启动线程
			std::thread threadObj(
				[] {
					for (int i = 1; i <= 1000; i++)
						Logger::WriteMessage(std::string("thread function Executing ").append(std::to_string(i)).c_str());
				}
			);

			for (int i = 1; i <= 1000; i++)
				Logger::WriteMessage( std::string("MainThread ").append(std::to_string(i)).c_str());

			// 主线程等待子线程执行完毕
			threadObj.join();

			Logger::WriteMessage("MainThread Exit");
		}

		static void thread_function_PrintID()
		{
			std::ostringstream oss;
			oss << "Inside Thread :: ID  = " << std::this_thread::get_id();

			Logger::WriteMessage( oss.str().c_str());
		}

		TEST_METHOD(GetThreadID)  // 获取Thread ID
		{
			std::ostringstream oss;

			std::thread threadObj1(thread_function_PrintID);
			std::thread threadObj2(thread_function_PrintID);

			if (threadObj1.get_id() != threadObj2.get_id())
			{
				Logger::WriteMessage( "Both Threads have different IDs");
			}

			oss << "From Main Thread :: ID of Thread 1 = " << threadObj1.get_id() << std::endl;
			Logger::WriteMessage( oss.str().c_str());

			oss.str(std::string()); // clear
			oss << "From Main Thread :: ID of Thread 2 = " << threadObj2.get_id() << std::endl;
			Logger::WriteMessage( oss.str().c_str());

			threadObj1.join();
			threadObj2.join();
		}



	};
}