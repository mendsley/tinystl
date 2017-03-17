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

TEST(test_reserve) {
	tinystl::string s;
	s.reserve(0);
	CHECK(s.capacity() == 15);
	s.reserve(10);
	s = "short";
	CHECK(s.capacity() == 15);
	CHECK(s == "short");
	s.reserve(15);
	CHECK(s.capacity() == 15);
	CHECK(s == "short");
	s.reserve(100);
	CHECK(s.capacity() == 100);
	CHECK(s == "short");
	s.reserve(101);
	CHECK(s.capacity() == 101);
	CHECK(s == "short");
}

TEST(test_resize) {
	tinystl::string s;
	s.resize(1, ' ');
	CHECK(s == " ");
	s.resize(16, '+');
	CHECK(s == " +++++++++++++++");
	s.clear();
	s.resize(16, '@');
	CHECK(s == "@@@@@@@@@@@@@@@@");
	s.resize(12, '-');
	CHECK(s == "@@@@@@@@@@@@");
}