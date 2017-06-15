#include "CameraManager.h"
#include <glm/glm.hpp>
#include "GraphicsCommon.h"
#include "GraphicsConfig.h"
#include "GraphicsInterface.h"
#include "Camera.h"

using namespace Cooler;

CCameraManager::CCameraManager()
{
}

CCameraManager::~CCameraManager()
{
}

//***********************************************************
//FUNCTION:
Cooler::IResource* Cooler::CCameraManager::__createResouceV(const CGraphicsConfig* vConfig) const
{
//	_ASSERTE(glGetError() == GL_NO_ERROR);
	glm::vec3 Eye = vConfig->getAttribute<glm::vec3>(CONFIG_KEYWORD::CAMERA_EYE);
	glm::vec3 Center = vConfig->getAttribute<glm::vec3>(CONFIG_KEYWORD::CAMERA_CENTER);
	glm::vec3 Up = vConfig->getAttribute<glm::vec3>(CONFIG_KEYWORD::CAMERA_UP);
	glm::vec3 Perspective = vConfig->getAttribute<glm::vec3>(CONFIG_KEYWORD::CAMERA_PERSPECTIVE);

	CCamera* pCamea = new CCamera(Eye, Center, Up);

	const CGraphicsConfig* pSceneConfig = vConfig->getParentConfig();
	const CGraphicsConfig* pWindowConfig = pSceneConfig->fetchSubConfigByName(CONFIG_KEYWORD::WINDOW);
	int WindowWidth = pWindowConfig->getAttribute<int>(CONFIG_KEYWORD::WINDOW_WIDTH);
	int WindowHeight = pWindowConfig->getAttribute<int>(CONFIG_KEYWORD::WINDOW_HEIGHT);

	pCamea->initPerspective(Perspective, WindowWidth, WindowHeight);
//	_ASSERTE(glGetError() == GL_NO_ERROR);
	return pCamea;
}