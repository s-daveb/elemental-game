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
#include "type-aliases.hpp"

#include <vector>

namespace elemental {
class Application : public IApplication
{
  public:
	virtual ~Application() = default;

	virtual void Init(int argc, c::const_string argv[],
	                  c::const_string envp[]) override;
	virtual int Run() override = 0;

	virtual inline const std::vector<std::string>& GetArguments()
	    const override
	{
		return this->arguments;
	}
	virtual inline const Dictionary<const std::string>& GetEnvironment()
	    const override
	{
		return this->environment_variables;
	}

  protected:
	Application();

	void parse_arguments(int argc, c::const_string argv[]);
	void create_env_dictionary(c::const_string envp[]);

	std::vector<std::string> arguments;
	Dictionary<const std::string> environment_variables;
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
