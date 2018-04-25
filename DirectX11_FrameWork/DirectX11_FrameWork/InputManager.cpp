#include "stdafx.h"
#include "InputManager.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::Initialize()
{
	// 모든 키들을 눌리지 않은 상태로 초기화합니다. 
	m_keys.reset();
} 

void InputManager::KeyDown(unsigned int input)
{ 
	// 키가 눌렸다면 그 상태를 배열에 저장합니다.
	m_keys.set(input, true);
} 

void InputManager::KeyUp(unsigned int input)
{ 
	// 키가 떼어졌다면 그 상태를 배열에 저장합니다. 
	m_keys.set(input, false);
} 

bool InputManager::IsKeyDown(unsigned int key)
{ 
	// 현재 키가 눌림/뗌 상태인지 반환합니다. 
	return m_keys[key]; 
}


