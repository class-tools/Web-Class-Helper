/*
Web Class Helper Unit Test Main File 2.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: ren-yc
*/
#include "../WCH.cpp"
#include "../modules/file-process.hpp"
#include "../modules/init.hpp"
#include "../modules/commands.hpp"
#include "../modules/functions.hpp"
#include "../modules/apis.hpp"
#include "../modules/basic.hpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
TEST_CLASS(UnitTest) {
public:
	TEST_METHOD(StrConvert) {
		Assert::AreEqual(StrToWstr("Test").c_str(), L"Test");
		Assert::AreEqual(WstrToStr(L"Test").c_str(), "Test");
		Assert::AreEqual(StrToWstr("测试").c_str(), L"测试");
		Assert::AreEqual(WstrToStr(L"测试").c_str(), "测试");
		Assert::AreEqual(StrToWstr("テスト").c_str(), L"テスト");
		Assert::AreEqual(WstrToStr(L"テスト").c_str(), "テスト");
	}
	TEST_METHOD(EncodeURL) {
		Assert::AreEqual(UrlEncode("测试").c_str(), "%E6%B5%8B%E8%AF%95");
	}
	TEST_METHOD(StrSplit) {
		vector<wstring> Temp;
		Temp = {L"A", L"BC"};
		Assert::IsTrue(WCH_split(L"A BC") == Temp);
		Temp = {L"A B", L"C"};
		Assert::IsTrue(WCH_split(L"\"A B\" C") == Temp);
		Temp = {L"你", L"好 啊"};
		Assert::IsTrue(WCH_split(L"你 \"好 啊\"") == Temp);
	}
	TEST_METHOD(StrDisplaySize) {
		Assert::AreEqual(WCH_GetWstrDisplaySize(L"Test"), (size_t)4);
		Assert::AreEqual(WCH_GetWstrDisplaySize(L"测试"), (size_t)4);
		Assert::AreEqual(WCH_GetWstrDisplaySize(L"テスト"), (size_t)6);
	}
	TEST_METHOD(CheckVersion) {
		Assert::AreEqual(WCH_CheckVersion({0, 0, 0}, {0, 0, 0}), false);
		Assert::AreEqual(WCH_CheckVersion({0, 0, 0}, {18446744073709551615, 18446744073709551615, 18446744073709551615}), true);
	}
	TEST_METHOD(GetVersion) {
		WCH_Version Temp;
		Temp = {(size_t)0, (size_t)0, (size_t)0};
		Assert::IsTrue(WCH_GetVersion(L"0.0.0") == Temp);
		Temp = {(size_t)18446744073709551615, (size_t)18446744073709551615, (size_t)18446744073709551615};
		Assert::IsTrue(WCH_GetVersion(L"18446744073709551615.18446744073709551615.18446744073709551615") == Temp);
	}
	TEST_METHOD(NumDigits) {
		Assert::AreEqual(WCH_GetNumDigits(0), (size_t)1);
		Assert::AreEqual(WCH_GetNumDigits(18446744073709551615), (size_t)20);
	}
};
}