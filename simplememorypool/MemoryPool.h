#pragma once
#include <exception>
#include <iostream>
#include <cstdlib>
template <typename T>
class MemoryPool
{
public:
	MemoryPool(int numObjects) : MaxNumbjects(numObjects), m_Pool(nullptr), m_FreeList(nullptr), m_UsingList(nullptr) {
		m_Pool = std::malloc(MaxNumbjects * sizeof(Block));
		if (m_Pool == nullptr)
		{
			throw std::exception("Fail to create memory pool");
		}
		Block* cur = (Block*)m_Pool;
		for (size_t i = 0; i < MaxNumbjects; i++)
		{
			cur->prev = nullptr;
			cur->next = m_FreeList;
			if (m_FreeList != nullptr)
			{
				m_FreeList->prev = cur;
			}
			m_FreeList = cur;
			++cur;
			std::cout << (int)(cur - m_FreeList->prev) << std::endl;
		}
	}
	~MemoryPool() {
		std::free(m_Pool);
	}
	void* Alloc() {
		if (m_FreeList == nullptr)
		{
			return (void*)std::malloc(sizeof(T));
		}
		else {
			try
			{
				Block* cur = m_FreeList;
				m_FreeList = m_FreeList->next;
				m_FreeList->prev = nullptr;
				cur->next = m_UsingList;
				if (m_UsingList != nullptr)
				{
					m_UsingList->prev = cur;
				}
				m_UsingList = cur;
				return (void*)cur->object;
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				std::cout << "Error occured in Alloc Function." << std::endl;
			}
		}
	}
	void Free(T* object) {
		if (object >= m_Pool && (char*)object < (char*)m_Pool + MaxNumbjects * sizeof(Block))
		{
			try {
				Block* cur = (Block*)( (char*)object - 2 * sizeof(Block*));
				if (cur->prev != nullptr)
				{
					if (cur->next != nullptr)
					{
						(cur->next)->prev = cur->prev;
					}
					(cur->prev)->next = cur->next;
				}
				else {
					m_UsingList = cur->next;
					m_UsingList->prev = nullptr;
				}
				cur->prev = nullptr;
				cur->next = m_FreeList;
				m_FreeList->prev = cur;
				m_FreeList = cur;
			}
			catch (const std::exception& e) {
				std::cout << e.what() << std::endl;
				std::cout << "Error occured in Free Function." << std::endl;
			}

		}
		else {
			std::free(object);
		}
	}
private:
	struct Block {
		Block* prev;
		Block* next;
		char object[sizeof(T)];
	};
	void* m_Pool;
	Block* m_FreeList;
	Block* m_UsingList;
	unsigned int MaxNumbjects;
};