#include "stdafx.h"
#include "c_system.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{ 
	//�޸� �� äũ
	#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	c_System* System; 
	bool result; 

	// system ��ü�� �����Ѵ�. 
	System = new c_System;
	if(!System) 
	{
		return 0; 
	} 

	// system ��ü�� �ʱ�ȭ�ϰ� run�� ȣ���Ѵ�. 
	result = System->Initialize(); 
	if(result) 
	{ 
		System->Run(); 
	} 
	// system��ü�� �����ϰ� �޸𸮸� ��ȯ�Ѵ�. 
	System->Shutdown(); 
	
	SAFE_DELETE(System);

	return 0; 
}
