#include <vector>
#include <memory>
#include "ISceneObject.h"
#include "Camera.h"
class SceneMgr
{
private:
	std::vector<std::shared_ptr<ISceneObject>> m_sceneObjectVec;
	std::shared_ptr<Camera> m_mainCamera;
public:
	SceneMgr();
	bool SetMainCamera(std::shared_ptr<Camera> camera);
	bool AddSceneObject(std::shared_ptr<ISceneObject> sceneObject);
	bool DelSceneObject(std::shared_ptr<ISceneObject> sceneObject);
	void ClearScene();
};