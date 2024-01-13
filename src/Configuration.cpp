/* Configuration.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Configuration.hpp"
#include "Exception.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "private/debuginfo.hpp"

#include "type-aliases.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

static std::stringstream error_buffer;

namespace elemental::configuration {

dictionary
load(const fs::path& file_path)
{
	ASSERT(file_path.empty() == false);
	dictionary result;
	try {

		// debugprint("Configuration::LoadConfig - Opening the file");
		std::ifstream file(file_path);
		if (!file.is_open()) {
			error_buffer.str("");
			error_buffer
			    << "Error opening configuration file: " << file_path
			    << std::endl;
			throw Exception(error_buffer.str());
		}

		json config_json;
		file >> config_json;

		if (config_json.is_object()) {
			auto loaded_data = config_json.get<dictionary>();
			result.swap(loaded_data);
		} else {
			error_buffer.str("");
			error_buffer
			    << "Error: Configuration file should contain "
			       "a JSON object."
			    << std::endl;
			throw Exception(error_buffer.str());
		}
	} catch (const std::exception& e) {
		error_buffer.str("");
		error_buffer << "Error loading configuration: " << e.what()
			     << std::endl;
		throw Exception(error_buffer.str());
	}
	return result;
}

void
save(dictionary& data, const fs::path& file_path)
{
	ASSERT(data.empty() == false);
	ASSERT(file_path.empty() == false);

	try {
		std::ofstream file(file_path);
		if (!file.is_open()) {
			error_buffer.str("");
			error_buffer
			    << "Error opening configuration file for writing: "
			    << file_path << std::endl;
			throw Exception(error_buffer.str());
		}

		json config_json = data;
		file << std::setw(4) << config_json
		     << std::endl; // Indentation for better human readability
		file.flush();
		file.close();

		return;
	} catch (const std::exception& e) {

		error_buffer.str("");
		error_buffer << "Error saving configuration: " << e.what()
			     << std::endl;
		throw Exception(error_buffer.str());
	}
}
} // namespace elemental::configuration

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
