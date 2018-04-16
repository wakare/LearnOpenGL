#include "GlobalState.h"
#include <iostream>

bool GlobalState::Init()
{
	// TODO: Init variables

	m_ePolygonMode = GL_FILL;
	return true;
}

void GlobalState::UnInit()
{
	// TODO: UnInit variables

	return;
}

std::shared_ptr<GlobalState> GlobalState::Instance()
{
	if (m_pInstance == nullptr)
	{
		m_InstanceLock.lock();
		if (m_pInstance == nullptr)
			m_pInstance = std::make_shared<GlobalState>();

		m_InstanceLock.unlock();
	}

	return m_pInstance;
}

GlobalState::~GlobalState()
{
	UnInit();
}

GlobalState::GlobalState()
{
	if (!Init())
	{
		std::cout << "[ERROR] Init GlobalState failed.";
		return;
	}
}

