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

#include <TINYSTL/allocator.h>
#include <TINYSTL/new.h>
#include <TINYSTL/stddef.h>

namespace tinystl {

	template<typename T, typename Alloc = allocator>
	class vector
	{
	public:
		vector();
		vector(const vector& other);
		vector(size_t size);
		vector(size_t size, const T& value);
		vector(const T* first, const T* last);
		~vector();

		vector& operator=(const vector& other);

		void assign(const T* first, const T* last);

		const T* data() const;
		T* data();
		size_t size() const;
		bool empty() const;

		T& operator[](size_t idx);
		const T& operator[](size_t idx) const;

		const T& back() const;
		T& back();

		void resize(size_t size);
		void resize(size_t size, const T& value);
		void clear();
		void reserve(size_t capacity);

		void push_back(const T& t);
		void pop_back();

		void swap(vector& other);

		typedef T value_type;

		typedef T* iterator;
		iterator begin();
		iterator end();

		typedef const T* const_iterator;
		const_iterator begin() const;
		const_iterator end() const;

		void insert(iterator where, const T& value);
		void insert(iterator where, const T* first, const T* last);

		iterator erase(iterator where);
		iterator erase(iterator first, iterator last);

	private:
		typedef T* pointer;

		pointer m_first;
		pointer m_last;
		pointer m_capacity;
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
		insert(m_last, other.m_first, other.m_last);
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>::vector(size_t size)
		: m_first(0)
		, m_last(0)
		, m_capacity(0)
	{
		resize(size);
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>::vector(size_t size, const T& value)
		: m_first(0)
		, m_last(0)
		, m_capacity(0)
	{
		reserve(size);
		for (pointer it = m_last, end = m_last + size; it != end; ++it)
			new(placeholder(), it) T(value);
		m_last += size;
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>::vector(const T* first, const T* last)
		: m_first(0)
		, m_last(0)
		, m_capacity(0)
	{
		insert(m_last, first, last);
	}

	template<typename T>
	static inline void destroy_range(T* first, T* last)
	{
		for (; first < last; ++first)
			first->~T();
	}

	template<typename T>
	static inline void destroy_range<>(T** first, T** last)
	{
	}


	template<typename T, typename Alloc>
	inline vector<T, Alloc>::~vector()
	{
		destroy_range(m_first, m_last);
		Alloc::static_deallocate(m_first, (size_t)((char*)m_capacity - (char*)m_first));
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& other)
	{
		vector(other).swap(*this);
		return *this;
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::assign(const T* first, const T* last)
	{
		clear();
		insert(m_last, first, last);
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
	bool vector<T, Alloc>::empty() const
	{
		return m_last == m_first;
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
	const T& vector<T, Alloc>::back() const
	{
		return m_last[-1];
	}

	template<typename T, typename Alloc>
	T& vector<T, Alloc>::back()
	{
		return m_last[-1];
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::resize(size_t size)
	{
		resize(size, T());
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::resize(size_t size, const T& value)
	{
		reserve(size);

		for (pointer it = m_last, end = m_first + size; it < end; ++it)
			new(placeholder(), it) T(value);
		destroy_range(m_first + size, m_last);

		m_last = m_first + size;
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::clear()
	{
		destroy_range(m_first, m_last);
		m_last = m_first;
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::reserve(size_t capacity)
	{
		if (m_first + capacity <= m_capacity)
			return;

		const size_t size = (size_t)(m_last - m_first);
		const size_t current = (size_t)(m_capacity - m_first);

		pointer newfirst = (T*)Alloc::static_allocate(sizeof(T) * capacity);
		for (pointer it = m_first, newit = newfirst, end = m_last; it != end; ++it, ++newit)
			new(placeholder(), newit) T(*it);
		destroy_range(m_first, m_last);

		Alloc::static_deallocate(m_first, sizeof(T) * current);

		m_first = newfirst;
		m_last = newfirst + size;
		m_capacity = m_first + capacity;
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::push_back(const T& t)
	{
		insert(m_last, &t, &t + 1);
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::pop_back()
	{
		if (m_last != m_first)
		{
			--m_last;
			destroy_range(m_last, m_last + 1);
		}
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::swap(vector& other)
	{
		const pointer tfirst = m_first, tlast = m_last, tcapacity = m_capacity;
		m_first = other.m_first, m_last = other.m_last, m_capacity = other.m_capacity;
		other.m_first = tfirst, other.m_last = tlast, other.m_capacity = tcapacity;
	}

	template<typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T,Alloc>::begin()
	{
		return m_first;
	}

	template<typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T,Alloc>::end()
	{
		return m_last;
	}

	template<typename T, typename Alloc>
	typename vector<T, Alloc>::const_iterator vector<T,Alloc>::begin() const
	{
		return m_first;
	}

	template<typename T, typename Alloc>
	typename vector<T, Alloc>::const_iterator vector<T,Alloc>::end() const
	{
		return m_last;
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::insert(iterator where, const T& value)
	{
		insert(where, &value, &value + 1);
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::insert(iterator where, const T* first, const T* last)
	{
		const size_t offset = (size_t)(where - m_first);
		const size_t newsize = (size_t)((m_last - m_first) + (last - first));
		if (m_first + newsize > m_capacity)
			reserve((newsize * 3) / 2);

		where = m_first + offset;
		for (pointer it = m_last, end = where, dest = m_last + (size_t)(last - first); it != end; --it, --dest)
		{
			new(placeholder(), dest - 1) T(*(it - 1));
			destroy_range(it - 1, it);
		}

		for ( ; first != last; ++first, ++m_last, ++where)
			new(placeholder(), where) T(*first);
	}

	template<typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator where)
	{
		return erase(where, where + 1);
	}

	template<typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last)
	{
		for (pointer dest = first, it = last, end = m_last; it != end; ++it, ++dest)
		{
			destroy_range(dest, dest + 1);
			new(placeholder(), dest) T(*it);
			destroy_range(it, it + 1);
		}

		m_last -= (last - first);
		return first;
	}
}

#endif 
