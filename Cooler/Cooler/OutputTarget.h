#pragma once
#include "BaseProduct.h"
#include "GraphicsCommon.h"

namespace Cooler
{
	class COutputTarget : public CBaseProduct
	{
	public:
		COutputTarget();
		virtual ~COutputTarget();

		void writeEvent(const std::string& vEvent);

	protected:
		virtual void _outputEventV(const std::string& vEvent) {}
		virtual void _initOutputTargetV() {}
		virtual bool _isOutputEventV(EEventLevel vLevel) const { return true; }

	private:
		bool m_Initialized;

	friend class CEventLogger;
	};
}