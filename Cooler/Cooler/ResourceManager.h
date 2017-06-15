#pragma once
#include <boost/any.hpp>
#include <map>

namespace Cooler
{
	class CGraphicsConfig;
	class IResource;

	class IResourceManager
	{
	public:
		IResourceManager();
		virtual ~IResourceManager();

		IResource* createResource(const CGraphicsConfig* vConfig);
		IResource* fetchResource(const std::string& vResourceKey);

	private:
		virtual IResource* __createResouceV(const CGraphicsConfig* vConfig) const { return nullptr; }

	private:
		std::map<std::string, IResource*> m_Key2ResourceMap;
	};
}