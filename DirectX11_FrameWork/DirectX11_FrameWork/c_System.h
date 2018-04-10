#pragma once

#include "c_Grapic.h"
#include "c_Input.h"

class c_System
{ 
private:
	wstring m_applicationName;  //어플리케이션 이름
	HINSTANCE m_hinstance;		//윈도우 인스턴스
	HWND m_hwnd;				//윈도우 헨들

private:
	c_Input* m_pInput;			//인풋 obj
	c_Grapic* m_pGraphics;		//그래픽 obj

private:
	/** 윈도우 초기화 함수 **/
	bool InitializeWindows(int& screenWidth, int& screenHeight);
	/** 프래임(Update마다 도는) 함수 **/
	bool Frame();
	/** 윈도우 해제 함수 **/
	void ShutdownWindows();
	/** 윈도우 창모드 변환 함수 **/
	void SetWindowedMode();
	/** 윈도우 전체창 모드 변환 함수 **/
	void SetWindowedFullScreenMode();

public:
	/** 시스템 초기화 함수 **/
	bool Initialize(); 
	/** 시스템 해제 함수 **/
	void Shutdown(); 
	/** 시스템 구동 함수 **/
	void Run(); 
	/** 시스템용 PROC 함수 **/
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg,
		WPARAM wparam, LPARAM lparam);

public:
	c_System();
	~c_System();
}; 

/** 윈도우 PROC 함수 **/
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, 
	WPARAM wparam, LPARAM lparam);



