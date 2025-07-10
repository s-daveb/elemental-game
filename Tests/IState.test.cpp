/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "elemental/IState.hpp"
#include "elemental/Observable.hpp"

#include "test-utils/common.hpp"

using namespace elemental;
using namespace fakeit;

using DrawCommandPtr = std::shared_ptr<IDrawCommand>;

BEGIN_TEST_SUITE("IState")
{
	class TestState : public elemental::IState {
	    public:
		void recieveMessage(
		    const Observable& sender, std::any message
		) override
		{
		}

		void step() override {}

		auto getDrawCommands() -> std::list<DrawCommandPtr> override
		{
			return {};
		}
	};
	struct TestFixture {
		TestFixture()
		    : fixture(this)
		    , state_under_test(std::make_shared<TestState>())
		{
		}
		~TestFixture() {}

		TestFixture* fixture;
		std::shared_ptr<IState> state_under_test;
	};

	FIXTURE_TEST("Can create and verify IState-derived object")
	{
		auto& state = fixture->state_under_test;

		REQUIRE(state);
	}

	TEST_WITH_FIXTURE(TestFixture, "step() can be called without error")
	{
		auto& state = fixture->state_under_test;

		state->step();
	}

	TEST_WITH_FIXTURE(TestFixture, "recieveMessage() handles messages")
	{
		// Given
		auto& state = fixture->state_under_test;
		std::any message;

		Mock<Observable> mock_sender;
		auto& sender = mock_sender.get();

		REQUIRE_NOTHROW([&]() {
			state->recieveMessage(sender, message);
		}());
	}

	TEST_WITH_FIXTURE(
	    TestFixture, "getDrawCommands() returns empty list by default"
	)
	{
		auto& state = fixture->state_under_test;
		auto commands = state->getDrawCommands();

		REQUIRE(commands.empty());
	}
}

// clang-format off
// vim: set  textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
