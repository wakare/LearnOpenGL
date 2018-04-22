#include "FPSPrinter.h"
#include <windows.h>

FPSPrinter::FPSPrinter()
{
	m_nFrameCount = 0;
	m_ulLastUpdateTime = GetTickCount64();
}

void FPSPrinter::Update()
{
	m_nFrameCount++;

	auto nowTime = GetTickCount64();
	auto timeInterval = nowTime - m_ulLastUpdateTime;
	if (timeInterval < 1000.0f)
		return;

	m_fCurrentFPS = (m_nFrameCount * 1000.0f) / timeInterval;
	m_nFrameCount = 0;
	m_ulLastUpdateTime = nowTime;

	std::cout << "FPS: " << GetCurrentFPS() << std::endl;
}

float FPSPrinter::GetCurrentFPS()
{
	return m_fCurrentFPS;
}