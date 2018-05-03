#pragma once


class c_Hash
{
public:
	c_Hash();
	~c_Hash();

	unsigned int Hash(const char* key, BYTE Length, const unsigned int Modifier = 0);
	void Mix(unsigned int &a, unsigned int &b, unsigned int &c);
	unsigned int Rotate(unsigned int x, unsigned int k);
	void Final(unsigned int& a, unsigned int& b, unsigned int& c);
};

