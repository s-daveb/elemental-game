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

#include <filesystem>

#include <nlohmann/json.hpp>

namespace elemental::configuration {

class JsonConfigFile : public FileResource
{
  public:
	JsonConfigFile(const std::filesystem::path& file_path,
	               create_dirs_mode mode = DEFAULT);
	virtual ~JsonConfigFile();

	nlohmann::json& Read();
	void Write();

	template<typename T>
	T Get() const
	{
		return config_json.get<T>();
	}

	template<typename T>
	void Set(const T& value)
	{
		config_json = value;
	}

	inline nlohmann::json& GetJsonData() { return this->config_json; };

  protected:
	nlohmann::json config_json;
};
} // namespace elemental::configuration

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
