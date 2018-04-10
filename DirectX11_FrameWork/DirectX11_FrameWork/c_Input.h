#pragma once
#include <bitset>
#define KEYMAX 256

class c_Input
{
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

public:
	c_Input();
	~c_Input();
};

