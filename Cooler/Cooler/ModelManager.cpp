#include "ModelManager.h"
#include <gl/glew.h>
#include "Model.h"
#include "GraphicsCommon.h"
#include "GraphicsConfig.h"

using namespace Cooler;

CModelManager::CModelManager()
{
}

CModelManager::~CModelManager()
{
}

//***********************************************************
//FUNCTION:
Cooler::IResource* Cooler::CModelManager::__createResouceV(const CGraphicsConfig* vConfig) const
{
//	_ASSERTE(glGetError() == GL_NO_ERROR);
	_ASSERTE(vConfig);

	if (vConfig->getType() == CONFIG_KEYWORD::MODEL)
	{
		CModel* pModel = new CModel;
		pModel->setModelSig(vConfig->getName());
		pModel->loadModelFile(DEFAULT_CONFIG::MODEL_PATH + vConfig->getAttribute<std::string>(CONFIG_KEYWORD::MODEL_FILE));

		return pModel;
	}
//	_ASSERTE(glGetError() == GL_NO_ERROR);
	return nullptr;
}
