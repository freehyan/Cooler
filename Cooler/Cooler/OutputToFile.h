#pragma  once
#include <fstream>
#include <boost/thread/mutex.hpp>
#include "OutputTarget.h"

namespace Cooler
{
	class COutputToFile : public COutputTarget
	{
	public:
		COutputToFile();
		virtual ~COutputToFile();

	protected:
		virtual void _outputEventV(const std::string& vEvent) override;

	private:
		std::string  m_LogFileName;
		std::string  m_Buffer;
		boost::mutex m_Mutex;
		std::fstream m_LogFile;
		unsigned int m_BufferMaxSize;
	};
}
