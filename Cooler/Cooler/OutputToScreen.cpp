#include "OutputToScreen.h"
#include <iostream>
#include "ProductFactory.h"

using namespace Cooler;
CProductFactory<COutputToScreen> theCreator(OUTPUT_EVENT_2_SCREEN);

COutputToScreen::COutputToScreen()
{
}

//*********************************************************************************
//FUNCTION:
void COutputToScreen::_outputEventV(const std::string& vEvent)
{
	std::cout << vEvent << std::endl;
}