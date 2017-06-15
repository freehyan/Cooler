#include "Model.h"
#include "GraphicsInterface.h"
#include "GraphicsMicro.h"
#include "SkinnedMesh.h"

using namespace Cooler;

CModel::CModel() : m_pMesh(nullptr)
{
}

CModel::~CModel()
{
	_SAFE_DELETE(m_pMesh);
}

bool Cooler::CModel::loadModelFile(const std::string& vModelFilePath, bool vWithAdjacencies /*= false*/)
{
	m_pMesh = new CSkinnedMesh;
	if (!m_pMesh->loadMesh(vModelFilePath))
	{
		Cooler::outputWarning(__EXCEPTION_SITE__, _BOOST_STR1("[%s%] model loads fail.", vModelFilePath));
		return false;
	}

	return true;
}

//***********************************************************
//FUNCTION:
void Cooler::CModel::render()
{
	_ASSERTE(m_pMesh);
	m_pMesh->render();
}