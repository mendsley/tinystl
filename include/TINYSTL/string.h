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
		string(const char* first, const char* last);
		string(const char* sz, size_t len);
		~string();

		string& operator=(const string& other);
		string& operator=(char ch);
		string& operator=(const char* sz);

		string& operator+=(const string& other);
		string& operator+=(char ch);
		string& operator+=(const char* sz);

		const char* c_str() const;
		bool empty() const;
		size_t size() const;
		size_t capacity() const;

		typedef char* iterator;
		iterator begin();
		iterator end();

		typedef const char* const_iterator;
		const_iterator begin() const;
		const_iterator end() const;

		char operator[](size_t pos) const;
		char& operator[](size_t pos);

		int compare(const string& other) const;
		int compare(const char* sz) const;

		void reserve(size_t capacity);
		void resize(size_t n);
		void resize(size_t n, char ch);

		void clear();

		void assign(char ch);
		void assign(const char* sz);
		void assign(const char* first, const char* last);
		void assign(const string& other);

		void push_back(char ch);
		void append(const char* sz);
		void append(const char* first, const char* last);
		void append(const string& other);
		void insert(iterator where, char ch);
		void insert(iterator where, const char* sz);
		void insert(iterator where, const char* first, const char* last);
		void insert(iterator where, const string& other);
		void erase(iterator first, iterator last);

		void swap(string& other);

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
		assign(other);
	}

	inline string::string(const char* sz)
		: m_size(0)
	{
		assign(sz);
	}

	inline string::string(const char* first, const char* last)
		: m_size(0)
	{
		assign(first, last);
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
			assign(other);
		return *this;
	}

	inline string& string::operator=(char ch) {
		assign(ch);
		return *this;
	}

	inline string& string::operator=(const char* sz) {
		assign(sz);
		return *this;
	}

	inline string& string::operator+=(const string& other) {
		append(other);
		return *this;
	}

	inline string& string::operator+=(char ch) {
		push_back(ch);
		return *this;
	}

	inline string& string::operator+=(const char* sz) {
		append(sz);
		return *this;
	}

	inline const char* string::c_str() const {
		if (m_size & c_longflag)
			return m_first;
		else
			return m_buffer;
	}

	inline bool string::empty() const {
		return size() == 0;
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

	inline char string::operator[](size_t pos) const {
		return begin()[pos];
	}

	inline char& string::operator[](size_t pos) {
		return begin()[pos];
	}

	inline int string::compare(const string& other) const {
		return compare(other.c_str());
	}

	inline int string::compare(const char* sz) const {
		const char* it = c_str();
		for (; *it && *sz && (*it == *sz); ++it, ++sz);
		return *it - *sz;
	}

	inline void string::reserve(size_t cap) {
		if (cap <= capacity())
			return;

		pointer newfirst = (pointer)TINYSTL_ALLOCATOR::static_allocate(cap + 1);
		for (pointer it = begin(), newit = newfirst, e = end() + 1; it != e; ++it, ++newit)
			*newit = *it;
		if (m_size & c_longflag)
			TINYSTL_ALLOCATOR::static_deallocate(m_first, m_capacity - m_first);
		else
			m_size |= c_longflag;
		m_first = newfirst;
		m_capacity = m_first + cap + 1;
	}

	inline void string::resize(size_t n) {
		resize(n, 0);
	}

	inline void string::resize(size_t n, char ch) {
		if (size() < n) {
			reserve(n);
			for (pointer it = end(), e = begin() + n; it != e; ++it)
				*it = ch;
		}
		pointer it = begin() + n;
		*it = 0;
		m_size = n | (m_size & c_longflag);
	}

	inline void string::clear() {
		resize(0);
	}

	inline void string::assign(char ch) {
		pointer it = begin();
		*it = ch;
		*(it + 1) = 0;
		m_size = 1 | (m_size & c_longflag);
	}

	inline void string::assign(const char* sz) {
		size_t len = 0;
		for (const char *it = sz; *it; ++it)
			++len;

		assign(sz, sz + len);
	}

	inline void string::assign(const char* first, const char* last) {
		size_t newsize = last - first;
		reserve(newsize);

		pointer newit = begin();
		for (const char* it = first; it != last; ++it, ++newit)
			*newit = *it;
		*newit = 0;
		m_size = newsize | (m_size & c_longflag);
	}

	inline void string::assign(const string& other) {
		assign(other.begin(), other.end());
	}

	inline void string::push_back(char ch) {
		if (size() != capacity()) {
			pointer it = end();
			*it = ch;
			*(it + 1) = 0;
			++m_size;
		} else {
			append(&ch, &ch + 1);
		}
	}

	inline void string::append(const char *sz) {
		size_t len = 0;
		for (const char *it = sz; *it; ++it)
			++len;
		append(sz, sz + len);
	}

	inline void string::append(const char* first, const char* last) {
		const size_t newsize = (size_t)(size() + (last - first));
		if (newsize > capacity())
			reserve((newsize * 3) / 2);

		pointer newit = end();
		for (const char* it = first; it != last; ++it, ++newit)
			*newit = *it;
		*newit = 0;
		m_size = newsize | (m_size & c_longflag);
	}

	inline void string::append(const string& other) {
		append(other.begin(), other.end());
	}

	inline void string::insert(iterator where, char ch) {
		insert(where, &ch, &ch + 1);
	}

	inline void string::insert(iterator where, const char* sz) {
		size_t len = 0;
		for (const char *it = sz; *it; ++it)
			++len;
		insert(where, sz, sz + len);
	}

	inline void string::insert(iterator where, const char* first, const char* last) {
		if (first == last)
			return;

		const size_t w = where - begin();
		const size_t newsize = (size_t)(size() + (last - first));
		if (newsize > capacity())
			reserve((newsize * 3) / 2);

		pointer newit = begin() + newsize;
		for (pointer it = end(), e = begin() + w; it >= e; --it, --newit)
			*newit = *it;

		newit = begin() + w;
		for (const char* it = first; it != last; ++it, ++newit)
			*newit = *it;
		m_size = newsize | (m_size & c_longflag);
	}

	inline void string::insert(iterator where, const string& other) {
		insert(where, other.begin(), other.end());
	}

	inline void string::erase(iterator first, iterator last) {
		if (first == last)
			return;

		const size_t newsize = (size_t)(size() + (first - last));
		pointer newit = first;
		for (pointer it = last, e = end(); it != e; ++it, ++newit)
			*newit = *it;
		*newit = 0;
		m_size = newsize | (m_size & c_longflag);
	}

	inline void string::swap(string& other) {
		const size_t tsize = m_size;
		pointer tfirst, tcapacity;
		char tbuffer[c_nbuffer];

		if (tsize & c_longflag) {
			tfirst = m_first;
			tcapacity = m_capacity;
		} else {
			for (pointer it = m_buffer, newit = tbuffer, e = m_buffer + tsize + 1; it != e; ++it, ++newit)
				*newit = *it;
		}

		m_size = other.m_size;
		if (other.m_size & c_longflag) {
			m_first = other.m_first;
			m_capacity = other.m_capacity;
		} else {
			for (pointer it = other.m_buffer, newit = m_buffer, e = other.m_buffer + m_size + 1; it != e; ++it, ++newit)
				*newit = *it;
		}

        other.m_size = tsize;
        if (tsize & c_longflag) {
			other.m_first = tfirst;
			other.m_capacity = tcapacity;
		} else {
			for (pointer it = tbuffer, newit = other.m_buffer, e = tbuffer + tsize + 1; it != e; ++it, ++newit)
				*newit = *it;
		}
	}

	inline string operator+(const string& lhs, const string& rhs) {
		string ret;
		ret.reserve(lhs.size() + rhs.size());
		ret += lhs;
		ret += rhs;
		return ret;
	}

	inline string operator+(const string& lhs, const char* rhs) {
		string ret = lhs;
		ret += rhs;
		return ret;
	}

	inline string operator+(const char* lhs, const string& rhs) {
		string ret = lhs;
		ret += rhs;
		return ret;
	}

	inline bool operator==(const string& lhs, const string& rhs) {
		if (lhs.size() != rhs.size())
			return false;

		return lhs.compare(rhs) == 0;
	}

	inline bool operator==(const string& lhs, const char* rhs) {
		return lhs.compare(rhs) == 0;
	}

	inline bool operator==(const char* lhs, const string& rhs) {
		return rhs.compare(lhs) == 0;
	}

	inline bool operator!=(const string& lhs, const string& rhs) {
		return !(lhs == rhs);
	}

	inline bool operator!=(const string& lhs, const char* rhs) {
		return !(lhs == rhs);
	}

	inline bool operator!=(const char* lhs, const string& rhs) {
		return !(lhs == rhs);
	}

	inline bool operator<(const string& lhs, const string& rhs) {
		return lhs.compare(rhs) < 0;
	}

	inline bool operator<(const string& lhs, const char* rhs) {
		return lhs.compare(rhs) < 0;
	}

	inline bool operator<(const char* lhs, const string& rhs) {
		return rhs.compare(lhs) > 0;
	}

	inline bool operator>(const string& lhs, const string& rhs) {
		return rhs < lhs;
	}

	inline bool operator>(const string& lhs, const char* rhs) {
		return rhs < lhs;
	}

	inline bool operator>(const char* lhs, const string& rhs) {
		return rhs < lhs;
	}

	inline bool operator<=(const string& lhs, const string& rhs) {
		return !(rhs < lhs);
	}

	inline bool operator<=(const string& lhs, const char* rhs) {
		return !(rhs < lhs);
	}

	inline bool operator<=(const char* lhs, const string& rhs) {
		return !(rhs < lhs);
	}

	inline bool operator>=(const string& lhs, const string& rhs) {
		return !(lhs < rhs);
	}

	inline bool operator>=(const string& lhs, const char* rhs) {
		return !(lhs < rhs);
	}

	inline bool operator>=(const char* lhs, const string& rhs) {
		return !(lhs < rhs);
	}

	static inline size_t hash(const string& value) {
		return hash_string(value.c_str(), value.size());
	}
}

#endif
