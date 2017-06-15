#pragma once
#include "ResourceManager.h"

namespace Cooler
{
	class IResource;

	class CTextureManager : public IResourceManager
	{
	public:
		CTextureManager();
		virtual ~CTextureManager();

	private:
		virtual IResource* __createResouceV(const CGraphicsConfig* vConfig) const override;

	private:

	};
}