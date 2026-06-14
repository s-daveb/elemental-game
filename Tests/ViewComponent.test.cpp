/* ViewComponent.test.cpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "test-utils/common.hpp"

#include "CircleViewComponent.hpp"
#include "ComponentFactory.hpp"
#include "RectangleViewComponent.hpp"
#include "SceneConfig.hpp"
#include "ShapeDrawCommand.hpp"
#include "types/color.hpp"
#include "types/id.hpp"
#include "types/rendering.hpp"

BEGIN_TEST_SUITE("elemental::ViewComponents")
{
	using namespace elemental;
	using namespace IOCore;

	class MockRenderer : public IRenderer
	{
		friend class IRenderer;

	    public:
		void init(RendererSettings&) override {}
		void deactivate() override {}
		auto isInitialized() -> bool override { return false; }
		auto getResolution() -> Resolution override { return { 0, 0 }; }
		auto getWindowSize() -> Area override { return { 0, 0 }; }
		void clearScreen() override {}
		void flip() override {}
		void queueTextTexture(const std::string& text,
		                      FontHandle         font,
		                      const Color&       color) override
		{
		}
		void processTextureQueue() override {}
		auto getTextTexture(const std::string& text)
		    -> std::shared_ptr<void> override
		{ return nullptr; }
		bool hasTextTexture(const std::string& text) const override
		{ return false; }

		virtual void blit(std::shared_ptr<void>,
		                  const Rectangle&) override
		{
		}
		virtual void drawFilledCircle(int32_t,
		                              int32_t,
		                              int32_t,
		                              const Color&) override
		{ ++circle_draw_count; }
		virtual void drawFilledRect(const Rectangle&,
		                            const Color&) override
		{ ++rect_draw_count; }

		void queueStateCommand(StateCommand cmd) override { (void)cmd; }
		void setStateCommandHandler(
		    std::function<void(StateCommand)> handler) override
		{ (void)handler; }
		void setFont(SdlPtr<TTF_Font>&& font) override { (void)font; }
		void drawText(const std::string& text,
		              const Rectangle&   bounds,
		              const Color&       color) override
		{
			(void)text;
			(void)bounds;
			(void)color;
		}
		void setMissingTextureBehavior(
		    MissingTextureBehavior behavior) override
		{ (void)behavior; }

		static int circle_draw_count;
		static int rect_draw_count;

	    public:
		MockRenderer() = default;
	};

	int MockRenderer::circle_draw_count{ 0 };
	int MockRenderer::rect_draw_count{ 0 };

}  // namespace

TEST_CASE("CircleViewComponent construction via factory")
{
	ComponentFactory<CircleViewComponent> factory;
	auto& comp = factory.create(Point{ 100, 200 }, 16, Color{ 255, 0, 0 });

	CHECK(comp.getPosition().x == 100);
	CHECK(comp.getPosition().y == 200);
	CHECK(comp.getRadius() == 16);
	CHECK(comp.getColor().r == 255);
	CHECK(comp.getColor().g == 0);
	CHECK(comp.getColor().b == 0);
	CHECK(comp.getTypeIndex() == typeid(CircleViewComponent));
	CHECK(&comp.getFactory() == &factory);
}

TEST_CASE("CircleViewComponent position mutation")
{
	ComponentFactory<CircleViewComponent> factory;
	auto& comp = factory.create(Point{ 50, 75 }, 8, Color{ 255, 255, 255 });

	comp.setPosition(Point{ 200, 300 });
	CHECK(comp.getPosition().x == 200);
	CHECK(comp.getPosition().y == 300);

	comp.setColor(Color{ 0, 255, 0 });
	CHECK(comp.getColor().g == 255);
}

TEST_CASE("CircleViewComponent radius mutation")
{
	ComponentFactory<CircleViewComponent> factory;
	auto& comp = factory.create(Point{ 0, 0 }, 10, Color{});

	comp.setRadius(20);
	CHECK(comp.getRadius() == 20);
}

TEST_CASE("CircleViewComponent produceDrawCommand returns FilledCircle")
{
	ComponentFactory<CircleViewComponent> factory;
	auto&                                 comp =
	    factory.create(Point{ 100, 100 }, 16, Color{ 255, 255, 255, 255 });

	MockRenderer renderer;
	auto         cmd = comp.produceDrawCommand(renderer);

	CHECK(cmd != nullptr);
	auto* shape_cmd = dynamic_cast<ShapeDrawCommand*>(cmd.get());
	CHECK(shape_cmd != nullptr);
	CHECK(shape_cmd->shape_type == ShapeType::FilledCircle);
	CHECK(shape_cmd->color.r == 255);
}

TEST_CASE("RectangleViewComponent construction via factory")
{
	ComponentFactory<RectangleViewComponent> factory;
	auto&                                    comp =
	    factory.create(Point{ 20, 296 }, 16, 96, Color{ 255, 255, 255 });

	CHECK(comp.getPosition().x == 20);
	CHECK(comp.getPosition().y == 296);
	CHECK(comp.getWidth() == 16);
	CHECK(comp.getHeight() == 96);
	CHECK(comp.getTypeIndex() == typeid(RectangleViewComponent));
}

TEST_CASE("RectangleViewComponent position and size mutation")
{
	ComponentFactory<RectangleViewComponent> factory;
	auto&                                    comp =
	    factory.create(Point{ 0, 0 }, 10, 20, Color{ 255, 255, 255 });

	comp.setPosition(Point{ 50, 100 });
	CHECK(comp.getPosition().x == 50);
	CHECK(comp.getPosition().y == 100);

	comp.setSize(30, 40);
	CHECK(comp.getWidth() == 30);
	CHECK(comp.getHeight() == 40);
}

TEST_CASE("RectangleViewComponent produceDrawCommand returns FilledRectangle")
{
	ComponentFactory<RectangleViewComponent> factory;
	auto&                                    comp =
	    factory.create(Point{ 10, 20 }, 16, 96, Color{ 255, 0, 0 });

	MockRenderer renderer;
	auto         cmd = comp.produceDrawCommand(renderer);

	CHECK(cmd != nullptr);
	auto* shape_cmd = dynamic_cast<ShapeDrawCommand*>(cmd.get());
	CHECK(shape_cmd != nullptr);
	CHECK(shape_cmd->shape_type == ShapeType::FilledRectangle);
	CHECK(shape_cmd->color.r == 255);
	CHECK(shape_cmd->color.g == 0);
	CHECK(shape_cmd->color.b == 0);
}

TEST_CASE("ShapeDrawCommand draw dispatches to renderer")
{
	MockRenderer renderer;

	SECTION("FilledCircle calls drawFilledCircle")
	{
		Rectangle        bounds{ 50, 50, 32, 32 };
		ShapeDrawCommand cmd(ShapeType::FilledCircle,
		                     bounds,
		                     Color{ 255, 255, 255 },
		                     renderer);
		auto             result = cmd.draw();
		CHECK(result == kSuccess);
		CHECK(renderer.circle_draw_count >= 1);
	}

	SECTION("FilledRectangle calls drawFilledRect")
	{
		Rectangle        bounds{ 10, 20, 16, 96 };
		ShapeDrawCommand cmd(ShapeType::FilledRectangle,
		                     bounds,
		                     Color{ 255, 255, 255 },
		                     renderer);
		auto             result = cmd.draw();
		CHECK(result == kSuccess);
		CHECK(renderer.rect_draw_count >= 1);
	}
}

TEST_CASE("Color struct construction")
{
	Color c1;
	CHECK(c1.r == 255);
	CHECK(c1.g == 255);
	CHECK(c1.b == 255);
	CHECK(c1.a == 255);

	Color c2{ 128, 64, 32, 200 };
	CHECK(c2.r == 128);
	CHECK(c2.g == 64);
	CHECK(c2.b == 32);
	CHECK(c2.a == 200);
}

TEST_CASE("id_t and idtype_t type aliases")
{
	elemental::id_t val = 42;
	CHECK(val == 42);
	CHECK(sizeof(elemental::id_t) == sizeof(uint32_t));

	elemental::idtype_t val2 = 100;
	CHECK(val2 == 100);
}

TEST_CASE("SceneConfig JSON serialization roundtrip")
{
	SceneConfig config;
	config.type       = "GameScene";
	config.kind       = "StaticViewPort";
	config.dimensions = Area{ 1280, 720 };

	EntityConfig entity;
	entity.entity_id = 1;
	entity.name      = "Ball";
	entity.type      = "Ball";
	entity.x         = 640;
	entity.y         = 360;
	entity.layer     = "foreground";

	ViewConfig view;
	view.shape  = "circle";
	view.radius = 8;
	view.color  = Color{ 255, 255, 255, 255 };
	entity.view = view;

	config.entities.push_back(entity);

	nlohmann::json j       = config;
	SceneConfig    config2 = j.get<SceneConfig>();

	CHECK(config2.type == "GameScene");
	CHECK(config2.kind == "StaticViewPort");
	CHECK(config2.dimensions.width == 1280);
	CHECK(config2.dimensions.height == 720);
	CHECK(config2.entities.size() == 1);
	CHECK(config2.entities[0].name == "Ball");
	CHECK(config2.entities[0].view.shape == "circle");
	CHECK(config2.entities[0].view.radius == 8);
}
