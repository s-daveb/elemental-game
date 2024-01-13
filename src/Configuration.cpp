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
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <unordered_map>

#include "private/debuginfo.hpp"

#include "type-aliases.hpp"

using namespace elemental;
namespace fs = std::filesystem;
using json = nlohmann::json;

static std::stringstream error_buffer;

Configuration::Configuration(const fs::path& config_file)
    : Dictionary<std::string>()
    , file_path(config_file)
{

	if (!this->file_path.empty()) {
		// debugprint("Configuration::constructor: file_path not empty:
		// "
		//            << this->file_path.string());
		if (!fs::exists(this->file_path)) {

			error_buffer.clear();
			error_buffer << "config_file  "
				     << fs::absolute(config_file).string()
				     << " does not exist or is inaccessible"
				     << std::endl;
			throw Exception(error_buffer.str());
		}

		LoadConfig(this->file_path);
	}
}

void
Configuration::LoadConfig()
{
	ASSERT(this->file_path.empty() == false);

	try {

		// debugprint("Configuration::LoadConfig - Opening the file");
		std::ifstream file(this->file_path);
		if (!file.is_open()) {
			error_buffer.clear();
			error_buffer << "Error opening configuration file: "
				     << this->file_path << std::endl;
			throw Exception(error_buffer.str());
		}

		json config_json;
		file >> config_json;

		if (config_json.is_object()) {
			// debugprint("Configuration::LoadConfig - "
			//            "json.is_object(): PASS");
			auto loaded_data =
			    config_json.get<Dictionary<std::string>>();
			// debugprint(
			//     "loaded_data.size() = " << loaded_data.size());
			this->swap(loaded_data);
			// debugprint("swapped!"
			//	   << "this->size() = " << this->size());
			return;
		} else {
			error_buffer.clear();
			error_buffer
			    << "Error: Configuration file should contain "
			       "a JSON object."
			    << std::endl;
			throw Exception(error_buffer.str());
		}
	} catch (const std::exception& e) {
		error_buffer.clear();
		error_buffer << "Error loading configuration: " << e.what()
			     << std::endl;
		throw Exception(error_buffer.str());
	}
}

void
Configuration::LoadConfig(const std::filesystem::path& config_file)
{
	this->file_path = config_file;
	this->LoadConfig();
}

void
Configuration::SaveConfig() const
{
	static std::stringstream berr;
	try {
		std::ofstream file(file_path);
		if (!file.is_open()) {
			berr.clear();
			berr << "Error opening configuration file for writing: "
			     << file_path << std::endl;
			throw Exception(berr.str());
		}

		json config_json = static_cast<Dictionary<std::string>>(*this);
		file << std::setw(4) << config_json
		     << std::endl; // Indentation for better human readability

		return;
	} catch (const std::exception& e) {

		berr.clear();
		berr << "Error saving configuration: " << e.what() << std::endl;
		throw Exception(berr.str());
	}
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
