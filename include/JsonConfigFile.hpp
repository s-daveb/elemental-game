/* JsonConfigFile.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "FileResource.hpp"

#include "types.hpp"
#include "types/configuration.hpp"

#include <filesystem>

namespace elemental::configuration {

class JsonConfigFile : public FileResource
{
  public:
	JsonConfigFile(const std::filesystem::path& file_path,
	               create_dirs_mode mode = DEFAULT);
	virtual ~JsonConfigFile();

	configuration::dictionary& Read();
	void Write();

	/// \name Convenience wrapper interface
	/// @{
	///
	inline std::string& operator[](const std::string& key) noexcept
	{
		return this->config_data[key];
	}
	inline std::string& at(const std::string& key)
	{
		return this->config_data.at(key);
	}
	/// @}

	inline configuration::dictionary& GetData()
	{
		return this->config_data;
	};

  protected:
	configuration::dictionary config_data;
};
} // namespace elemental::configuration

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
