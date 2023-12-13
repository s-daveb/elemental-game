/* Result.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "types.hpp"
#include <utility>

namespace elemental {

template<typename T>
struct Result
{

	inline Result(const error_t& error_flag, const T error_data)
	    : val(error_flag, error_data)
	    , IsError(this->val.first)
	    , ErrorData(this->val.second)
	{
	}
	inline Result(const bool& error_flag, const T error_data)
	    : val(error_t(error_flag), error_data)
	    , IsError(this->val.first)
	    , ErrorData(this->val.second)
	{
	}

	virtual ~Result() = default;
	Result(const Result& other) = default;
	inline Result& operator=(const Result& other)
	{
		this->val = other.val;
		this->IsError = this->val.first;
		this->ErrorData = this->val.data;
	}
	error_t& IsError;
	T& ErrorData;

  private:
	std::pair<error_t, T> val;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
