#include "Scene.h"
#include "Camera.h"
#include "GraphicsCommon.h"
#include "GraphicsMicro.h"
#include "GraphicsConfig.h"
#include "XmlParser.h"

using namespace Cooler;

CScene::CScene() : m_pSceneConfig(nullptr)
{
}

CScene::~CScene()
{
	_SAFE_DELETE(m_pSceneConfig);
	for (auto camera : m_pCameraSet)
		_SAFE_DELETE(camera.second);
}

//***********************************************************
//FUNCTION:
bool Cooler::CScene::initScene()
{
	for (unsigned int i = 0; i < m_pSceneConfig->getSubConfigNum(); i++)
	{
		CGraphicsConfig* pSubConfig = m_pSceneConfig->fetchSubConfigAt(i);
		_ASSERTE(pSubConfig);
		if (pSubConfig->getType() == "CAMERA")
		{
			//	m_pCamera = dynamic_cast<CCamera*>(CResourceExplorer::getInstance()->getOrCreateResource(pSubConfig));
			glm::vec3 Eye = pSubConfig->getAttribute<glm::vec3>(CONFIG_KEYWORD::CAMERA_EYE);
			glm::vec3 Center = pSubConfig->getAttribute<glm::vec3>(CONFIG_KEYWORD::CAMERA_CENTER);
			glm::vec3 Up = pSubConfig->getAttribute<glm::vec3>(CONFIG_KEYWORD::CAMERA_UP);
			CCamera* pCamea = new CCamera(Eye, Center, Up);
			m_pCameraSet[pSubConfig->getName()] = pCamea;
		}
	}

	return true;
}

//***********************************************************
//FUNCTION:
Cooler::CCamera* Cooler::CScene::fecthCamera(const std::string& vCameraName)
{
	if (m_pCameraSet.find(vCameraName) != m_pCameraSet.end())
		return m_pCameraSet[vCameraName];
	return nullptr;
}

//***********************************************************
//FUNCTION:
const Cooler::CCamera* Cooler::CScene::getCamera(const std::string& vCameraName) const
{
	auto itr = m_pCameraSet.find(vCameraName);
	if (itr != m_pCameraSet.end())
		return itr->second;

	return nullptr;
}
