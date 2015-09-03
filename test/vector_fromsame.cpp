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

#include <TINYSTL/vector.h>
#include <UnitTest++.h>

struct zero_on_destroy {
	zero_on_destroy() { m_data = 1; filler = 12345; }
	zero_on_destroy(int data) { m_data = data; }
	~zero_on_destroy() { m_data = 0; filler = -12345; }
	zero_on_destroy(const zero_on_destroy& other) {
		m_data = other.m_data;
	}

	int filler;
	int m_data;
};


TEST(vector_push_back_from_same) {
	tinystl::vector<zero_on_destroy> v;
	v.reserve(1);
	v.emplace_back(123);
	v.push_back(v.front());
	CHECK( v.back().m_data == 123 );
}

TEST(vector_emplace_back_from_same) {
	tinystl::vector<zero_on_destroy> v;
	v.reserve(1);
	v.emplace_back(123);
	v.emplace_back(v.front().m_data);
	CHECK( v.back().m_data == 123 );
}

TEST(vector_insert_from_same) {
	tinystl::vector<zero_on_destroy> v;
	v.reserve(10);
	v.emplace_back(123);
	v.insert(v.end(), v.front());
	v.emplace_back(124);
	v.insert(v.begin(), v.back());
	CHECK( v.front().m_data == 124 );
	v.insert(v.end(), v[1]);
	CHECK( v.back().m_data == 123 );
	v.emplace_back(125);
	v.insert(v.end() - 1, v.back());
	CHECK( v.back().m_data == 125 );
}
