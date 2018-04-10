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
	// 모든 키들을 눌리지 않은 상태로 초기화합니다. 
	m_keys.reset();
} 

void c_Input::KeyDown(unsigned int input)
{ 
	// 키가 눌렸다면 그 상태를 배열에 저장합니다.
	m_keys.set(input, true);
} 

void c_Input::KeyUp(unsigned int input)
{ 
	// 키가 떼어졌다면 그 상태를 배열에 저장합니다. 
	m_keys.set(input, false);
} 

bool c_Input::IsKeyDown(unsigned int key)
{ 
	// 현재 키가 눌림/뗌 상태인지 반환합니다. 
	return m_keys[key]; 
}


