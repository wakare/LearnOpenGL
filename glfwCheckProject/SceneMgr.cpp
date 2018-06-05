#include "SceneMgr.h"

SceneMgr::SceneMgr()
{
	m_sceneObjectVec.clear();
	m_mainCamera = Camera::GetCamera();
}

bool SceneMgr::SetMainCamera(std::shared_ptr<Camera> camera)
{
	m_mainCamera = camera;

	return true;
}

bool SceneMgr::AddSceneObject(std::shared_ptr<ISceneObject> sceneObject)
{
	m_sceneObjectVec.push_back(sceneObject);

	return true;
}

bool SceneMgr::DelSceneObject(std::shared_ptr<ISceneObject> sceneObject)
{
	bool deleted = false;
	auto it = m_sceneObjectVec.begin();
	for (; it < m_sceneObjectVec.end(); it++)
	{
		if (*it == sceneObject)
			break;
	}

	if (it != m_sceneObjectVec.end())
	{
		m_sceneObjectVec.erase(it);
		deleted = true;
	}

	return deleted;
}

void SceneMgr::ClearScene()
{
	m_sceneObjectVec.clear();
}