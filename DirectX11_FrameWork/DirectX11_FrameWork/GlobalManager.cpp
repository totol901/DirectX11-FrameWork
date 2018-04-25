#include "stdafx.h"
#include "GlobalManager.h"

c_ControlWindow GlobalManager::sM_ControlWindow;

//------------------------------------------------------------------------------------
c_ControlWindow &GlobalManager::GetContorlWindow()
{
	return sM_ControlWindow;
}
