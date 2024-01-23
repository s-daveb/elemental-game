/* JsonConfigFile.cpp
 * Copyright © 202t4 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "JsonConfigFile.hpp"
#include "Exception.hpp"
#include "types/configuration.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "private/debuginfo.hpp"

#include "types.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

static std::stringstream error_buffer;

namespace elemental {

JsonConfigFile::JsonConfigFile(const fs::path& file_path)
    : FileResource(file_path)
{
}

JsonConfigFile::~JsonConfigFile() {}

void
JsonConfigFile::Read()
{
	try {
		std::ifstream file_stream(file_path);

		if (!file_stream.is_open()) {
			error_buffer.str("");
			error_buffer
			    << "Error opening JsonConfigFile for reading: "
			    << file_path << std::endl;
			throw Exception(error_buffer.str());
		}

		json config_json;
		file_stream >> config_json;

		if (config_json.is_object()) {
			auto loaded_data =
			    config_json.get<configuration::dictionary>();
			this->config_data.swap(loaded_data);
		} else {
			error_buffer.str("");
			error_buffer << "Error: JsonConfigFile should contain "
					"a serialized JSON object."
				     << std::endl;
			throw Exception(error_buffer.str());
		}
	} catch (elemental::Exception& except) {
		throw;
	} catch (const std::exception& e) {
		error_buffer.str("");
		error_buffer << "JsonConfigFile::Read() error" << std::endl
			     << e.what() << std::flush;
		throw Exception(error_buffer.str());
	}
}

void
JsonConfigFile::Write()
{
	try {
		std::ofstream file_stream(file_path,
		                          std::ios::out); // | std::ios::trunc);
		if (!file_stream.is_open()) {
			error_buffer.str("");
			error_buffer << "Error opening JsonConfigFile "
					"for writing: "
				     << file_path << std::endl;
			throw Exception(error_buffer.str());
		}

		json config_json = config_data;
		file_stream << config_json << std::endl;

		/* Force the file to flush to disk on macOS.
		 * (The buffering is really aggressive!) */
		file_stream.flush();
		file_stream.close();

		return;
	} catch (elemental::Exception& except) {
		throw;
	} catch (const std::exception& e) {

		error_buffer.str("");
		error_buffer << "JSonConfigFile::Save() error: " << std::endl
			     << e.what() << std::flush;
		throw Exception(error_buffer.str());
	}
}
} // namespace elemental::configuration

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
