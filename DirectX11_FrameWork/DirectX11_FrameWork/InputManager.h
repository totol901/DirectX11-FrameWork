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
	std::bitset<KEYMAX> m_keys; //Ű �Է� ���� �Ǻ��� bitset

public:
	/** Input �ʱ�ȭ �Լ� **/
	void Initialize(); 
	/** Input Ű �ٿ� �Լ� **/
	void KeyDown(unsigned int); 
	/** Input Ű �� �Լ� **/
	void KeyUp(unsigned int); 
	/** Input Ű �ٿ� �Ǻ� �Լ� **/
	bool IsKeyDown(unsigned int); 


};

