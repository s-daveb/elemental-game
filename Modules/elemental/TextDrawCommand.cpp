#include "TextDrawCommand.hpp"

#include "IRenderer.hpp"

namespace elemental {

std::shared_ptr<void> TextDrawCommand::null_data = nullptr;

auto TextDrawCommand::draw() -> ErrorFlag
{
	renderer.drawText(text, bounds, color);
	return IOCore::kSuccess;
}

void TextDrawCommand::prepare(FontHandle font)
{ renderer.queueTextTexture(text, font, color); }

}  // namespace elemental
