/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v2.0 (MPL2)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "util/testing.hpp"
#include "test-utils/common.hpp"

#include <array>
#include <cstring>
#include <string>
#include <tuple>

using namespace std;
using namespace elemental;

namespace NS = elemental;

struct BlackBox {
	BlackBox(const char* in_data) { std::strcpy(this->data, in_data); }
	~BlackBox() = default;

    private:
	char data[256];
	TEST_INSPECTABLE(BlackBox);
};

template<>
struct NS::debug::Inspector<BlackBox> {
	Inspector<BlackBox>(BlackBox& subject) : impl(&subject) {}
	using data_t = std::tuple<char*, size_t>;
	auto getData() -> data_t
	{
		return { impl->data, strlen(impl->data) };
	};
	BlackBox* impl;
};

BEGIN_TEST_SUITE("testing-utils")
{
	TEST_CASE("Inspector can read private values")
	{
		namespace debug = NS::debug;
		BlackBox hidden_data(std::string("Hello World").c_str());
		debug::Inspector<BlackBox> data_viewer(hidden_data);

		auto boxed_data = data_viewer.getData();
		REQUIRE(
		    std::string(std::get<0>(boxed_data)) ==
		    std::string("Hello World")
		);
	}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
