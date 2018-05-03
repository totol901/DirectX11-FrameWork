#pragma once

namespace GameEngine
{
	//������ â ��Ÿ��
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

		/** ������ �ʱ�ȭ �Լ� **/
		bool CreateControlWindow(HINSTANCE hInstance, std::wstring wS_ApplicationName, E_WINDOW_MODE_STYLE eWindowModeStyle, int &iScreenWidth, int &iScreenHeight);
		/** ������ ���� �Լ� **/
		void DestroyControlWindow();
		/** ������ â��� ��ȯ �Լ� **/
		void SetWindowedMode();
		/** ������ ��üâ ��� ��ȯ �Լ� **/
		void SetWindowedFullScreenMode();
		/** ���ӿ� PROC �Լ� **/
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

/** ������ PROC �Լ� **/
extern LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);