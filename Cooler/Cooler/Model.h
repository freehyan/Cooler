#pragma once
#include "GraphicsCommon.h"
#include "Resource.h"

namespace Cooler
{
	class CSkinnedMesh;

	class CModel : public IResource
	{
	public:
		CModel();
		~CModel();

		bool loadModelFile(const std::string& vModelFilePath, bool vWithAdjacencies = false);
		void render();

		void setModelSig(const std::string& vModelName) { m_ModelName = vModelName; }
	private:
		std::string m_ModelName;
		CSkinnedMesh* m_pMesh;
	};
}