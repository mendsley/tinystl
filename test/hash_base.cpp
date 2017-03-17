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

#include <TINYSTL/hash_base.h>
#include <TINYSTL/string.h>
#include <UnitTest++.h>
#include <utility>

TEST(pair_construct) {
	typedef tinystl::pair<int, int> pair;
	using tinystl::string;

	{
		pair p(1, 2);
		CHECK( p.first == 1 );
		CHECK( p.second == 2 );
	}

	{
		const pair other(1, 2);
		pair p = other;

		CHECK( p.first == 1 );
		CHECK( p.second == 2 );
	}

	{
		tinystl::pair<string, string> other("hello", "world");
		tinystl::pair<string, string> p = std::move(other);

		CHECK( p.first == string("hello") );
		CHECK( p.second == string("world") );
		CHECK( 0 == other.first.size() );
		CHECK( 0 == other.second.size() );
	}

	{
		string key = "hello";
		string value = "world";
		tinystl::pair<string, string> p(std::move(key), std::move(value));

		CHECK( p.first == string("hello") );
		CHECK( p.second == string("world") );
		CHECK( key.size() == 0 );
		CHECK( value.size() == 0 );
	}
}

TEST(pair_assign) {

	{
		typedef tinystl::pair<int, int> pair;

		const pair baseline(1, 2);
		pair p(3, 4);
		p = baseline;

		CHECK( p.first == baseline.first );
		CHECK( p.second == baseline.second );
	}

	{
		using tinystl::string;
		typedef tinystl::pair<string, string> pair;

		const pair baseline("hello", "world");
		pair other = baseline;
		pair p("not", "valid");
		p = std::move(other);

		CHECK( p.first == baseline.first );
		CHECK( p.second == baseline.second );
		CHECK( other.first.size() == 0 );
		CHECK( other.second.size() == 0 );
	}

}

TEST(pair_make) {
	typedef tinystl::pair<int, int> pair;
	using tinystl::make_pair;
	using tinystl::string;

	{
		pair p = make_pair(1, 2);
		CHECK( p.first == 1 );
		CHECK( p.second == 2 );
	}

	{
		const pair other = make_pair(1, 2);
		pair p = other;

		CHECK( p.first == 1 );
		CHECK( p.second == 2 );
	}

	{
		tinystl::pair<string, string> other = make_pair(string("hello"), string("world"));
		tinystl::pair<string, string> p = std::move(other);

		CHECK( p.first == string("hello") );
		CHECK( p.second == string("world") );
		CHECK( 0 == other.first.size() );
		CHECK( 0 == other.second.size() );
	}

	{
		string key = "hello";
		string value = "world";
		tinystl::pair<string, string> p = make_pair(std::move(key), std::move(value));

		CHECK( p.first == string("hello") );
		CHECK( p.second == string("world") );
		CHECK( key.size() == 0 );
		CHECK( value.size() == 0 );
	}
}
