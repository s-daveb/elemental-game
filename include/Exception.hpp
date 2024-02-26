/* exceptions.hpp
 * Copyright © 2020 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "types.hpp"

#include <algorithm>
#include <cstring>
#include <exception>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

#define ASSERT(condition)                                                      \
	if (condition == false) {                                              \
		elemental::assert_impl(#condition);                            \
	}

#define ASSERT_MSG(condition, msg)                                             \
	if (condition == false) {                                              \
		elemental::assert_impl(#condition, msg);                       \
	}

namespace elemental {

class Exception : public std::exception
{

  public:
	Exception(c::const_string message = kDEFAULT_ERROR);
	Exception(std::string message);

	Exception(const elemental::Exception& other) = default;
	Exception(const std::exception& inner);

	auto operator=(const Exception&) -> Exception& = delete;

	[[nodiscard]] auto what() const noexcept -> const char* override;
	[[nodiscard]] auto stacktrace() const noexcept -> const std::string&;

	constexpr static auto kDEFAULT_ERROR = "An exception has ocurred!";

  protected:
	void build_what_message(c::const_string class_name = "",
	                        c::const_string optional_data = "");

  private:
	std::string error_message;
	std::string what_message;
	std::string stack_trace;

	std::exception_ptr inner_exception_ptr;
};

struct NotImplementedException : public Exception
{
	NotImplementedException() : Exception("Method not implemented") {}
	~NotImplementedException() override = default;
};

void inline assert_impl(c::const_string failed_condition,
                        c::const_string assert_reason = "")
{
	std::stringstream assert_buffer;
	assert_buffer << failed_condition << " is false!";

	if (std::strlen(assert_reason) > 0) {
		assert_buffer << std::endl
			      << "\tassert_resion: " << assert_reason;
	}
	assert_buffer << std::flush;
	throw Exception(assert_buffer.str());
}
} // namespace elemental
  // clang-format off
// vim: set foldmethod=marker foldmarker=@{,@} textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
