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
TEST(test_equal) {
	tinystl::string s("hello");
	CHECK(s == tinystl::string("hello"));
	CHECK(s == "hello");
	CHECK("hello" == s);
	CHECK(s != tinystl::string("hello world"));
	CHECK(s != "hello world");
	CHECK("hello world" != s);
}

TEST(test_ltgt) {
	tinystl::string s("hello");
	CHECK(!(s < "hello"));
	CHECK(s < "helloo");
	CHECK(s < "hello0");
	CHECK(s > "he1");
	CHECK(s > "hell");
	CHECK(s > "a");
	CHECK(s < "z");
	CHECK(s > "aaaaaaaa");
	CHECK(s < "zzzzzzzz");
	CHECK(s > "hella");
	CHECK(s < "hellz");
	CHECK(s < "hellz");
}

TEST(test_lege) {
	tinystl::string s("hello");
	CHECK(s <= "hello");
	CHECK(s >= "hello");
	CHECK(s <= "helloo");
	CHECK(s <= "hello0");
	CHECK(s >= "he1");
	CHECK(s >= "hell");
	CHECK(s >= "a");
	CHECK(s <= "z");
	CHECK(s >= "aaaaaaaa");
	CHECK(s <= "zzzzzzzz");
	CHECK(s >= "hella");
	CHECK(s <= "hellz");
	CHECK(s <= "hellz");
}