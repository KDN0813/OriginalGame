#pragma once

#include <windows.h>
#include <crtdbg.h>
#include <cwchar>

#if defined( DEBUG ) || defined( _DEBUG )
#define _ASSERT_EXPR_A(expr, msg) \
	(void)((!!(expr)) || \
	(1 != _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, "%s", msg)) || \
	(_CrtDbgBreak(), 0))

#define _ASSERT_EXPR_W(expr, msg) \
    (void)((!!(expr)) || \
    (1 != _CrtDbgReportW(_CRT_ASSERT, __FILEW__, __LINE__, NULL, L"%s", msg)) || \
    (_CrtDbgBreak(), 0))
#else
#define  _ASSERT_EXPR_A(expr, expr_str) ((void)0)
#define  _ASSERT_EXPR_W(expr, expr_str) ((void)0)
#endif

inline LPWSTR HRTrace(HRESULT hr)
{
	LPWSTR msg;
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&msg), 0, NULL);
	return msg;
}

class Benchmark
{
	LARGE_INTEGER ticksPerSecond;
	LARGE_INTEGER startTicks;
	LARGE_INTEGER currentTicks;

public:
	Benchmark()
	{
		QueryPerformanceFrequency(&ticksPerSecond);
		QueryPerformanceCounter(&startTicks);
		QueryPerformanceCounter(&currentTicks);
	}
	void begin()
	{
		QueryPerformanceCounter(&startTicks);
	}
	float end()
	{
		QueryPerformanceCounter(&currentTicks);
		return static_cast<float>(currentTicks.QuadPart - startTicks.QuadPart) / static_cast<float>(ticksPerSecond.QuadPart);
	}
};