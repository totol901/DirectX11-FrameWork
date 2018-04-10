#include "stdafx.h"
#include "c_system.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{ 
	//메모리 릭 채크
	#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	c_System* System; 
	bool result; 

	// system 객체를 생성한다. 
	System = new c_System;
	if(!System) 
	{
		return 0; 
	} 

	// system 객체를 초기화하고 run을 호출한다. 
	result = System->Initialize(); 
	if(result) 
	{ 
		System->Run(); 
	} 
	// system객체를 종료하고 메모리를 반환한다. 
	System->Shutdown(); 
	
	SAFE_DELETE(System);

	return 0; 
}
