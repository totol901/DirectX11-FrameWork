#pragma once
#include "c_ControlWindow.h"

using namespace GameEngine;

//�۷ι�(static) Ŭ������ �����ϴ� �Ŵ����Դϴ�.
namespace GameEngine
{
	class GlobalManager
	{
	public:
		GlobalManager() = delete;
		GlobalManager(const GlobalManager &) = delete;
		GlobalManager(const GlobalManager &&) = delete;
		~GlobalManager() = delete;
		GlobalManager& operator=(const GlobalManager &) = delete;
		GlobalManager& operator= (GlobalManager &&) = delete;

	public:
		static c_ControlWindow &GetContorlWindow();

	private:
		static c_ControlWindow sM_ControlWindow;

	};
};

