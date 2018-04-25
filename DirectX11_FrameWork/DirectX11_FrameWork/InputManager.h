#pragma once
#include "singletonBase.h"
#include <bitset>
#define KEYMAX 256

class InputManager : public singletonBase<InputManager>
{
public:
	InputManager();
	~InputManager();

private:
	std::bitset<KEYMAX> m_keys; //키 입력 유무 판별할 bitset

public:
	/** Input 초기화 함수 **/
	void Initialize(); 
	/** Input 키 다운 함수 **/
	void KeyDown(unsigned int); 
	/** Input 키 업 함수 **/
	void KeyUp(unsigned int); 
	/** Input 키 다운 판별 함수 **/
	bool IsKeyDown(unsigned int); 


};

