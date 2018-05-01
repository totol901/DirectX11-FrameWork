#include "stdafx.h"
#include "GlobalManager.h"

c_ControlWindow GlobalManager::sM_ControlWindow;
MemoryManager GlobalManager::sM_MemooryManager;

//------------------------------------------------------------------------------------
c_ControlWindow &GlobalManager::GetContorlWindow()
{
	return sM_ControlWindow;
}

MemoryManager & GameEngine::GlobalManager::GetMemoryManager()
{
	return sM_MemooryManager;
}
