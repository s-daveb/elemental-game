/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "test-utils/common.hpp"

#include "elemental/IState.hpp"
#include "elemental/Observable.hpp"
#include "elemental/StateStack.hpp"
#include "util/testing.hpp"

#include <algorithm>
#include <functional>

using namespace elemental;
using namespace fakeit;

template<>
struct elemental::debug::Inspector<StateStack> {
	Inspector(StateStack& ref) : stack(ref) {}

	inline auto getStdStack() -> auto& { return stack.stack; }
	StateStack& stack;
};
BEGIN_TEST_SUITE("StateStack")
{
	struct TestFixture : public Observable {
		Mock<IState> mock_state;
		std::unique_ptr<IState> state;
		StateStack stack;
		debug::Inspector<decltype(stack)> inspector;

		TestFixture()
		    : Observable()
		    , mock_state()
		    , state(&mock_state.get())
		    , inspector(stack)
		{
			Fake(Dtor(mock_state));
			Fake(Method(mock_state, step));
			When(Method(mock_state, getDrawCommands)).Return({});
			When(Method(mock_state, recieveMessage)).AlwaysDo(
		}

		virtual ~TestFixture() = default;
	};

	struct MockState : public IState {
		MockState(bool& flag) : IState(), triggered(flag) {}
		~MockState() override = default;

		void recieveMessage(
		    const Observable& sender, std::any message
		) override {};

		void step() override { this->triggered = true; }
		auto getDrawCommands()
		    -> std::list<std::shared_ptr<IDrawCommand>> override
		{
			return {};
		}
		bool& triggered;
	};

	FIXTURE_TEST("Basic State Management")
	{
		auto& real_stack = inspector.getStdStack();
		CHECK(real_stack.size() == 0);

		stack.pushState(state);
		CHECK(real_stack.size() == 1);

		auto& internal_stack = real_stack;

		stack.pop();
		REQUIRE(real_stack.size() == 0);
	}
	FIXTURE_TEST("StateStack::step passes through to underlying state")
	{
		bool triggered = false;

		std::unique_ptr<IState> mock_state_ptr =
		    std::make_unique<MockState>(triggered);
		stack.pushState(mock_state_ptr);

		CHECK(!triggered);
		stack.step();
		REQUIRE(triggered);
	}
	FIXTURE_TEST(
	    "StateStack:receiveMessage passes notification to child"
	)
	{
		bool triggered = false;
		Mock<Observable> mock_observable;

		std::unique_ptr<IState> mock_state_ptr =
		    std::make_unique<MockState>(triggered);
		stack.pushState(mock_state_ptr);

		CHECK(!triggered);
		stack.recieveMessage(*this, "Test");
		REQUIRE(triggered);
	}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
