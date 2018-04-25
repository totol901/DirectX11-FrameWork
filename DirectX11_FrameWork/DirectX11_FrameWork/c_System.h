#pragma once
#include "c_Grapic.h"
#include "InputManager.h"

namespace GameEngine
{
	class c_System
	{
	public:
		c_System();
		~c_System();

	public:
		/** 시스템 초기화 함수 **/
		bool Initialize(HINSTANCE hInstance, const wstring wS_ApplicationName);
		/** 시스템 해제 함수 **/
		void Shutdown();
		/** 시스템 구동 함수 **/
		void Run();

	private:
		/** 프래임(Update마다 도는) 함수 **/
		bool Frame();

	private:
		c_Grapic* m_pGraphics;		//그래픽 obj

	};
};
