#pragma once
#include "ResourceManager.h"

namespace Cooler
{
	class IResource;

	class CModelManager : public IResourceManager
	{
	public:
		CModelManager();
		virtual ~CModelManager();

	private:
		virtual IResource* __createResouceV(const CGraphicsConfig* vConfig) const override;

	private:

	};
}