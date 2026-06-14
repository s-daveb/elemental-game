#include "MainMenu.hpp"

#include "elemental/IRenderer.hpp"
#include "elemental/TextDrawCommand.hpp"
#include "elemental/types/rendering.hpp"

#include "Phong.hpp"

#include <SDL_events.h>
#include <SDL_scancode.h>

#include <any>

namespace elemental {

MainMenu::MainMenu(IRenderer& r, FontHandle f) : IState(), renderer(r), font(f)
{ queueTextures(); }

void MainMenu::queueTextures()
{
	for (const auto& item: menu_items) {
		renderer.queueTextTexture(
		    item, font, Color{ 255, 255, 255, 255 });
		renderer.queueTextTexture(
		    item, font, Color{ 255, 255, 0, 255 });
	}
}

auto MainMenu::step() -> void {}

auto MainMenu::receiveMessage(const Observable&, std::any message) -> void
{
	if (!message.has_value()) return;
	handleInput(std::move(message));
}

void MainMenu::handleInput(std::any message)
{
	try {
		auto event = std::any_cast<SDL_Event>(message);

		if (event.type != SDL_KEYDOWN) return;
		switch (event.key.keysym.scancode) {
		case SDL_SCANCODE_UP:
			if (selected_index > 0) --selected_index;
			break;
		case SDL_SCANCODE_DOWN:
			if (selected_index < menu_items.size() - 1)
				++selected_index;
			break;
		case SDL_SCANCODE_RETURN:
		case SDL_SCANCODE_SPACE:
			switch (selected_index) {
			case 0:  // Start Game
				renderer.queueStateCommand(
				    StateCommand::PushGame);
				break;
			case 1: {  // Exit
				SDL_Event quit{};
				quit.type = SDL_QUIT;
				SDL_PushEvent(&quit);
				break;
			}
			}
			break;
		default: break;
		}
	} catch (const std::bad_any_cast&) {
	}
}

auto MainMenu::getItemColor(std::size_t idx) const -> Color
{
	return (idx == selected_index) ? Color{ 255, 255, 0, 255 }
	                               : Color{ 255, 255, 255, 255 };
}

auto MainMenu::getDrawCommands() -> std::list<std::shared_ptr<IDrawCommand>>
{
	std::list<std::shared_ptr<IDrawCommand>> cmds;
	const int                                cx = 1280 / 2;
	int                                      y  = kMenuStartY;
	for (std::size_t i = 0; i < menu_items.size(); ++i) {
		Rectangle bounds{ static_cast<uint32_t>(cx - 100),
			          static_cast<uint32_t>(y),
			          200,
			          50 };
		cmds.push_back(
		    std::make_shared<TextDrawCommand>(
		        menu_items[i], bounds, getItemColor(i), renderer));
		y += kMenuItemHeight + kMenuItemSpacing;
	}
	return cmds;
}

}  // namespace elemental
