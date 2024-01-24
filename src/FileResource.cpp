/* FileResource.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "FileResource.hpp"
#include "Exception.hpp"
#include "types.hpp"

#include <filesystem>
#include <fstream>

using namespace elemental;

namespace fs = std::filesystem;

FileResource::FileResource(const fs::path& file_path, create_dirs_mode mode)
    : file_path(file_path)
{
	ASSERT(file_path.empty() == false);
	try {
		auto directory_path = file_path.parent_path();
		std::cout << "Dir path: " << directory_path.string()
			  << std::endl;

		if (!directory_path.empty() && !fs::exists(directory_path)) {
			if (mode != CREATE_MISSING_DIRS) {
				throw UnreachablePathException(directory_path);
			} else {
				std::cout << "creating config dir" << std::endl;
				fs::create_directories(directory_path);
			}
		}

		if (fs::exists(file_path) == false) {
			std::ofstream file_stream(
			    file_path, std::ios::out | std::ios::trunc);
			file_stream.flush();
			file_stream.close();
		}
	} catch (elemental::Exception&) {
		throw;
	} catch (std::exception& exception) {
		throw Exception(exception);
	}
};
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
