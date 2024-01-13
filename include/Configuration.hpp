/* Configuration.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "type-aliases.hpp"

namespace elemental {

class Configuration : public Dictionary<std::string>
{
  public:
	inline Configuration() : file_path() {}
	Configuration(const std::filesystem::path& file_path);

	void LoadConfig();
	void LoadConfig(const std::filesystem::path& file_path);
	void SaveConfig() const;

	inline const std::filesystem::path& FilePath()
	{
		return this->file_path;
	}

  private:
	std::filesystem::path file_path;
};

using json = nlohmann::json;

namespace ns {
inline void
to_json(json& json_data, const Configuration& config)
{
	json_data = json{};
	for (auto& [key, value] : config) {
		json_data[key] = value;
	}
}

inline void
from_json(const json& json_data, Configuration& config)
{
	auto options_map = json_data.get<Dictionary<std::string>>();
	config.swap(options_map);
}
} // names[ace ns
} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
