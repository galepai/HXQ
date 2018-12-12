#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include "../HXQ/Func.h"
#include "../HXQ/CameraThread.h"
#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(Func)
		{
			// TODO: 在此输入测试代码
			std::vector<bool> num = Parse_Galil_Input(16);
			//Assert::AreEqual(true, num[0]);
			Assert::IsFalse(num[0]);
			Assert::IsFalse(num[3]);
		}

		TEST_METHOD(CameraThread)
		{
			// TODO: 在此输入测试代码
			Camera_Thread* cam = new Camera_Thread(Camera_Thread::ConnectionType::GigEVision2, "0030531d42b0_Basler_raL204848gm");
			//cam->start();
			//Sleep(500);
			Assert::IsTrue(cam);

		}

	};
}