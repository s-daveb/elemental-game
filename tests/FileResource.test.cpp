/* FileResource.test.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "FileResource.hpp"

#include "sys/debuginfo.hpp"
#include "test-utils/common.hpp"
#include "util/debug.hpp"

#include "Exception.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace elemental;

namespace fs = std::filesystem;

constexpr c::const_string kTEST_FILE_PATH = "/tmp/test_file.txt";
constexpr c::const_string kUNREACHABLE_FILE_PATH =
    "/tmp/nonexistent_dir/file.txt";

BEGIN_TEST_SUITE("elemental::FileResource")
{
	namespace { // Test fixtures
		struct SampleFileGenerator
		{
			SampleFileGenerator()
			{
				if (!fs::exists(kTEST_FILE_PATH)) {
					std::ofstream f(kTEST_FILE_PATH);
					f << "Sample file content" << std::endl;
					f.close();
				}
			}

			~SampleFileGenerator()
			{
				try {
					if (fs::exists(kTEST_FILE_PATH)) {
						fs::remove(kTEST_FILE_PATH);
					}
				} catch (std::exception& e) {
					DBG_PRINT(e.what());
				}
			}
		};
		using TestFixture = SampleFileGenerator;
	} // namespace

	FIXTURE_TEST("FileResource constructor")
	{
		SECTION(" with valid input file path, file exists ")
		{
			REQUIRE_NOTHROW(FileResource(kTEST_FILE_PATH));
		}

		SECTION("throws exception for unreachable file")
		{
			REQUIRE_THROWS_AS(
			    [&]() {
				    auto fileResource =
					FileResource(kUNREACHABLE_FILE_PATH);
			    }(),
			    UnreachablePathException);
		}
	}

} // END_TEST_SUITE("elemental::FileResource")

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
