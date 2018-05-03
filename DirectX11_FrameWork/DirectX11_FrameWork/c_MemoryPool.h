#pragma once


namespace GameEngine
{
	class c_MemoryPool
	{
	public:
		c_MemoryPool();
		c_MemoryPool(const char* name, void* original, void* aligned);
		~c_MemoryPool();

		unsigned int GetPoolID();
		void* GetOriginalMemoryStart();
		c_MemoryPool* GetNextPool();

	private:
		void* OriginalMemoryPointer;
		void* AlignedMemoryPointer;
		unsigned int NameHash;
	};
}
