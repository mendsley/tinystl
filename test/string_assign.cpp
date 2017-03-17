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

TEST(test_empty) {
	tinystl::string s;
	CHECK(s.empty());
	CHECK(s.capacity() == 15);
	CHECK(s.begin() == s.end());
	CHECK(strlen(s.c_str()) == 0);
	CHECK(s == "");
}

TEST(test_small) {
	tinystl::string s1("");
	CHECK(s1.empty());
	CHECK(s1.capacity() == 15);
	CHECK(s1.begin() == s1.end());
	CHECK(strlen(s1.c_str()) == 0);
	CHECK(s1 == "");

	tinystl::string s2("hello");
	CHECK(s2.size() == 5);
	CHECK(s2.capacity() == 15);
	CHECK(s2.begin() + 5 == s2.end());
	CHECK(strlen(s2.c_str()) == 5);
	CHECK(s2 == "hello");

	tinystl::string s3("exactly 15 char");
	CHECK(s3.size() == 15);
	CHECK(s3.capacity() == 15);
	CHECK(s3.begin() + 15 == s3.end());
	CHECK(strlen(s3.c_str()) == 15);
	CHECK(s3 == "exactly 15 char");
}

TEST(test_long) {
	const char* origin = "very long string larger than large string limit";
	size_t len = strlen(origin);
	tinystl::string s(origin);
	CHECK(s.size() == len);
	CHECK(s.capacity() == len);
	CHECK(s.begin() + len == s.end());
	CHECK(strlen(s.c_str()) == len);
	CHECK(s == origin);
}

TEST(test_assign) {
	tinystl::string s;
	const char* originshort = "short";
	size_t lenshort = strlen(originshort);
	s = originshort;
	CHECK(s.size() == lenshort);
	CHECK(s.capacity() == 15);
	CHECK(s.begin() + lenshort == s.end());
	CHECK(strlen(s.c_str()) == lenshort);
	CHECK(s == originshort);

	const char* originlong = "long long long long long long long long long long long long";
	size_t lenlong = strlen(originlong);
	s = originlong;
	CHECK(s.size() == lenlong);
	CHECK(s.capacity() == lenlong);
	CHECK(s.begin() + lenlong == s.end());
	CHECK(strlen(s.c_str()) == lenlong);
	CHECK(s == originlong);

	s = originshort;
	CHECK(s.size() == lenshort);
	CHECK(s.capacity() == lenlong);
	CHECK(s.begin() + lenshort == s.end());
	CHECK(strlen(s.c_str()) == lenshort);
	CHECK(s == originshort);
}

TEST(test_swap) {
	tinystl::string ss1("short");
	tinystl::string ss2("another");
	tinystl::string sl1("long string for testing purposes");
	tinystl::string sl2("another long string for testing purposes");

	ss1.swap(ss2);
	CHECK(ss1 == "another");
	CHECK(ss2 == "short");

	sl1.swap(sl2);
	CHECK(sl1 == "another long string for testing purposes");
	CHECK(sl2 == "long string for testing purposes");

	ss1.swap(sl2);
	CHECK(ss1 == "long string for testing purposes");
	CHECK(sl2 == "another");

	sl1.swap(ss2);
	CHECK(sl1 == "short");
	CHECK(ss2 == "another long string for testing purposes");
}