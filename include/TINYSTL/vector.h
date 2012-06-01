/*-
 * Copyright 2012 Matthew Endsley
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted providing that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TINYSTL_VECTOR_H
#define TINYSTL_VECTOR_H

#include <new>
#include <stddef.h>
#include <TINYSTL/allocator.h>

namespace tinystl {

	template<typename T, typename Alloc = allocator>
	class vector
	{
	public:
		vector();
		vector(const vector& other);
		~vector();

		vector& operator=(const vector& other);

		const T* data() const;
		T* data();
		size_t size() const;

		T& operator[](size_t idx);
		const T& operator[](size_t idx) const;

		void resize(size_t size);
		void reserve(size_t capacity);

		void push_back(const T& t);
		void pop_back();
		void append(const T* first, const T* last);

		void swap(vector& other);

	private:
		typedef T* pointer;

		void grow(size_t size);

		T* m_first;
		T* m_last;
		T* m_capacity;
	};

	template<typename T, typename Alloc>
	vector<T, Alloc>::vector()
		: m_first(0)
		, m_last(0)
		, m_capacity(0)
	{
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>::vector(const vector& other)
		: m_first(0)
		, m_last(0)
		, m_capacity(0)
	{
		reserve(other.size());
		append(other.m_first, other.m_last);
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>::~vector()
	{
		for (pointer it = m_first, end = m_last; it != end; ++it)
		{
			it->~T();
		}

		Alloc::deallocate(m_first, (size_t)((char*)m_capacity - (char*)m_first));
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& other)
	{
		vector(other).swap(*this);
		return *this;
	}

	template<typename T, typename Alloc>
	const T* vector<T, Alloc>::data() const
	{
		return m_first;
	}

	template<typename T, typename Alloc>
	T* vector<T, Alloc>::data()
	{
		return m_first;
	}

	template<typename T, typename Alloc>
	size_t vector<T, Alloc>::size() const
	{
		return (size_t)(m_last - m_first);
	}

	template<typename T, typename Alloc>
	T& vector<T, Alloc>::operator[](size_t idx)
	{
		return m_first[idx];
	}

	template<typename T, typename Alloc>
	const T& vector<T, Alloc>::operator[](size_t idx) const
	{
		return m_first[idx];
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::resize(size_t size)
	{
		reserve(size);

		for (pointer it = m_last, end = m_first + size; it < end; ++it)
			new(it) T();
		for (pointer it = m_first + size, end = m_last; it < end; ++it)
			it->~T();

		m_last = m_first + size;
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::reserve(size_t capacity)
	{
		if (m_first + capacity <= m_capacity)
			return;

		const size_t size = (size_t)(m_last - m_first);
		const size_t current = (size_t)(m_capacity - m_first);

		T* newfirst = (T*)Alloc::allocate(sizeof(T) * capacity);
		for (pointer it = m_first, newit = newfirst, end = m_last; it != end; ++it, ++newit)
			new(newit) T(*it);
		for (pointer it = m_first, end = m_last; it != end; ++it)
			it->~T();

		Alloc::deallocate(m_first, current);

		m_first = newfirst;
		m_last = newfirst + size;
		m_capacity = m_first + capacity;
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::push_back(const T& t)
	{
		append(&t, &t + 1);
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::pop_back()
	{
		const size_t sz = size();
		if (sz)
			resize(sz - 1);
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::append(const T* first, const T* last)
	{
		const size_t count = (size_t)(last - first);
		const size_t newsize = size() + count;
		if (m_first + newsize > m_capacity)
			reserve((newsize * 3) / 2);

		for (pointer it = m_last, end = m_last + count; it != end; ++it, ++first)
			new(it) T(*first);

		m_last += count;
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::swap(vector& other)
	{
		const pointer tfirst = m_first, tlast = m_last, tcapacity = m_capacity;
		m_first = other.m_first, m_last = other.m_last, m_capacity = other.m_capacity;
		other.m_first = tfirst, other.m_last = tlast, other.m_capacity = tcapacity;
	}

}

#endif 
