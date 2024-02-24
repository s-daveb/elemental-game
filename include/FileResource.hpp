/* FileResource.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "Exception.hpp"
#include "types.hpp"

#include <filesystem>
#include <fstream>

namespace elemental {
class FileResource
{
  public:
	enum create_dirs_mode
	{
		DEFAULT = 0,
		CREATE_MISSING_DIRS = 1,
	};
	FileResource(const std::filesystem::path& file_path,
	             create_dirs_mode mode = DEFAULT);

	virtual ~FileResource() = default;

  protected:
	std::filesystem::path file_path;
};

struct UnreachablePathException : public Exception
{
	inline UnreachablePathException(const std::filesystem::path& path)
	    : Exception("Unreachable path or directory")
	    , unreachable_path(path)
	{
		this->build_what_message("elemental::UnreachablePathException",
		                         path.c_str());
	}

	virtual const char* what() const noexcept override
	{
		return Exception::what();
	}

	std::filesystem::path unreachable_path;
};
}

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
