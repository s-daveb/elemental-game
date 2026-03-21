/*
 * Entity.test.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "test-utils/common.hpp"

#include "elemental/Entity.hpp"

BEGIN_TEST_SUITE("Entity.test.cpp")
{
	struct TestEntityType : public elemental::Entity
	{
		TestEntityType()
		    : Entity("Test", { 10, 10 }, { 16, 16 }, "sprite")
		{
		}

		virtual ~TestEntityType() = default;
	};

	struct TestFixture
	{
		std::string      type = "player", layer = "main";
		std::vector<int> position = { 0, 0 }, size = { 10, 10 };

		TestEntityType object;

		TestFixture() : object() {}
		virtual ~TestFixture() = default;
	};

	FIXTURE_TEST("Test") {}
}

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
