/* LoopRegulator.test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Exception.hpp"
#include "LoopRegulator.hpp"
#include "sys/platform.hpp"

#include "./common.hpp"

#include <catch2/matchers/catch_matchers_string.hpp>

#include <chrono>
#include <cmath>
#include <random>
#include <thread>

BEGIN_TEST_SUITE("elemental::LoopRegulator")
{
	using namespace elemental;
	namespace this_thread = std::this_thread;

	struct TestFixture
	{
		LoopRegulator test_object;
	};
	FIXTURE_TEST("elemental::LoopRegulator - Initialization")
	{
		CHECK(test_object.elapsed_ms.count() == 0);
		CHECK(test_object.start_time == steady_clock::time_point());
	}
	FIXTURE_TEST(
	    "elemental::LoopRegulator - Time calculations work properly")
	{
		test_object.StartUpdate();
		this_thread::sleep_for(std::chrono::seconds(1));
		auto timestamp1 = test_object.start_time;
		auto elapsed_time = test_object.EndUpdate();
		auto timestamp2 = test_object.end_time;
		CHECK(timestamp1 < timestamp2);
		REQUIRE(test_object.elapsed_ms.count() > 900);
	};

#ifdef CI_BUILD
	TEST("elemental::LoopRegulator:Delay works within 5ms tolerance")
	{
		if (platform::MACOS) {
			WARN("(macos) this test always fails due to low "
			     "priority processor scheduling in CI build env");
			SUCCEED();
			return;
		}
	}
#else
	FIXTURE_TEST(
	    "elemental::LoopRegulator:Delay works within 5ms tolerance")
	{

		const auto acceptable_margin_error_ms = 5ms;

		// Seed the random number generator with the current time
		unsigned seed =

		    std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine gen(seed);

		// Define the distribution for random delays (0 to 1000/60
		// milliseconds)
		std::uniform_int_distribution<int> delay_generator(
		    0, static_cast<int>(1000.0 / 60));

		for (unsigned i = 0; i < 100; ++i) {
			auto random_delay = milliseconds(delay_generator(gen));

			test_object.StartUpdate();
			this_thread::sleep_for(random_delay);
			auto time_delayed_ms = test_object.Delay();
			auto& expected_delay = test_object.desired_delay_ms;

			auto margin_error_ms =
			    (expected_delay - (time_delayed_ms + random_delay));

			if (margin_error_ms.count() < 0) {
				CHECK(margin_error_ms >
				      (-1 * acceptable_margin_error_ms));

			} else {
				CHECK(margin_error_ms <
				      acceptable_margin_error_ms);
			}
		}
	};
#endif
}
// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
