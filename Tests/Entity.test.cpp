/*
 * Entity.test.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "elemental/Entity.hpp"
#include "test-utils/common.hpp"

BEGIN_TEST_SUITE("Entity.test.cpp")
{
	struct TestFixture {
		std::string type = "player", layer = "main";
		std::vector<int> position = { 0, 0 }, size = { 10, 10 };

		virtual ~TestFixture() = default;

		elemental::Entity object;
	};

	FIXTURE_TEST("Test") {}
}

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
