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
#include "util/debug.hpp"

#include "test-utils/common.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

using namespace elemental;
using namespace elemental::configuration;

namespace fs = std::filesystem;

constexpr c::const_string kINPUT_FILE_PATH = "/tmp/test_config.json";
constexpr c::const_string kBADFILE_PATH = "/tmp/test_bad_config.json";
constexpr c::const_string kNON_EXISTENT_PATH = "/hades/tmp/dne/file.json";

BEGIN_TEST_SUITE("elemental::JsonConfigFile")
{
	namespace { // Test fixtures
		struct SampleFileGenerator
		{
			SampleFileGenerator()
			{
				if (!fs::exists(kINPUT_FILE_PATH)) {
					std::ofstream f(kINPUT_FILE_PATH,
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
					if (fs::exists(kINPUT_FILE_PATH)) {
						fs::remove(kINPUT_FILE_PATH);
					}
				} catch (std::exception& e) {
					DBG_PRINT(e.what());
				}
			}
		};
		using TestFixture = SampleFileGenerator;
	} // namespace

	TEST("elemental::nlohmann::json is serializablable like "
	     "std::map<std::string,std::string>")
	{
		elemental::Dictionary<std::string> test_data;
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
			auto config = JsonConfigFile(kINPUT_FILE_PATH);
			auto& config_data = config.getJsonData();

			REQUIRE(config_data.empty());
		}
		SECTION("JsonConfigFile w/ invalid path throws exception")
		{

			REQUIRE_THROWS_AS(JsonConfigFile(kNON_EXISTENT_PATH),
			                  elemental::UnreachablePathException);
		}
	}
	FIXTURE_TEST("JsonConfigFile::Read")
	{
		auto config_file = JsonConfigFile(kINPUT_FILE_PATH);
		auto& json_data = config_file.getJsonData();

		SECTION("JsonConfigFile::Read w/ valid file")
		{
			config_file.read();
			REQUIRE_FALSE(json_data.empty());
			REQUIRE(json_data.size() == 2);

			REQUIRE(json_data["key1"] == "value1");
			REQUIRE(json_data["key2"] == "value2");
		}
		SECTION("JsonConfigFile::Read w/ bad file throws exception")
		{
			if (!fs::exists(kBADFILE_PATH)) {
				std::ofstream f(kBADFILE_PATH);
				f << R"({"Hello World"})" << std::endl;
				f.close();
			}
			REQUIRE_THROWS_AS(
			    [&]() {
				    auto bad_config =
					JsonConfigFile(kBADFILE_PATH);

				    bad_config.read();
			    }(),
			    Exception);
		}
		if (fs::exists(kBADFILE_PATH)) {
			fs::remove(kBADFILE_PATH);
		}
	}
	TEST("JsonConfigFile::Write()")
	{
		SECTION("Create File and Read It back In")
		{
			auto config_file = JsonConfigFile(kINPUT_FILE_PATH);
			auto& test_data = config_file.getJsonData();

			test_data["one"] = "1";
			test_data["resolution"] = "1280x720";
			test_data["Hello"] = "world";

			config_file.write();

			std::ifstream resulting_file(kINPUT_FILE_PATH);
			nlohmann::json jobject;

			resulting_file >> jobject;
			auto written_data =
			    jobject.get<elemental::Dictionary<std::string>>();

			REQUIRE(written_data["one"] == test_data["one"]);
			REQUIRE(written_data["resolution"] ==
			        test_data["resolution"]);
			REQUIRE(written_data["Hello"] == test_data["Hello"]);
		}
		fs::remove(kINPUT_FILE_PATH);
	}

	FIXTURE_TEST(
	    "JsonConfigFile::Get<T>() basically wraps nlohmann::json::get<T>()")
	{
		auto config_file = JsonConfigFile(kINPUT_FILE_PATH);
		auto& json_data = config_file.getJsonData();

		config_file.read();
		auto obtained_data =
		    config_file.get<elemental::Dictionary<std::string>>();

		REQUIRE(obtained_data["key1"] == "value1");
		REQUIRE(obtained_data["key2"] == "value2");
	}
	FIXTURE_TEST(
	    "JsonConfigFile::Set() basically wraps nlohmann::json::operator=()")
	{
		auto config_file = JsonConfigFile(kINPUT_FILE_PATH);
		elemental::Dictionary<std::string> test_data;

		test_data["one"] = "1";
		test_data["resolution"] = "1280x720";
		test_data["Hello"] = "world";

		config_file.set(test_data);

		auto& json_data = config_file.getJsonData();

		REQUIRE(json_data["one"] == test_data["one"]);
		REQUIRE(json_data["resolution"] == test_data["resolution"]);
		REQUIRE(json_data["Hello"] == test_data["Hello"]);
	}
}
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
