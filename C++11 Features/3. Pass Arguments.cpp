// https://thispointer.com//c11-multithreading-part-3-carefully-pass-arguments-to-threads/
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
	TEST_CLASS(C11ThreadPassArguments)
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


		static void newThreadCallback(int* p)
		{
			std::cout << "Inside Thread :  "" : p = " << p << std::endl;
			std::chrono::milliseconds dura(1000);
			std::this_thread::sleep_for(dura);
			*p = 19;
		}
		static void startNewThread()
		{
			int i = 10;
			std::cout << "Inside Main Thread :  "" : i = " << i << std::endl;
			std::thread t(newThreadCallback, &i);
			t.detach();
			std::cout << "Inside Main Thread :  "" : i = " << i << std::endl;
		}


		TEST_METHOD(DoNotPassPointToLocalStackVariables)  // 不要传递在当前栈的变量的地址给线程
		{
			//Don’t pass addresses of variables from local stack to thread’s callback function. 
			//Because it might be possible that local variable in Thread 1 goes out of scope but Thread 2 is still trying to access it through it’s address.
			//In such scenario accessing invalid address can cause unexpected behaviour.
			startNewThread();
			std::chrono::milliseconds dura(2000);
			std::this_thread::sleep_for(dura);
		}

		static void  startNewThread2()
		{
			int* p = new int();
			*p = 10;
			std::cout << "Inside Main Thread :  "" : *p = " << *p << std::endl;
			std::thread t(newThreadCallback, p);
			t.detach();
			delete p;
			p = NULL;
		}


		TEST_METHOD(PossibleErrorPassPointToHeap)  // 传递堆内存中的指针也可能会出现错误
		{
			//Similarly be careful while passing pointer to memory located on heap to thread.Because it might be possible that some thread deletes that memory before new thread tries to access it.
			//In such scenario accessing invalid address can cause unexpected behaviour.
			startNewThread2();
			std::chrono::milliseconds dura(2000);
			std::this_thread::sleep_for(dura);
		}



		static void threadCallback2(int const& x)
		{
			int& y = const_cast<int&>(x);
			y++;
			std::cout << "Inside Thread x = " << x << std::endl;
		}

		TEST_METHOD(PassReferenceWrong)  // 传递引用的错误方法 
		{
			//Even if threadCallback accepts arguments as reference but still changes done it are not visible outside the thread.
			//Its because x in the thread function threadCallback is reference to the temporary value copied at the new thread’s stack.

			int x = 9;
			std::cout << "In Main Thread : Before Thread Start x = " << x << std::endl;
			std::thread threadObj(threadCallback2, x);
			threadObj.join();
			std::cout << "In Main Thread : After Thread Joins x = " << x << std::endl;
		}

		TEST_METHOD(PassReferenceRight)  // 传递引用的正确方法 std::ref
		{
			int x = 9;
			std::cout << "In Main Thread : Before Thread Start x = " << x << std::endl;
			std::thread threadObj(threadCallback2, std::ref(x));
			threadObj.join();
			std::cout << "In Main Thread : After Thread Joins x = " << x << std::endl;
		}



		class DummyClass {
		public:
			DummyClass(int i)
				:y(i)
			{}
			DummyClass(const DummyClass& obj)
			{}
			void sampleMemberFunction(int x)
			{
				std::ostringstream oss;
				oss << "Inside sampleMemberFunction x:" << x  << " y:"<< y << std::endl;
				Logger::WriteMessage(oss.str().c_str());
			}
			int y;
		};

		TEST_METHOD(PassPointToMemberFunction)  // 传递对象的成员函数
		{
			DummyClass dummyObj(0);
			int x = 10;
			std::thread threadObj(&DummyClass::sampleMemberFunction, &dummyObj, x);
			threadObj.join();
		}


	};
}