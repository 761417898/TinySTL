#ifndef ALLOC_H

#include <cstdlib> //malloc
#include <new> //std::bad_alloc

namespace tinystl {

	enum EAlign { ALIGN = 8 };
	enum EMaxBytes { MAXBYTES = 128 }; //大于这个值直接malloc
	enum ENFreeLists { NFREELISTS = (EMaxBytes::MAXBYTES / EAlign::ALIGN) };
	enum ENBlocks { NBLOCKS = 20 }; //每次要申请增加的节点数

	union Obj {
		union Obj *next;
		char data[1];
	};

	class alloc {
	private:
		//free_list链表节点
		static Obj *free_list[ENFreeLists::NFREELISTS]; //空闲快链表

		static char *start_free;
		static char *end_free;
		static size_t heap_size;
	private:
		static size_t round_up(size_t bytes);
		static size_t free_list_index(size_t bytes);
		//从内存池中获取一段内存，被划分为若干个大小为n的内存块，返回其中一块，剩下的加入到free_list中
		static void *refill(size_t n);
		//从内存池中分配nblocks个size内存块，nbblocks最后被指定为实际分配的个数
		static char *chunk_alloc(size_t size, size_t &nblocks);
	public:
		static void *allocate(size_t bytes);
		static void deallocate(void *ptr, size_t bytes);
		static void *reallocate(void *ptr, size_t oldsize, size_t newsize);
	};

	//静态成员初始化
	char *alloc::start_free = nullptr;
	char *alloc::end_free = nullptr;
	size_t alloc::heap_size = 0;

	Obj *alloc::free_list[ENFreeLists::NFREELISTS] = {
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr
	};

	size_t alloc::round_up(size_t bytes) {
		return (bytes + EAlign::ALIGN - 1) / EAlign::ALIGN * EAlign::ALIGN;
	}

	size_t alloc::free_list_index(size_t bytes) {
		//bytes != 0
		return alloc::round_up(bytes) / EAlign::ALIGN - 1;
	}

	void *alloc::refill(size_t n) {
		size_t nblocks = ENBlocks::NBLOCKS;
		char *chunk = alloc::chunk_alloc(n, nblocks);
		if (nblocks == 1) {
			return chunk;
		}
		//多余的节点重新插入到free_list中
		Obj* ret = (Obj*)chunk;
		Obj* cur = (Obj*)(chunk + n);
		Obj* next = nullptr;
		//找到大小对应的链表
		Obj* volatile *my_free_list = free_list + free_list_index(n);
		*my_free_list = cur; //头结点
		for (size_t idx = 1; idx < nblocks; ++idx) {
			next = (Obj*)((char*)cur + n);
			cur->next = next;
			cur = cur->next;
		}
		cur->next = nullptr;
		return ret;
	}

	char *alloc::chunk_alloc(size_t size, size_t &nblocks) {
		size_t left_bytes = end_free - start_free;
		size_t need_bytes = size * nblocks;
		char *ret = nullptr;
		if (left_bytes >= need_bytes) {
			ret = start_free;
			start_free += need_bytes;
			return ret;
		}
		else if (left_bytes >= size) {
			nblocks = left_bytes / size;
			ret = start_free;
			start_free += size;
			return ret;
		}
		else {
			//内存池中剩余的内存挂在free_list上
			if (left_bytes > 0) {
				size_t index = free_list_index(left_bytes);
				((Obj*)start_free)->next = free_list[index];
				free_list[index] = (Obj*)start_free;
			}
			//std::malloc申请内存
			size_t bytes_to_get = (need_bytes << 1) + round_up(heap_size >> 4);
			start_free = (char *)malloc(bytes_to_get);
			//申请失败，去free_list中查找有无更大的内存块,若存在，将此快放入内存池，递归调用chunk_alloc
			if (!start_free) {
				for (size_t idx = size; idx <= EMaxBytes::MAXBYTES; idx += EAlign::ALIGN) {
					size_t index = free_list_index(idx);
					Obj *volatile *my_free_list = free_list + index;
					Obj *ptr = *my_free_list;
					if (ptr) {
						*my_free_list = ptr->next;
						start_free = (char*)ptr;
						end_free = start_free + idx;
						return chunk_alloc(size, nblocks);
					}
					end_free = nullptr;
					throw std::bad_alloc();
				}
			}
			end_free = start_free + bytes_to_get;
			heap_size += bytes_to_get;
			return chunk_alloc(size, nblocks);
		}
	}

	void *alloc::allocate(size_t n) {
        if (n == 0) {
            return nullptr;
        }
		if (n > static_cast<size_t>(EMaxBytes::MAXBYTES)) {
			return malloc(n);
		}
		Obj *volatile *my_free_list = free_list + free_list_index(n);
		Obj *ret = *my_free_list;
		if (!ret) {
			void *r = refill(round_up(n));
			return r;
		}
		*my_free_list = ret->next;
		return ret;
	}

	void alloc::deallocate(void *ptr, size_t bytes) {
		if (bytes > static_cast<size_t>(EMaxBytes::MAXBYTES)) {
			free(ptr);
			return;
		}
		Obj *head = free_list[free_list_index(bytes)];
		//reinterpret_cast用在任意指针（或引用）类型之间的转换；以及指针与足够大的整数类型之间的转换
		(reinterpret_cast<Obj*>(ptr))->next = head;
		free_list[free_list_index(bytes)] = reinterpret_cast<Obj*>(ptr);
		return;
	}

	void *alloc::reallocate(void* p, size_t old_size, size_t new_size) {
		deallocate(p, old_size);
		return p = allocate(new_size);
	}
}

#endif

