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
#include "types/rendering.hpp"

#include "util/testing.hpp"

#include <SDL.h>
#include <random>

namespace NS = elemental;
using NS::SdlEventSource;

template<>
class NS::debug::Inspector<SdlEventSource>
{
  public:
	static auto getEventQueue(SdlEventSource& other)
	    -> std::queue<SDL_Event>&
	{
		return other.event_queue;
	}
};
typedef NS::debug::Inspector<NS::SdlEventSource> Inspector;

BEGIN_TEST_SUITE("elemental::SdlEventSource")
{
	using namespace NS;
	class EventRecorder : public IObserver
	{
	  public:
		EventRecorder() : IObserver() {}

		void recieveMessage(const Observable& sender,
		                    std::any message) override
		{
			auto event = std::any_cast<SDL_Event&>(message);
			received.push_back(event);
		}
		~EventRecorder() override = default;
		std::vector<SDL_Event> received;
	};

	struct SdlEventSourceFixture : public SdlTestFixture
	{

		SdlEventSourceFixture()
		    : SdlTestFixture()
		    , test_object(Singleton::getReference<SdlEventSource>())
		    , recorder()
		    , event_queue_ref(Inspector::getEventQueue(test_object))
		    , dev_rand()
		{
			/* Clear the event queue in between tests */
			while (!event_queue_ref.empty()) {
				event_queue_ref.pop();
			}
		}

		~SdlEventSourceFixture() override = default;
		SdlEventSource& test_object;
		EventRecorder recorder;
		std::queue<SDL_Event>& event_queue_ref;

		std::random_device dev_rand;
	};
	using TestFixture = SdlEventSourceFixture;

	FIXTURE_TEST(
	    "elemental::SdlEventSource::Enqueue stores events in order")
	{
		const int kMAX_EVENTS = 10;
		SDL_Event test_input_list[kMAX_EVENTS];

		unsigned int rand_count =
		    std::uniform_int_distribution(5, kMAX_EVENTS)(dev_rand);

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
		const int kMAX_EVENTS = 5;
		SDL_Event test_input[kMAX_EVENTS];

		unsigned int rand_count =
		    std::uniform_int_distribution(5, kMAX_EVENTS)(dev_rand);

		REQUIRE_NOTHROW(
		    [&]() { test_object.registerObserver(recorder); }());

		for (unsigned i = 0; i < rand_count; ++i) {
			auto& input = test_input[i];
			input = SdlEventSimulator::randomArrowKey();
			event_queue_ref.push(input);
		}

		test_object.transmitEvents();

		REQUIRE(recorder.received.size() > 0);

		for (unsigned i = 0; i < kMAX_EVENTS; ++i) {
			auto& event = recorder.received[i];

			CHECK(event.key.keysym.sym ==
			      test_input[i].key.keysym.sym);
		}
	}

	FIXTURE_TEST("elemental::SdlEventSource::PollEvents works")
	{
		auto& event_queue = Inspector::getEventQueue(test_object);
		while (!event_queue.empty()) {
			event_queue.pop();
		}
		
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			;
		}
		for (unsigned n = 0; n < 5; ++n) {
			SdlEventSimulator::randomArrowKey();
		}

		test_object.pollEvents();

		REQUIRE(event_queue.size() > 0);
	}
}
// clang-format off
 // vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
