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

TEST(test_append) {
	tinystl::string s;
	s += "hello";
	s += ' ';
	s += "world";
	CHECK(s == "hello world");
	s += " and this is a very long string";
	CHECK(s == "hello world and this is a very long string");
}

TEST(test_add) {
	CHECK(tinystl::string("hello") + tinystl::string(" world") == "hello world");
	CHECK(tinystl::string("hello") + " world" == "hello world");
	CHECK(tinystl::string("hello") + " " + "world" == "hello world");
	CHECK("hello" + tinystl::string(" ") + "world" == "hello world");
}

TEST(test_insert) {
	tinystl::string s("world");
	s.insert(s.end(), '!');
	CHECK(s == "world!");
	s.insert(s.begin(), "hello");
	CHECK(s == "helloworld!");
	s.insert(s.begin() + 5, " ");
	CHECK(s == "hello world!");
	s.insert(s.end() - 1, ", prepend a huge string to check");
	CHECK(s == "hello world, prepend a huge string to check!");
}

TEST(test_erase) {
	tinystl::string s("hello");
	s.erase(s.begin(), s.end());
	CHECK(s.empty());
	s = "hello";
	s.erase(s.end() - 1, s.end());
	CHECK(s == "hell");
	s = "hello world and this is a very long string";
	s.erase(s.begin(), s.begin() + 4);
	CHECK(s == "o world and this is a very long string");
	s.erase(s.begin(), s.end());
	CHECK(s.empty());
}