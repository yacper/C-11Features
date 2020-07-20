// https://thispointer.com//c11-multithreading-part-2-joining-and-detaching-threads/
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
	class WorkerThread
	{
	public:
		void operator()()
		{
			std::ostringstream oss;
			oss<< "Worker Thread " << std::this_thread::get_id() << " is Executing" << std::endl;

			Logger::WriteMessage(oss.str().c_str());
		}
	};


	TEST_CLASS(C11JoiningDetachingThread)
	{
	public:
		TEST_METHOD(JoinThread)  // Join等待线程执行完毕
		{
			// 创建10个工作线程
			std::vector<std::thread> threadList;
			for (int i = 0; i < 10; i++)
			{
				threadList.push_back(std::thread(WorkerThread()));
			}

			// Now wait for all the worker thread to finish i.e.
			// Call join() function on each of the std::thread object

			Logger::WriteMessage("wait for all the worker thread to finish");

			std::for_each(threadList.begin(), threadList.end(), std::mem_fn(&std::thread::join));

			Logger::WriteMessage("Exiting from Main Thread" );

		}


		TEST_METHOD(ThreadJoinable)  // thread 是否可以join
		{
			// before calling join() or detach() we should check if thread is join-able every time i.e.
			std::thread threadObj((WorkerThread()));
			if (threadObj.joinable())
			{
				std::cout << "Detaching Thread " << std::endl;
				threadObj.detach();
			}
			if (threadObj.joinable())		// not joinable
			{
				std::cout << "Detaching Thread " << std::endl;
				threadObj.detach();
			}

			std::thread threadObj2((WorkerThread()));
			if (threadObj2.joinable())
			{
				std::cout << "Joining Thread " << std::endl;
				threadObj2.join();
			}
			if (threadObj2.joinable())
			{
				std::cout << "Joining Thread " << std::endl;
				threadObj2.join();
			}
		}


		class ThreadRAII  // thread detruct wrapper
		{
			std::thread& m_thread;
		public:
			ThreadRAII(std::thread& threadObj) : m_thread(threadObj)
			{

			}
			~ThreadRAII()
			{
				// Check if thread is joinable then detach the thread
				if (m_thread.joinable())
				{
					m_thread.detach();
				}
			}
		};


		TEST_METHOD(ThreadDestructWrapper)  // thread销毁包装器 thread object必须join或者detach，不然它自己destruct的时候，会terminate program
		{
			std::thread threadObj((WorkerThread()));

			// If we comment this Line, then program will crash
			ThreadRAII wrapperObj(threadObj);
		}
	};
}