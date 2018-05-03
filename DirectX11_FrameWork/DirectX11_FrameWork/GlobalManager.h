#pragma once
#include "c_ControlWindow.h"
#include "MemoryManager.h"
#include "c_Hash.h"

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
		static c_ControlWindow* GetContorlWindow();
		static MemoryManager* GetMemoryManager();
		static c_Hash* GetHash();

	private:
		static c_ControlWindow sM_ControlWindow;
		static MemoryManager sM_MemoryManager;
		static c_Hash sM_Hash;
	};
};

