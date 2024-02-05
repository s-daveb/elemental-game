/* JsonConfigFile.test.cpp
 * }
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "JsonConfigFile.hpp"
#include "Exception.hpp"

#include "sys/debuginfo.hpp"
#include "types/legible_ctypes.hpp"
#include "util/debugprint.hpp"

#include "test-utils/common.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

using namespace elemental;
using namespace elemental::configuration;

namespace fs = std::filesystem;

constexpr c::const_string INPUT_FILE_PATH = "/tmp/test_config.json";
constexpr c::const_string BADFILE_PATH = "/tmp/test_bad_config.json";
constexpr c::const_string NON_EXISTENT_PATH = "/hades/tmp/dne/file.json";

BEGIN_TEST_SUITE("elemental::JsonConfigFile")
{
	namespace { // Test fixtures
		struct SampleFileGenerator
		{
			SampleFileGenerator()
			{
				if (!fs::exists(INPUT_FILE_PATH)) {
					std::ofstream f(INPUT_FILE_PATH,
					                std::ios::out |
					                    std::ios::trunc);
					f << R"({"key1": "value1", "key2": "value2"})"
					  << std::endl;
					f.close();
				}
			}
			~SampleFileGenerator()
			{
				try {
					if (fs::exists(INPUT_FILE_PATH)) {
						fs::remove(INPUT_FILE_PATH);
					}
				} catch (std::exception& e) {
					debugprint(e.what());
				}
			}
		};
		using TestFixture = SampleFileGenerator;
	} // namespace

	TEST("elemental::nlohmann::json is serializablable like "
	     "std::map<std::string,std::string>")
	{
		elemental::dictionary<std::string> test_data;
		nlohmann::json jsonified;

		test_data["one"] = "1";
		test_data["resolution"] = "1280x720";
		test_data["Hello"] = "world";

		REQUIRE_NOTHROW([&]() { jsonified = test_data; }());

		REQUIRE(test_data["one"] == jsonified["one"]);
		REQUIRE(test_data["resolution"] == jsonified["resolution"]);
		REQUIRE(test_data["Hello"] == jsonified["Hello"]);
	}

	FIXTURE_TEST("JsonConfigFile construction")
	{
		SECTION("JsonConfigFile w/ valid path")
		{
			auto config = JsonConfigFile(INPUT_FILE_PATH);
			auto& config_data = config.GetJsonData();

			REQUIRE(config_data.empty());
		}
		SECTION("JsonConfigFile w/ invalid path throws exception")
		{

			REQUIRE_THROWS_AS(JsonConfigFile(NON_EXISTENT_PATH),
			                  elemental::UnreachablePathException);
		}
	}
	FIXTURE_TEST("JsonConfigFile::Read")
	{
		auto config_file = JsonConfigFile(INPUT_FILE_PATH);
		auto& json_data = config_file.GetJsonData();

		SECTION("JsonConfigFile::Read w/ valid file")
		{
			config_file.Read();
			REQUIRE_FALSE(json_data.empty());
			REQUIRE(json_data.size() == 2);

			REQUIRE(json_data["key1"] == "value1");
			REQUIRE(json_data["key2"] == "value2");
		}
		SECTION("JsonConfigFile::Read w/ bad file throws exception")
		{
			if (!fs::exists(BADFILE_PATH)) {
				std::ofstream f(BADFILE_PATH);
				f << R"({"Hello World"})" << std::endl;
				f.close();
			}
			REQUIRE_THROWS_AS(
			    [&]() {
				    auto bad_config =
					JsonConfigFile(BADFILE_PATH);

				    bad_config.Read();
			    }(),
			    Exception);
		}
		if (fs::exists(BADFILE_PATH)) {
			fs::remove(BADFILE_PATH);
		}
	}
	TEST("JsonConfigFile::Write()")
	{
		SECTION("Create File and Read It back In")
		{
			auto config_file = JsonConfigFile(INPUT_FILE_PATH);
			auto& test_data = config_file.GetJsonData();

			test_data["one"] = "1";
			test_data["resolution"] = "1280x720";
			test_data["Hello"] = "world";

			config_file.Write();

			std::ifstream resulting_file(INPUT_FILE_PATH);
			nlohmann::json jobject;

			resulting_file >> jobject;
			auto written_data =
			    jobject.get<elemental::dictionary<std::string>>();

			REQUIRE(written_data["one"] == test_data["one"]);
			REQUIRE(written_data["resolution"] ==
			        test_data["resolution"]);
			REQUIRE(written_data["Hello"] == test_data["Hello"]);
		}
		fs::remove(INPUT_FILE_PATH);
	}

	FIXTURE_TEST(
	    "JsonConfigFile::Get<T>() basically wraps nlohmann::json::get<T>()")
	{
		auto config_file = JsonConfigFile(INPUT_FILE_PATH);
		auto& json_data = config_file.GetJsonData();

		config_file.Read();
		auto obtained_data =
		    config_file.Get<elemental::dictionary<std::string>>();

		REQUIRE(obtained_data["key1"] == "value1");
		REQUIRE(obtained_data["key2"] == "value2");
	}
	FIXTURE_TEST(
	    "JsonConfigFile::Set() basically wraps nlohmann::json::operator=()")
	{
		auto config_file = JsonConfigFile(INPUT_FILE_PATH);
		elemental::dictionary<std::string> test_data;

		test_data["one"] = "1";
		test_data["resolution"] = "1280x720";
		test_data["Hello"] = "world";

		config_file.Set(test_data);

		auto& json_data = config_file.GetJsonData();

		REQUIRE(json_data["one"] == test_data["one"]);
		REQUIRE(json_data["resolution"] == test_data["resolution"]);
		REQUIRE(json_data["Hello"] == test_data["Hello"]);
	}
}
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8 foldlevel=99 noexpandtab ft=cpp.doxygen :
