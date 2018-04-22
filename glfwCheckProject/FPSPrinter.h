#pragma once
#include <iostream>
#include <atomic>

class FPSPrinter
{
private:
	unsigned long m_ulLastUpdateTime;
	size_t m_nFrameCount;
	std::atomic<float> m_fCurrentFPS;

public:
	FPSPrinter();
	void Update();
	float GetCurrentFPS();
};