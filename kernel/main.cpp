
#include <os/os.hpp>
#include <memory.hpp>
#include "../games/invaders/invaders.hpp"
#include <graphics/texture.hpp>
#include <graphics/graphics.hpp>
#include <graphics/text.hpp>
void update_game_logic(void);

extern uint8_t _binary_assets_vga_font_dos_gob_start;
extern uint8_t _binary_assets_vga_font_dos_gob_end;

extern uint8_t _binary_assets_colortest_bmp_start;
extern uint8_t _binary_assets_colortest_bmp_end;

extern uint8_t _binary_assets_checker_pattern_bmp_start;
extern uint8_t _binary_assets_checker_pattern_bmp_end;
/// This is the "main" main, which will just be running a menu and switch between games
void main()
{
	using namespace os::graphics;

	// Texture *fontmap = Texture::fromGob(&_binary_assets_vga_font_dos_gob_start, &_binary_assets_vga_font_dos_gob_end);
	// // Texture *checkerTest = Texture::fromBmp(&_binary_assets_checker_pattern_bmp_start, &_binary_assets_checker_pattern_bmp_end);
	// //  //Texture *fontmap = Texture::fromBmp(&_binary_assets_colortest_bmp_start, &_binary_assets_colortest_bmp_end);
	// //  Graphics::getInstance().clearScreen(Color(0, 0, 0));
	// //  if (!fontmap)
	// //  {
	// //  	panic_with_message("Failed to load fontmap");
	// //  }
	// fontmap->resize(Vec2i(fontmap->getWidth(), fontmap->getHeight() * 6));
	// Graphics::getInstance().drawTexture(fontmap, Vec2i(64, 33));

	// Text t = Text(&FontManager::getInstance().getDefaultFont());
	// t.setText("you will never see this");
	// // t.setText("#nya !boobs? ^-^ 0_0 T~T `BANANANA` .....//// $$ {}");
	// t.draw();
	// hcf();
	Games::Invaders::init();
	GameManager::getInstance().setInputHandler(Games::Invaders::handleInput);
	GameManager::getInstance().setUpdateHandler(Games::Invaders::updateGameLogic);
	hcf();
}