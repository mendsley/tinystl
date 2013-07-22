/*-
 * Copyright 2013 Keith W. Thompson
 * Copyright 2013 Matthew Endsley
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

#include <TINYSTL/algorithm.h>
#include <UnitTest++.h>
#include <algorithm>
#include <string.h>

struct complex {
	complex() {data = 0;}
	complex(const char* s) { if (s) data = strdup(s); else data = 0; }
	~complex() { free(data); }
	complex(const complex& other) { data = 0; if (other.data) data = strdup(other.data); }
	complex& operator=(const complex& other) { complex(other).swap(*this); return *this; }
	void swap(complex& other) { std::swap(data, other.data); }

	char* data;
};

static inline bool operator==(const complex& lhs, const complex& rhs) {
	if (lhs.data == 0 && rhs.data == 0)
		return true;
	if (lhs.data != 0 && rhs.data != 0)
		return 0 == strcmp(lhs.data, rhs.data);
	return false;
}

static inline bool operator<(const complex& lhs, const complex& rhs) {
	if (lhs.data == 0 && rhs.data != 0)
		return true;
	if (lhs.data != 0 && rhs.data != 0)
		return strcmp(lhs.data, rhs.data) < 0;
	return false;
}

static inline bool operator>(const complex& lhs, const complex& rhs) {
	if (lhs.data != 0 && rhs.data == 0)
		return true;
	if (lhs.data != 0 && rhs.data != 0)
		return strcmp(lhs.data, rhs.data) > 0;
	return false;
}

TEST(min) {
	CHECK( tinystl::min(1, 1) == 1 );
	CHECK( tinystl::min(1, 2) == 1 );
	CHECK( tinystl::min(2, 1) == 1 );
	
	CHECK( tinystl::min(3.1415, 9.8) == 3.1415 );
	CHECK( tinystl::min(9.8, 3.1415) == 3.1415 );

	CHECK( tinystl::min('a', 'z') == 'a' );
	CHECK( tinystl::min('z', 'a') == 'a' );

	CHECK( tinystl::min<complex>("aardvark", "aardvark") == "aardvark" );
	CHECK( tinystl::min<complex>("aardvark", "zebra") == "aardvark" );
	CHECK( tinystl::min<complex>("zebra", "aardvark") == "aardvark" );

	CHECK( tinystl::min<complex>(complex(), "zebra") == complex() );
	CHECK( tinystl::min<complex>("zebra", complex()) == complex() );
	CHECK( tinystl::min<complex>(0L, 0L) == complex() );
}

TEST(max) {
	CHECK( tinystl::max(1, 1) == 1 );
	CHECK( tinystl::max(1, 2) == 2 );
	CHECK( tinystl::max(2, 1) == 2 );
	
	CHECK( tinystl::max(3.1415, 9.8) == 9.8 );
	CHECK( tinystl::max(9.8, 3.1415) == 9.8 );
	
	CHECK( tinystl::max('A', 'Z') == 'Z' );
	CHECK( tinystl::max('Z', 'A') == 'Z' );

	CHECK( tinystl::max<complex>("aardvark", "aardvark") == "aardvark" );
	CHECK( tinystl::max<complex>("aardvark", "zebra") == "zebra" );
	CHECK( tinystl::max<complex>("zebra", "aardvark") == "zebra" );

	CHECK( tinystl::max<complex>(complex(), "zebra") == "zebra" );
	CHECK( tinystl::max<complex>("zebra", complex()) == "zebra" );
	CHECK( tinystl::max<complex>(0L, 0L) == complex() );
}
