#pragma  once
#include "OutputTarget.h"

namespace Cooler
{
	class COutputToScreen : public COutputTarget
	{
	public:
		COutputToScreen();
		virtual ~COutputToScreen() {}

	protected:
		virtual void _outputEventV(const std::string& vEvent) override;
		virtual bool _isOutputEventV(EEventLevel vLevel) const override { return (vLevel >= WARNING_LEVEL); }
	};
}