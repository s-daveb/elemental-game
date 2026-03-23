/* FontConfig.test.cpp
 * Copyright © 2020-2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "test-utils/common.hpp"

#include "elemental/FontConfig.hpp"

BEGIN_TEST_SUITE("FontConfig")
{
	using elemental::FontConfig;

	TEST_CASE("getInstance")
	{
		auto& instance        = FontConfig::getInstance();
		auto& second_instance = FontConfig::getInstance();
		REQUIRE(&instance == &second_instance);
	}

	struct TestFixture
	{
		TestFixture() : test_subject{ FontConfig::getInstance() } {}

		FontConfig& test_subject;
	};

	/// \TODO figure out why this isnt working in FreeBSD and Linux!
#if defined(CI_BUILD) && !(defined(__FreeBSD__) || (defined(__linux__)))
	FIXTURE_TEST("getFont")
	{ REQUIRE(test_subject.getFont("") != ""); }
#endif
}

// clang-format off
// vim: set  textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
