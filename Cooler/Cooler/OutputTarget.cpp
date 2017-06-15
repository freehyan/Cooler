#include "OutputTarget.h"
using namespace Cooler;

COutputTarget::COutputTarget() : m_Initialized(false)
{
}

COutputTarget::~COutputTarget()
{
}

//***********************************************************
//FUNCTION:
void COutputTarget::writeEvent(const std::string& vEvent)
{
	if (!m_Initialized)
	{
		_initOutputTargetV();
		m_Initialized = true;
	}

	_outputEventV(vEvent);
}

