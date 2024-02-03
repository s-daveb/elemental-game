/*SdlEventSource.test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "SdlEventSource.hpp"
#include "IObserver.hpp"

#include "test-utils/SdlHelpers.hpp"
#include "test-utils/common.hpp"

#include <SDL.h>
#include <random>

BEGIN_TEST_SUITE("elemental::SdlEventSource")
{
	using namespace elemental;

	class EventRecorder : public IObserver
	{
	  public:
		EventRecorder() : IObserver() {}

		virtual void RecieveMessage(const Observable& sender,
		                            std::any message)
		{
			auto event = std::any_cast<SDL_Event&>(message);
			received.push_back(event);
		}
		virtual ~EventRecorder() = default;
		std::vector<SDL_Event> received;
	};
	class SdlEventSourceInspector : public SdlEventSource
	{
	  public:
		static std::queue<SDL_Event>& GetEventQueue(
		    SdlEventSource& other)
		{
			return other.event_queue;
		}
	}; // #endregion
	using Inspector = SdlEventSourceInspector;

	struct SdlEventSourceFixture : public SdlTestFixture
	{
		SdlEventSourceFixture()
		    : SdlTestFixture()
		    , test_object(Singleton::GetReference<SdlEventSource>())
		    , recorder()
		    , event_queue_ref(Inspector::GetEventQueue(test_object))
		    , dev_rand()
		{
			/* Clear the event queue in between tests */
			while (!event_queue_ref.empty()) {
				event_queue_ref.pop();
			}
		}

		~SdlEventSourceFixture() {}
		SdlEventSource& test_object;
		EventRecorder recorder;
		std::queue<SDL_Event>& event_queue_ref;

		std::random_device dev_rand;
	};
	using TestFixture = SdlEventSourceFixture;

	FIXTURE_TEST("elemental::SdlEventSource::Initialization validation")
	{
		REQUIRE_NOTHROW(test_object.InitJoystick());
		REQUIRE(event_queue_ref.size() == 0);
	}
	FIXTURE_TEST(
	    "elemental::SdlEventSource::Enqueue stores events in order")
	{
		const int MAX_EVENTS = 10;
		SDL_Event test_input_list[MAX_EVENTS];

		unsigned int rand_count =
		    std::uniform_int_distribution(5, MAX_EVENTS)(dev_rand);

		for (unsigned i = 0; i < rand_count; ++i) {
			auto& input = test_input_list[i];
			input = SdlEventSimulator::randomArrowKey();
			event_queue_ref.push(input);
		}

		REQUIRE(event_queue_ref.size() == rand_count);

		for (unsigned i = 0; i < rand_count; ++i) {
			auto event = event_queue_ref.front();
			event_queue_ref.pop();

			REQUIRE(event.key.keysym.sym ==
			        test_input_list[i].key.keysym.sym);
		}
	}

	FIXTURE_TEST(
	    "elemental::SdlEventSource::Notify sends out events in order")
	{
		const int MAX_EVENTS = 5;
		SDL_Event test_input[MAX_EVENTS];

		unsigned int rand_count =
		    std::uniform_int_distribution(5, MAX_EVENTS)(dev_rand);

		REQUIRE_NOTHROW(
		    [&]() { test_object.RegisterObserver(recorder); }());

		for (unsigned i = 0; i < rand_count; ++i) {
			auto& input = test_input[i];
			input = SdlEventSimulator::randomArrowKey();
			event_queue_ref.push(input);
		}

		test_object.Notify();

		REQUIRE(recorder.received.size() > 0);

		for (unsigned i = 0; i < MAX_EVENTS; ++i) {
			auto& event = recorder.received[i];

			CHECK(event.key.keysym.sym ==
			      test_input[i].key.keysym.sym);
		}
	}

	FIXTURE_TEST("elemental::SdlEventSource::PollEvents works")
	{
		auto& event_queue = Inspector::GetEventQueue(test_object);
		while (!event_queue.empty()) {
			event_queue.pop();
		}
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
		}
		for (unsigned n = 0; n < 5; ++n) {
			SdlEventSimulator::randomArrowKey();
		}

		test_object.PollEvents();

		REQUIRE(event_queue.size() > 0);
	}

	// Add more tests as needed

} // End of test suite

// clang-format off
 // vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
