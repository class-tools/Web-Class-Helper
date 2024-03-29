/*
Web Class Helper Unit Test Main File 2.1.2
This source code file is under MIT License.
Copyright (c) 2022 - 2023 Class Tools Develop Team
Contributors: ren-yc
*/
#include "../version.hpp"
#include "../shasum.hpp"
#include "../modules/file-process.hpp"
#include "../modules/init.hpp"
#include "../modules/commands.hpp"
#include "../modules/functions.hpp"
#include "../modules/apis.hpp"
#include "../modules/basic.hpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace APIs {
TEST_CLASS(String) {
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
		Assert::AreEqual(UrlEncode("测 试").c_str(), "%E6%B5%8B+%E8%AF%95");
		Assert::AreEqual(UrlEncode("测.试").c_str(), "%E6%B5%8B.%E8%AF%95");
		Assert::AreEqual(UrlEncode("测A试").c_str(), "%E6%B5%8BA%E8%AF%95");
	}
	TEST_METHOD(StrSplit) {
		Assert::IsTrue(WCH_split(L"A BC") == []() {
			vector<wstring> _tmp = {L"A", L"BC"};
			return _tmp;
		}());
		Assert::IsTrue(WCH_split(L"\"A B\" C") == []() {
			vector<wstring> _tmp = {L"A B", L"C"};
			return _tmp;
		}());
		Assert::IsTrue(WCH_split(L"A B C") == []() {
			vector<wstring> _tmp = {L"A", L"B", L"C"};
			return _tmp;
		}());
		Assert::IsTrue(WCH_split(L"你 \"好 啊\"") == []() {
			vector<wstring> _tmp = {L"你", L"好 啊"};
			return _tmp;
		}());
		Assert::IsTrue(WCH_split(L"A B C\"") == []() {
			vector<wstring> _tmp = {L"Incorrect"};
			return _tmp;
		}());
	}
	TEST_METHOD(StrDisplaySize) {
		Assert::AreEqual(WCH_GetWstrDisplaySize(L"Test"), (size_t)4);
		Assert::AreEqual(WCH_GetWstrDisplaySize(L"测试"), (size_t)4);
		Assert::AreEqual(WCH_GetWstrDisplaySize(L"テスト"), (size_t)6);
		Assert::AreEqual(WCH_GetWstrDisplaySize(L"T测テ"), (size_t)5);
	}
	TEST_METHOD(NewlineCount) {
		Assert::AreEqual(WCH_NewlineCount(L"\nTest\n"), (size_t)2);
		Assert::AreEqual(WCH_NewlineCount(L"\n\n\n"), (size_t)3);
	}
};

TEST_CLASS(Version) {
public:
	TEST_METHOD(CheckVersion) {
		WCH_Version Temp1 = {0, 0, 0}, Temp2 = {2147483647, 2147483647, 2147483647};
		Assert::IsFalse(Temp1 < Temp1);
		Assert::IsTrue(Temp1 < Temp2);
	}
	TEST_METHOD(GetVersion) {
		WCH_Version Temp;
		Temp = {0, 0, 0};
		Assert::IsTrue(WCH_GetVersion(L"0.0.0") == Temp);
		Temp = {2147483647, 2147483647, 2147483647};
		Assert::IsTrue(WCH_GetVersion(L"2147483647.2147483647.2147483647") == Temp);
	}
};

TEST_CLASS(Other) {
public:
	TEST_METHOD(NumDigits) {
		Assert::AreEqual(WCH_GetNumDigits((size_t)0), (size_t)1);
		Assert::AreEqual(WCH_GetNumDigits((size_t)18446744073709551615), (size_t)20);
	}
	TEST_METHOD(CheckConfigValid) {
		pair<bool, wstring> Temp;
		Temp = WCH_CheckConfigValid(L"FocusEndContent", L"Test");
		Assert::IsTrue(Temp.first);
		Assert::AreEqual(Temp.second.c_str(), L"String");
		Temp = WCH_CheckConfigValid(L"FocusEndPrompt", L"True");
		Assert::IsTrue(Temp.first);
		Assert::AreEqual(Temp.second.c_str(), L"Boolean");
		Temp = WCH_CheckConfigValid(L"FocusKillInterval", L"2147483647");
		Assert::IsFalse(Temp.first);
		Assert::AreEqual(Temp.second.c_str(), L"String");
		Temp = WCH_CheckConfigValid(L"FocusKillInterval", L"60000");
		Assert::IsTrue(Temp.first);
		Assert::AreEqual(Temp.second.c_str(), L"Number");
		Temp = WCH_CheckConfigValid(L"Language", L"Test");
		Assert::IsFalse(Temp.first);
		Assert::AreEqual(Temp.second.c_str(), L"String");
		Temp = WCH_CheckConfigValid(L"ScreenshotSavePath", L"C:\\Test");
		Assert::IsFalse(Temp.first);
		Assert::AreEqual(Temp.second.c_str(), L"String");
		Temp = WCH_CheckConfigValid(L"Test", L"Test");
		Assert::IsFalse(Temp.first);
		Assert::AreEqual(Temp.second.c_str(), L"String");
	}
};
}
