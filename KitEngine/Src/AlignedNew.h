#pragma once

#include <malloc.h>
#include <exception>

namespace kit {

	template<typename TDerived>
	struct AlignedNew {
		static void* operator new (size_t _size) {
			const size_t alignment = __alignof(TDerived);

			static_assert(alignment > 8, "AlignedNew is only useful for types with > 8 byte alignment.");

			void* ptr = _aligned_malloc(_size, alignment);

			if (!ptr){	throw std::bad_alloc();	}

			return ptr;
		}

		static void operator delete (void* _ptr) {
			_aligned_free(_ptr);
		}

		static void* operator new[](size_t _size) {
			return operator new(_size);
		}

		static void operator delete[](void* _ptr) {
			operator delete(_ptr);
		}

	};
}