/* Configuration.test.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Configuration.hpp"
#include "Exception.hpp"
#include "test-utils/common.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "private/debuginfo.hpp"

#include "type-aliases.hpp"

using namespace elemental;

namespace fs = std::filesystem;

constexpr c::const_string TEST_FILE_PATH = "/tmp/test_config.json";
constexpr c::const_string INVALID_FILE_PATH = "/tmp/test_bad_config.json";

BEGIN_TEST_SUITE("elemental::configuration")
{
	namespace { // Test fixtures
	struct SampleFileGenerator
	{
		SampleFileGenerator()
		{
			if (!fs::exists(TEST_FILE_PATH)) {
				std::ofstream f(TEST_FILE_PATH);
				f << R"({"key1": "value1", "key2": "value2"})"
				  << std::endl;
				f.close();
			}
		}
		~SampleFileGenerator()
		{
			try {
				if (!fs::exists(TEST_FILE_PATH)) {
					fs::remove(TEST_FILE_PATH);
				}
			} catch (std::exception& e) {
				debugprint(e.what());
			}
		}
	};
	using TestFixture = SampleFileGenerator;
	}

	TEST("elemental::configuration::dictionary is serializablable like "
	     "std::map<std::string,std::string>")
	{
		configuration::dictionary test_data;
		nlohmann::json j_object;
		test_data["one"] = "1";
		test_data["resolution"] = "1280x720";
		test_data["Hello"] = "world";

		REQUIRE_NOTHROW([&]() { j_object = test_data; }());

		REQUIRE(test_data["one"] == j_object["one"]);
		REQUIRE(test_data["resolution"] == j_object["resolution"]);
		REQUIRE(test_data["Hello"] == j_object["Hello"]);
	}

	FIXTURE_TEST("configuration::load")
	{
		SECTION("configuration::load w/ valid file")
		{
			auto config = configuration::load(TEST_FILE_PATH);

			REQUIRE_FALSE(config.empty());
			REQUIRE(config.size() == 2);
			REQUIRE(config["key1"] == "value1");
			REQUIRE(config["key2"] == "value2");
		}
		SECTION("configuration::load w/ invalid path throws exception")
		{

			REQUIRE_THROWS_AS(
			    [&]() {
				    auto config = configuration::load(
					"/tmp/does-not-exists");
			    }(),
			    Exception);
		}
		SECTION("configuration::load w/ bad file throws exception")
		{
			if (!fs::exists(INVALID_FILE_PATH)) {
				std::ofstream f(INVALID_FILE_PATH);
				f << R"({"Hello World"})" << std::endl;
				f.close();
			}

			REQUIRE_THROWS_AS(
			    [&]() {
				    auto config =
					configuration::load(INVALID_FILE_PATH);
			    }(),
			    Exception);
		}
	}
	TEST("configuration::save()")
	{
		const std::string ALT_FILE_PATH = "/tmp/save-test.json";
		std::ifstream resulting_file;
		nlohmann::json resulting_json;

		auto test_data = configuration::dictionary();
		test_data["one"] = "1";
		test_data["resolution"] = "1280x720";
		test_data["Hello"] = "world";

		SECTION("an invalid path is provided")
		{
			REQUIRE_THROWS_AS(
			    [&]() {
				    configuration::save(
					test_data, "/BADPATH/lolfile.json");
			    }(),
			    Exception);
		}
		SECTION("a valid path is provided")
		{
			REQUIRE_NOTHROW([&]() {
				configuration::save(test_data, ALT_FILE_PATH);
			}());

			resulting_file.open(ALT_FILE_PATH);
			resulting_file >> resulting_json;

			REQUIRE(test_data["one"] == resulting_json["one"]);
			REQUIRE(test_data["resolution"] ==
			        resulting_json["resolution"]);
			REQUIRE(test_data["Hello"] == resulting_json["Hello"]);
		}
		fs::remove(ALT_FILE_PATH);
	}

} // END_TEST_SUITE("elemental::Configuration")

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
