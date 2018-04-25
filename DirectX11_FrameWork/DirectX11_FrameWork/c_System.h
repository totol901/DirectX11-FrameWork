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
		/** �ý��� �ʱ�ȭ �Լ� **/
		bool Initialize(HINSTANCE hInstance, const wstring wS_ApplicationName);
		/** �ý��� ���� �Լ� **/
		void Shutdown();
		/** �ý��� ���� �Լ� **/
		void Run();

	private:
		/** ������(Update���� ����) �Լ� **/
		bool Frame();

	private:
		c_Grapic* m_pGraphics;		//�׷��� obj

	};
};
