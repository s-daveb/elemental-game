/* LoopRegulator.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "LoopRegulator.hpp"

#include <chrono>
#include <thread>

using namespace std::chrono;

// Constructor with default desired framerate of 30 frames per second
LoopRegulator::LoopRegulator(uint32_t rate_per_second)
    : desired_rate_per_second(0)
    , start_time()
    , end_time()
    , elapsed_ms(0)
    , desired_delay_ms(0)
{
	this->SetRate(rate_per_second);
}

LoopRegulator::~LoopRegulator() {}

void
LoopRegulator::StartUpdate()
{
	end_time = high_resolution_clock::time_point();
	start_time = high_resolution_clock::now();
}

// End the loop update and calculate elapsed time
std::chrono::milliseconds
LoopRegulator::EndUpdate()
{
	this->end_time = high_resolution_clock::now();
	this->elapsed_ms = duration_cast<milliseconds>(end_time - start_time);
	return elapsed_ms;
}

void
LoopRegulator::SetRate(float new_rate)
{
	this->desired_rate_per_second = new_rate;
	this->desired_delay_ms =
	    duration_cast<milliseconds>(seconds(1) / new_rate);
}

// Delay the loop to achieve the desired framerate
milliseconds
LoopRegulator::Delay()
{

	if (end_time == high_resolution_clock::time_point()) {
		this->EndUpdate();
	}

	// Calculate the remaining time to dela
	auto remaining_delay_ms = this->desired_delay_ms - this->elapsed_ms;

	// Check if delay is needed
	if (remaining_delay_ms.count() > 0.0) {
		std::this_thread::sleep_for(remaining_delay_ms);
		return remaining_delay_ms;
	} else {
		return milliseconds(0);
	}
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
