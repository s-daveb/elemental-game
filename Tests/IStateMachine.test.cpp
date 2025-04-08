/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "elemental/IStateMachine.hpp"
#include "IOCore/types/errors.hpp"

#include "test-utils/common.hpp"

using namespace elemental;
using namespace fakeit;
using namespace IOCore;

BEGIN_TEST_SUITE("IStateMachine")
{
	class TestStateMachine : public IStateMachine {
	    public:
		void step() override {}

		auto draw() -> ErrorFlag override
		{
			return kSuccess; // Implement actual
			                 // drawing logic as needed
		}
	};

	struct TestFixture {
		TestFixture()
		    : fixture(this)
		    , state_under_test(std::make_shared<TestStateMachine>())
		{
		}

		~TestFixture() {}

		TestFixture* fixture;
		std::shared_ptr<IStateMachine> state_under_test;
	};

	FIXTURE_TEST("Can create and verify IStateMachine-derived object")
	{
		auto& state = test_fixture->state_under_test;

		REQUIRE(state);
	}

	FIXTURE_TEST("step() can be called without error")
	{
		auto& state = fixture->state_under_test;

		state->step();
	}

	FIXTURE_TEST("draw() returns success flag by default")
	{
		auto& state = fixture->state_under_test;
		auto result = state->draw();

		REQUIRE(result == kSuccess);
	}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
