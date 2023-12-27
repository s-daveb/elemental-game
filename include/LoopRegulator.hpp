/* LoopRegulator.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <SDL.h>
#include <chrono>
#include <ratio>
#include <thread>

using namespace std::chrono;

class LoopRegulator
{
  public:
	// Constructor with default desired framerate of 30 frames per second
	LoopRegulator(uint32_t rate_per_second = 30);
	virtual ~LoopRegulator();

	// Start the loop update
	void StartUpdate();

	// End the loop update and calculate elapsed time
	milliseconds EndUpdate();

	void SetRate(float new_rate);

	// Delay the loop to achieve the desired framerate
	milliseconds Delay();
#ifndef UNIT_TEST
  protected:
#endif
	uint32_t desired_rate_per_second;

	high_resolution_clock::time_point start_time;
	high_resolution_clock::time_point end_time;

	milliseconds elapsed_ms;
	milliseconds desired_delay_ms;
};

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
