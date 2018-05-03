#pragma once

namespace GameEngine
{
	//윈도우 창 스타일
	enum E_WINDOW_MODE_STYLE
	{
		E_WINDOW_FULL_SCREEN,
		E_FULL_SCREEN,
		E_WINDOW
	};

	class c_ControlWindow
	{
	public:
		c_ControlWindow();
		~c_ControlWindow();

		/** 윈도우 초기화 함수 **/
		bool CreateControlWindow(HINSTANCE hInstance, std::wstring wS_ApplicationName, E_WINDOW_MODE_STYLE eWindowModeStyle, int &iScreenWidth, int &iScreenHeight);
		/** 윈도우 해제 함수 **/
		void DestroyControlWindow();
		/** 윈도우 창모드 변환 함수 **/
		void SetWindowedMode();
		/** 윈도우 전체창 모드 변환 함수 **/
		void SetWindowedFullScreenMode();
		/** 게임용 PROC 함수 **/
		LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg,
			WPARAM wparam, LPARAM lparam);

	public:
		const HWND &GetControlWindowHandle();
		const E_WINDOW_MODE_STYLE &GetWindowModeStyle();

	private:
		std::wstring m_wS_ApplicationName;
		E_WINDOW_MODE_STYLE m_eWindowModeStyle;
		HINSTANCE m_hInstance;
		HWND m_hContorlWindowHandle;
		HDC m_Hdc;
	};
};

/** 윈도우 PROC 함수 **/
extern LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);