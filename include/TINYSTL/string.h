/*-
 * Copyright 2012-1015 Matthew Endsley
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

#ifndef TINYSTL_STRING_H
#define TINYSTL_STRING_H

#include <TINYSTL/allocator.h>
#include <TINYSTL/stddef.h>
#include <TINYSTL/hash.h>

namespace tinystl {

	class string {
	public:
		string();
		string(const string& other);
		string(const char* sz);
		string(const char* sz, size_t len);
		~string();

		string& operator=(const string& other);

		const char* c_str() const;
		size_t size() const;
		size_t capacity() const;

		void reserve(size_t capacity);
		void resize(size_t size);

		void assign(const char* first, const char* last);

		void append(const char* first, const char* last);

		void swap(string& other);

		typedef char* iterator;
		iterator begin();
		iterator end();

		typedef const char* const_iterator;
		const_iterator begin() const;
		const_iterator end() const;

	private:
		typedef char* pointer;
		static const size_t c_nbuffer = 16;
		static const size_t c_longflag = ((size_t)1) << (sizeof(size_t) * 8 - 1);
		size_t m_size;
		union {
			struct {
				pointer m_first;
				pointer m_capacity;
			};
			char m_buffer[c_nbuffer];
		};
	};

	inline string::string()
		: m_size(0)
	{
		m_buffer[0] = 0;
	}

	inline string::string(const string& other)
		: m_size(0)
	{
		assign(other.begin(), other.end());
	}

	inline string::string(const char* sz)
		: m_size(0)
	{
		size_t len = 0;
		for (const char* it = sz; *it; ++it)
			++len;

		assign(sz, sz + len);
	}

	inline string::string(const char* sz, size_t len)
		: m_size(0)
	{
		append(sz, sz + len);
	}

	inline string::~string() {
		if (m_size & c_longflag)
			TINYSTL_ALLOCATOR::static_deallocate(m_first, m_capacity - m_first);
	}

	inline string& string::operator=(const string& other) {
		if (this != &other)
			assign(other.begin(), other.end());
		return *this;
	}

	inline const char* string::c_str() const {
		if (m_size & c_longflag)
			return m_first;
		else
			return m_buffer;
	}

	inline size_t string::size() const {
		return m_size & ~c_longflag;
	}

	inline size_t string::capacity() const {
		if (m_size & c_longflag)
			return m_capacity - m_first - 1;
		else
			return c_nbuffer - 1;
	}

	inline void string::reserve(size_t cap) {
		if (cap <= capacity())
			return;

		pointer newfirst = (pointer)TINYSTL_ALLOCATOR::static_allocate(cap + 1);
		if (m_size & c_longflag) {
			for (pointer it = m_first, newit = newfirst, e = m_first + (m_size & ~c_longflag); it != e; ++it, ++newit)
				*newit = *it;
			TINYSTL_ALLOCATOR::static_deallocate(m_first, m_capacity - m_first);
		} else {
			for (pointer it = m_buffer, newit = newfirst, e = m_buffer + m_size; it != e; ++it, ++newit)
				*newit = *it;
			m_size |= c_longflag;
		}
		m_first = newfirst;
		m_capacity = m_first + cap + 1;
	}

	inline void string::resize(size_t size) {
		reserve(size);
		pointer newend = begin() + size;
		for (pointer it = end(); it != newend; ++it)
			*it = 0;
		*newend = 0;
		m_size = size;
	}

	inline void string::assign(const char* first, const char* last) {
		size_t newsize = last - first;
		reserve(newsize);

		char* newit = begin();
		for (const char* it = first; it != last; ++it, ++newit)
			*newit = *it;
		*newit = 0;
		m_size = newsize | (m_size & c_longflag);
	}

	inline void string::append(const char* first, const char* last) {
		const size_t newsize = (size_t)(size() + (last - first));
		if (newsize > capacity())
			reserve((newsize * 3) / 2);

		char* newit = end();
		for (const char* it = first; it != last; ++it, ++newit)
			*newit = *it;
		*newit = 0;
		m_size = newsize | (m_size & c_longflag);
	}

	inline void string::swap(string& other) {
		size_t tsize = m_size;
		pointer tfirst, tcapacity;
		char tbuffer[c_nbuffer];

		if (tsize & c_longflag) {
			tfirst = m_first;
			tcapacity = m_capacity;
		} else {
			for (pointer it = m_buffer, newit = tbuffer, e = m_buffer + tsize; it != e; ++it, ++newit)
				*newit = *it;
		}

		m_size = other.m_size;
		if (other.m_size & c_longflag) {
			m_first = other.m_first;
			m_capacity = other.m_capacity;
		} else {
			for (pointer it = other.m_buffer, newit = m_buffer, e = other.m_buffer + m_size; it != e; ++it, ++newit)
				*newit = *it;
			m_buffer[m_size] = 0;
		}

        other.m_size = tsize;
        if (tsize & c_longflag) {
			other.m_first = tfirst;
			other.m_capacity = tcapacity;
		} else {
			for (pointer it = tbuffer, newit = other.m_buffer, e = tbuffer + tsize; it != e; ++it, ++newit)
				*newit = *it;
			other.m_buffer[tsize] = 0;
		}
	}

	inline string::iterator string::begin() {
		if (m_size & c_longflag)
			return m_first;
		else
			return m_buffer;
	}

	inline string::iterator string::end() {
		if (m_size & c_longflag)
			return m_first + (m_size & ~c_longflag);
		else
			return m_buffer + m_size;
	}

	inline string::const_iterator string::begin() const {
		if (m_size & c_longflag)
			return m_first;
		else
			return m_buffer;
	}

	inline string::const_iterator string::end() const {
		if (m_size & c_longflag)
			return m_first + (m_size & ~c_longflag);
		else
			return m_buffer + m_size;
	}

	inline bool operator==(const string& lhs, const string& rhs) {
		typedef const char* pointer;

		const size_t lsize = lhs.size(), rsize = rhs.size();
		if (lsize != rsize)
			return false;

		for (pointer lit = lhs.c_str(), rit = rhs.c_str(), lend = lit + lsize; lit != lend; ++lit, ++rit)
			if (*lit != *rit)
				return false;

		return true;
	}

	static inline size_t hash(const string& value) {
		return hash_string(value.c_str(), value.size());
	}
}

#endif
