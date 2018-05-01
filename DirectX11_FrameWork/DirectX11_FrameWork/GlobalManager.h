#pragma once
#include "c_ControlWindow.h"
#include "MemoryManager.h"

using namespace GameEngine;

//글로벌(static) 클래스를 관리하는 매니저입니다.
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
		static MemoryManager &GetMemoryManager();

	private:
		static c_ControlWindow sM_ControlWindow;
		static MemoryManager sM_MemooryManager;
	};
};

