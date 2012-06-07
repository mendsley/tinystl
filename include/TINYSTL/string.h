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

#ifndef TINYSTL_STRING_H
#define TINYSTL_STRING_H

#include <TINYSTL/stddef.h>
#include <TINYSTL/vector.h>

namespace tinystl {

	class string
	{
	public:
		string();
		string(const char* sz);
		string(const char* sz, size_t len);

		const char* c_str() const;
		size_t size() const;

		void reserve(size_t size);
		void resize(size_t size);

		void append(const char* first, const char* last);

	private:
		vector<char> m_buffer;
	};

	inline string::string()
	{
		resize(0);
	}

	inline string::string(const char* sz)
	{
		size_t len = 0;
		for (const char* it = sz; *it; ++it)
			++len;

		reserve(len);
		append(sz, sz + len);
	}

	inline string::string(const char* sz, size_t len)
	{
		reserve(len);
		append(sz, sz + len);
	}

	inline const char* string::c_str() const
	{
		return m_buffer.data();
	}

	inline size_t string::size() const
	{
		return m_buffer.size() - 1;
	}

	inline void string::reserve(size_t size)
	{
		m_buffer.reserve(size + 1);
	}

	inline void string::resize(size_t size)
	{
		m_buffer.resize(size);
		m_buffer.push_back(0);
	}

	inline void string::append(const char* first, const char* last)
	{
		m_buffer.pop_back();
		m_buffer.append(first, last);
		m_buffer.push_back(0);
	}

	inline bool operator==(const string& lhs, const string& rhs)
	{
		typedef const char* pointer;

		const size_t lsize = lhs.size(), rsize = rhs.size();
		if (lsize != rsize)
			return false;

		pointer lit = lhs.c_str(), rit = rhs.c_str();
		pointer lend = lit + lsize;
		while (lit != lend)
		{
			if (*lit++ != *rit++)
				return false;
		}

		return true;
	}
}

#endif
