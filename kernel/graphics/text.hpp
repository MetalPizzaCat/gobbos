#pragma once

#include <graphics/texture.hpp>
#include <graphics/font.hpp>
#include <string.hpp>
#include <graphics/drawable.hpp>
namespace os::graphics
{
	class Text : public Drawable
	{
	public:
		explicit Text(Font const *font);
		void setText(std::string const &text);
		void draw() override;

		virtual ~Text();

	private:
		Font const *m_font;
		Texture *m_resultTexture;
		uint64_t m_lineSpacing;
	};
} // namespace os::graphics
