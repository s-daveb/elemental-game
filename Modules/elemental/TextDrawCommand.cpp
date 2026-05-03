#include "TextDrawCommand.hpp"

#include "IRenderer.hpp"
#include "SdlRenderer.hpp"

#include <SDL.h>
#include <SDL_render.h>
#include <SDL_ttf.h>

namespace elemental {

std::shared_ptr<void> TextDrawCommand::null_data = nullptr;

auto TextDrawCommand::draw() -> ErrorFlag
{
	// Fetch pre-created texture from renderer cache
	auto texture = renderer.getTextTexture(text);

	if (!texture) {
		// Texture hasn't been created yet - skip drawing
		// The texture should have been queued via queueTextTexture()
		return IOCore::kSuccess;  // Not an error, just not ready yet
	}

	// Get SDL renderer from IRenderer
	// This assumes SdlRenderer is the actual type
	auto&         sdl_renderer = dynamic_cast<SdlRenderer&>(renderer);
	SDL_Renderer* sdl_r        = sdl_renderer.get<SDL_Renderer*>();

	// Set texture color mod if needed (for color changes)
	SDL_SetTextureColorMod(
	    static_cast<SDL_Texture*>(texture.get()),
	    color.r,
	    color.g,
	    color.b);
	SDL_SetTextureAlphaMod(
	    static_cast<SDL_Texture*>(texture.get()), color.a);

	// Render the texture
	SDL_Rect dest_rect = { static_cast<int>(bounds.x),
		               static_cast<int>(bounds.y),
		               static_cast<int>(bounds.width),
		               static_cast<int>(bounds.height) };

	SDL_RenderCopy(
	    sdl_r,
	    static_cast<SDL_Texture*>(texture.get()),
	    nullptr,
	    &dest_rect);

	return IOCore::kSuccess;
}

// Optional prepare method to queue the texture

void TextDrawCommand::prepare(FontHandle font)
{
	auto ttf_font = static_cast<TTF_Font*>(font.get());
	renderer.queueTextTexture(text, font, color);
}

}  // namespace elemental
