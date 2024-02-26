/* Application.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vector>

#include "IApplication.hpp"
#include "types.hpp"

#include <vector>

namespace elemental {
class Application : public IApplication // NOLINT
{
  public:
	~Application() override = default;

	auto init(int argc, c::const_string argv[], c::const_string envp[])
	    -> void override;

	auto run() -> int override = 0;

	[[nodiscard]] inline auto getArguments() const
	    -> const std::vector<std::string>& override
	{
		return this->arguments;
	}
	[[nodiscard]] inline auto getEnvironment() const
	    -> const Dictionary<const std::string>& override
	{
		return this->environment_variables;
	}

  protected:
	Application();
	Application(const IApplication&) = delete;
	Application(IApplication&&) = delete;
	auto operator=(const IApplication&) -> Application& = delete;
	auto operator=(IApplication&&) -> Application& = delete;

	void parse_arguments(int argc, c::const_string argv[]);
	void create_env_dictionary(c::const_string envp[]);

	std::vector<std::string> arguments;
	Dictionary<const std::string> environment_variables;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
