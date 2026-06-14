// clang-format off
/*
  * Copyright © 2024 Saul D. Benitez
  * License: Mozilla Public License v. 2.0
  *
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v2.0. If a copy of the MPL was not distributed with this file,
  * You can obtain one at https://mozilla.org/MPL/2.0/.
  */
// clang-format on

#include "IOCore/Exception.hpp"

#include "DrawCommand.hpp"

#include <IOCore/TomlTable.hpp>
#include <test-utils/common.hpp>

namespace {
using namespace elemental;

class DummyRenderer : public IRenderer
{
    public:
	static auto getInstance() -> DummyRenderer&
	{
		static DummyRenderer instance;
		return instance;
	}

	explicit DummyRenderer() = default;

	void init(RendererSettings& settings) override {}
	void queueTextTexture(const std::string& text,
	                      FontHandle         font,
	                      const Color&       color) override
	{
	}
	void                  processTextureQueue() override {}
	std::shared_ptr<void> getTextTexture(const std::string& text) override
	{ return nullptr; }
	bool hasTextTexture(const std::string& text) const override
	{ return false; }

	auto isInitialized() -> bool override { return true; }

	void deactivate() override {}

	void clearScreen() override {}

	void flip() override {}

	void blit(std::shared_ptr<void> image_data,
	          const Rectangle&      placement) override
	{
	}

	void drawFilledCircle(int32_t      x,
	                      int32_t      y,
	                      int32_t      radius,
	                      const Color& color) override
	{
	}

	void drawFilledRect(const Rectangle& rect, const Color& color) override
	{
	}

	[[nodiscard]] auto getResolution() -> Resolution override
	{
		return { 0, 0 };  // Default resolution
	}

	[[nodiscard]] auto getWindowSize() -> Area override
	{
		return { 0, 0 };  // Default window size
	}

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
	void setMissingTextureBehavior(MissingTextureBehavior behavior) override
	{ (void)behavior; }

	template<typename TR>
	static auto toRectangle(const TR& data) -> Rectangle
	{
		return {};  // Placeholder implementation
	}

	template<typename TR>
	static auto fromRectangle(const Rectangle& rectangle) -> TR
	{ return {}; }
};
}  // namespace

BEGIN_TEST_SUITE("elemental::DrawCommand")
{
	using namespace elemental;

	TEST_CASE("Basic DrawCommand construction")
	{
		Rectangle             rect{ { 0, 0 }, { 100, 200 } };
		std::shared_ptr<void> image_data =
		    std::make_shared<SDL_Surface>();
		DummyRenderer renderer;

		DrawCommand cmd(renderer, rect, image_data);
	}

	TEST_CASE("DrawCommand rectangle getter")
	{
		Rectangle             rect{ { 0, 0 }, { 100, 200 } };
		std::shared_ptr<void> image_data = nullptr;
		DummyRenderer         renderer;

		DrawCommand cmd(renderer, rect, image_data);

		// Test rectangle() returns correct value
		auto result_rect = cmd.rectangle();
		REQUIRE(result_rect.position.x == rect.position.x);
		REQUIRE(result_rect.position.y == rect.position.y);
	}

	TEST_CASE("DrawCommand image data getter")
	{
		Rectangle             rect{ { 0, 0 }, { 100, 200 } };
		std::shared_ptr<void> image_data = nullptr;
		DummyRenderer         renderer;

		DrawCommand cmd(renderer, rect, image_data);

		// Test imageData() returns the correct shared_ptr
		auto result_data = cmd.imageData();
		REQUIRE(result_data.get() == image_data.get());
	}

	TEST_CASE("DrawCommand draw method success case")
	{
		Rectangle             rect{ { 0, 0 }, { 100, 200 } };
		std::shared_ptr<void> image_data = nullptr;
		DummyRenderer         renderer;

		DrawCommand cmd(renderer, rect, image_data);

		auto result = cmd.draw();
		REQUIRE(result == kSuccess);
	}

	TEST_CASE("DrawCommand draw method error case")
	{
		Rectangle             rect{ { 0, 0 }, { 100, 200 } };
		std::shared_ptr<void> image_data = nullptr;

		// Override render() to throw an exception
		class FaultyRenderer : public DummyRenderer
		{
		    public:
			void blit(std::shared_ptr<void> image_data,
			          const Rectangle&      placement) override
			{ throw std::runtime_error("Rendering failed"); }
		} renderer;

		DrawCommand cmd(renderer, rect, image_data);

		auto result = cmd.draw();
		REQUIRE(result == kError);
	}
}
// clang-format off
// vim: set  textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
