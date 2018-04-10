#pragma once

#include "c_Grapic.h"
#include "c_Input.h"

class c_System
{ 
private:
	wstring m_applicationName;  //���ø����̼� �̸�
	HINSTANCE m_hinstance;		//������ �ν��Ͻ�
	HWND m_hwnd;				//������ ���

private:
	c_Input* m_pInput;			//��ǲ obj
	c_Grapic* m_pGraphics;		//�׷��� obj

private:
	/** ������ �ʱ�ȭ �Լ� **/
	bool InitializeWindows(int& screenWidth, int& screenHeight);
	/** ������(Update���� ����) �Լ� **/
	bool Frame();
	/** ������ ���� �Լ� **/
	void ShutdownWindows();
	/** ������ â��� ��ȯ �Լ� **/
	void SetWindowedMode();
	/** ������ ��üâ ��� ��ȯ �Լ� **/
	void SetWindowedFullScreenMode();

public:
	/** �ý��� �ʱ�ȭ �Լ� **/
	bool Initialize(); 
	/** �ý��� ���� �Լ� **/
	void Shutdown(); 
	/** �ý��� ���� �Լ� **/
	void Run(); 
	/** �ý��ۿ� PROC �Լ� **/
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg,
		WPARAM wparam, LPARAM lparam);

public:
	c_System();
	~c_System();
}; 

/** ������ PROC �Լ� **/
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, 
	WPARAM wparam, LPARAM lparam);



