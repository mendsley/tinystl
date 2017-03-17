/*-
 * Copyright 2012-2015 Matthew Endsley
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

#include <TINYSTL/string.h>
#include <UnitTest++.h>
#include <string.h>
#include <utility>

TEST(string_constructor) {

	using tinystl::string;

	{
		string s;
		CHECK( s.size() == 0 );
	}
	{
		string s("hello");
		CHECK( s.size() == 5 );
		CHECK( 0 == strcmp(s.c_str(), "hello") );
	}
	{
		string s("hello world", 5);
		CHECK( s.size() == 5 );
		CHECK( 0 == strcmp(s.c_str(), "hello") );
	}
	{
		const string other("hello");
		string s = other;

		CHECK( s.size() == 5 );
		CHECK( 0 == strcmp(s.c_str(), "hello") );
	}
	{
		string other("hello");
		string s = std::move(other);

		CHECK( s.size() == 5 );
		CHECK( 0 == strcmp(s.c_str(), "hello") );
		CHECK( other.size() == 0 );
	}
}

TEST(string_assign) {
	using tinystl::string;

	{
		const string other("hello");
		string s("new");
		s = other;

		CHECK( s.size() == 5 );
		CHECK( 0 == strcmp(s.c_str(), "hello") );
	}
	{
		string other("hello");
		string s("new");
		s = std::move(other);

		CHECK( s.size() == 5 );
		CHECK( 0 == strcmp(s.c_str(), "hello") );
		CHECK( other.size() == 0 );
	}
}
