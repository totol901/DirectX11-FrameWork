#include "stdafx.h"
#include "c_Input.h"

c_Input::c_Input()
{
}

c_Input::~c_Input()
{
}

void c_Input::Initialize()
{
	// ��� Ű���� ������ ���� ���·� �ʱ�ȭ�մϴ�. 
	m_keys.reset();
} 

void c_Input::KeyDown(unsigned int input)
{ 
	// Ű�� ���ȴٸ� �� ���¸� �迭�� �����մϴ�.
	m_keys.set(input, true);
} 

void c_Input::KeyUp(unsigned int input)
{ 
	// Ű�� �������ٸ� �� ���¸� �迭�� �����մϴ�. 
	m_keys.set(input, false);
} 

bool c_Input::IsKeyDown(unsigned int key)
{ 
	// ���� Ű�� ����/�� �������� ��ȯ�մϴ�. 
	return m_keys[key]; 
}


