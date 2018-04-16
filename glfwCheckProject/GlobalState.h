#pragma once

#include <memory>
#include <mutex>
#include <GL/glew.h>

class GlobalState
{
private:
	GlobalState();
	
	bool Init();
	void UnInit();

	static std::shared_ptr<GlobalState> m_pInstance;
	static std::mutex m_InstanceLock;
public:
	~GlobalState();

	static std::shared_ptr<GlobalState> Instance();
	static GLenum m_ePolygonMode;

};