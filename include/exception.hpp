/* exceptions.hpp
 * Copyright © 2020 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "private/debuginfo.hpp"
#include "types.hpp"

#include <algorithm>
#include <exception>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

namespace elemental {

class exception : public std::exception {

  public:
    exception(c::const_string _message = default_error);
    exception(const std::string &message);

    exception(const elemental::exception &other) = default;
    exception(const std::exception &inner);

    exception &operator=(const exception &) = delete;

    virtual const char *what() const noexcept override;
    const std::string &stacktrace() const noexcept;

    constexpr static auto default_error = "An exception has ocurred!";

  private:
    void build_what_message();

    std::string error_message;
    std::string what_message;
    std::string stack_trace;

    std::exception_ptr inner_exception_ptr;
};
} // namespace elemental
  // clang-format off
// vim: set foldmethod=marker foldmarker=@{,@} textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
