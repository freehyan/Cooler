#pragma once
#include "ResourceManager.h"

namespace Cooler
{
	class IResource;

	class CCameraManager : public IResourceManager
	{
	public:
		CCameraManager();
		virtual ~CCameraManager();

	private:
		virtual IResource* __createResouceV(const CGraphicsConfig* vConfig) const override;

	private:

	};
}