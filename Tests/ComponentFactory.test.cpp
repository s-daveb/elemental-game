/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "test-utils/common.hpp"

#include "ComponentFactory.hpp"

namespace {
using namespace elemental;

/* This class cannot use FakeIt::Mock because the Component class is a
 * template. At least, I current do not know of a way in which it can
 * be used */
class TestComponent : public Component
{
    public:
	using Base = Component;

	auto getTypeIndex() -> TypeInfo override
	{ return typeid(TestComponent); }

	TestComponent(ComponentFactory& owner) : Component(owner) {}
	~TestComponent() override = default;
};

}  // namespace

BEGIN_TEST_SUITE("elemental::ComponentFactory")
{
	using namespace elemental;

	TEST_CASE("Basic ComponentFactory construction")
	{ ComponentFactory factory; }

	TEST_CASE("ComponentFactory::createComponent")
	{
		ComponentFactory factory;

		auto result = factory.createComponent<TestComponent>();
		REQUIRE(result != nullptr);
	}

	TEST_CASE("ComponentFactory::getComponentVector")
	{
		ComponentFactory factory;

		auto new_component = factory.createComponent<TestComponent>();

		REQUIRE_NOTHROW([&]() -> void {
			auto result =
			    factory.getComponentVector<TestComponent>();
			REQUIRE(result.size() == 1);
		}());
	}

	TEST_CASE("ComponentFactory::getComponent")
	{
		ComponentFactory factory;

		REQUIRE_THROWS_AS(
		    [&]() -> void {
			    factory.getComponent<TestComponent>(0x0);
		    }(),
		    std::out_of_range);
	}
}

// clang-format off
// vim: set textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
