/* IApplication.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "types.hpp"

#include <functional>
#include <vector>

namespace elemental {

struct IApplication;
using IApplicationRef = std::reference_wrapper<IApplication>;

class IApplication
{
  public:
	virtual ~IApplication() = default;

	virtual void init(int argc, c::const_string argv[],
	                  c::const_string envp[]) = 0;
	virtual int run() = 0;

	virtual const std::vector<std::string>& getArguments() const = 0;
	virtual const Dictionary<const std::string>& getEnvironment() const = 0;

  protected:
	IApplication() {}
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax  textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
