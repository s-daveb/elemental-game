/* MainMenu.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/types/errors.hpp"

#include "elemental/IDrawCommand.hpp"
#include "elemental/IState.hpp"
#include "elemental/Observable.hpp"
#include "elemental/types/color.hpp"
#include "elemental/types/rendering.hpp"

#include <SDL_events.h>

#include <any>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace elemental {

class IRenderer;

class MainMenu : public IState
{
    public:
	explicit MainMenu(IRenderer& renderer, FontHandle font);
	~MainMenu() override = default;

	auto step() -> void override;
	auto receiveMessage(const Observable&, std::any) -> void override;

	auto getDrawCommands()
	    -> std::list<std::shared_ptr<IDrawCommand>> override;

    private:
	IRenderer&               renderer;
	FontHandle               font;
	std::size_t              selected_index{ 0 };
	std::vector<std::string> menu_items{ "Start Game", "Exit" };

	static constexpr int kMenuStartY      = 300;
	static constexpr int kMenuItemHeight  = 60;
	static constexpr int kMenuItemSpacing = 20;

	void handleInput(const SDL_Event& event);
	void queueTextures();
	auto getItemColor(std::size_t index) const -> Color;
};

}  // namespace elemental

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
