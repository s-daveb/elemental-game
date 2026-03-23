/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "test-utils/common.hpp"

#include "util/testing.hpp"

#include "Component.hpp"

#include <catch2/catch_approx.hpp>

using namespace Catch::literals;
using namespace fakeit;
using namespace elemental;

// Fake implementation
struct elemental::ComponentFactory
{
	virtual ~ComponentFactory() = default;
};

struct TestComponent : public Component
{
	TestComponent(ComponentFactory& owner) : Component(owner) {}
	~TestComponent() override = default;

	auto getFactory() -> auto& { return this->factory; };
	auto getTypeIndex() -> TypeInfo override
	{ return typeid(TestComponent); }
};

TEST_CASE("Test Component Class", "[component]")
{
	// Mock the ComponentFactory
	auto              mock_factory = Mock<ComponentFactory>();
	ComponentFactory& factory      = mock_factory.get();

	// Create a component instance with the mocked factory
	std::shared_ptr<TestComponent> test_component =
	    std::make_shared<TestComponent>(factory);
	std::shared_ptr<Component> component(test_component);

	// Verify the component's factory reference matches our mock
	REQUIRE(component != nullptr);
	REQUIRE(&(test_component->getFactory()) == &(factory));

	// Test instance ID
	auto instance_id = component->getInstanceId();
	REQUIRE(instance_id >= 0);

	// Test isChildClass template specializations
	static_assert(Component::is_child_class<TestComponent>(),
	              "TestComponent is a child class of Component>");
}
// clang-format off
// vim: set  textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
