#include "stdafx.h"
#include "GlobalManager.h"

c_ControlWindow GlobalManager::sM_ControlWindow;
c_Hash GlobalManager::sM_Hash;
MemoryManager GlobalManager::sM_MemoryManager;

//------------------------------------------------------------------------------------
c_ControlWindow* GlobalManager::GetContorlWindow()
{
	return &sM_ControlWindow;
}
//------------------------------------------------------------------------------------
MemoryManager* GlobalManager::GetMemoryManager()
{
	return &sM_MemoryManager;
}
//------------------------------------------------------------------------------------
c_Hash* GlobalManager::GetHash()
{
	return &sM_Hash;
}
