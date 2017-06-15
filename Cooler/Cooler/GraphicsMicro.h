#pragma once
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "GraphicsInterface.h"

namespace Cooler
{
	#define __EXCEPTION_SITE__    __FUNCTION__, __FILE__, __LINE__

	#define _GRAPHICS_SIMPLE_IF(condition, opTrue) if(condition) {opTrue;}
	#define _GRAPHICS_SIMPLE_IF_ELSE(condition, opTrue, opFalse) if(condition) {opTrue;} else {opFalse;}

	#define _BOOST_STR1(s, aug1)             boost::str(boost::format(s) % (aug1))
	#define _BOOST_STR2(s, aug1, aug2)       boost::str(boost::format(s) % (aug1) % (aug2))
	#define _BOOST_STR3(s, aug1, aug2, aug3) boost::str(boost::format(s) % (aug1) % (aug2) % (aug3))
	#define _BOOST_STR4(s, aug1, aug2, aug3, aug4) boost::str(boost::format(s) % (aug1) % (aug2) % (aug3) % (aug4))
	#define _BOOST_STR5(s, aug1, aug2, aug3, aug4, aug5) boost::str(boost::format(s) % (aug1) % (aug2) % (aug3) % (aug4) % (aug5))
	#define _BOOST_STR6(s, aug1, aug2, aug3, aug4, aug5, aug6) boost::str(boost::format(s) % (aug1) % (aug2) % (aug3) % (aug4) % (aug5) % (aug6))
	#define _SAFE_DELETE(p)       {delete (p); (p)=nullptr; }


	#define CHECK_GL_ERROR() graphicsCheckError_(__FILE__, __LINE__)
}