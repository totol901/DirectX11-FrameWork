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
	// ��� Ű���� ������ ���� ���·� �ʱ�ȭ�մϴ�. 
	m_keys.reset();
} 

void InputManager::KeyDown(unsigned int input)
{ 
	// Ű�� ���ȴٸ� �� ���¸� �迭�� �����մϴ�.
	m_keys.set(input, true);
} 

void InputManager::KeyUp(unsigned int input)
{ 
	// Ű�� �������ٸ� �� ���¸� �迭�� �����մϴ�. 
	m_keys.set(input, false);
} 

bool InputManager::IsKeyDown(unsigned int key)
{ 
	// ���� Ű�� ����/�� �������� ��ȯ�մϴ�. 
	return m_keys[key]; 
}


