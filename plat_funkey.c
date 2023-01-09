#include <SDL/SDL.h>
#include "libretro.h"
#include "libpicofe/plat.h"
#include "libpicofe/input.h"
#include "libpicofe/in_sdl.h"
#include "main.h"
#include "util.h"

#define MAX_SAMPLE_RATE 48000

static const struct in_default_bind in_sdl_defbinds[] = {
	{ SDLK_u, IN_BINDTYPE_PLAYER12, RETRO_DEVICE_ID_JOYPAD_UP },
	{ SDLK_d, IN_BINDTYPE_PLAYER12, RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ SDLK_l, IN_BINDTYPE_PLAYER12, RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ SDLK_r, IN_BINDTYPE_PLAYER12, RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ SDLK_b, IN_BINDTYPE_PLAYER12, RETRO_DEVICE_ID_JOYPAD_B },
	{ SDLK_a, IN_BINDTYPE_PLAYER12, RETRO_DEVICE_ID_JOYPAD_A },
	{ SDLK_x, IN_BINDTYPE_PLAYER12, RETRO_DEVICE_ID_JOYPAD_X },
	{ SDLK_y, IN_BINDTYPE_PLAYER12, RETRO_DEVICE_ID_JOYPAD_Y },
	{ SDLK_s, IN_BINDTYPE_PLAYER12, RETRO_DEVICE_ID_JOYPAD_START },
	{ SDLK_k, IN_BINDTYPE_PLAYER12, RETRO_DEVICE_ID_JOYPAD_SELECT },
	{ SDLK_m, IN_BINDTYPE_PLAYER12, RETRO_DEVICE_ID_JOYPAD_L },
	{ SDLK_n, IN_BINDTYPE_PLAYER12, RETRO_DEVICE_ID_JOYPAD_R },
	{ SDLK_v, IN_BINDTYPE_PLAYER12, RETRO_DEVICE_ID_JOYPAD_L2 },
	{ SDLK_o, IN_BINDTYPE_PLAYER12, RETRO_DEVICE_ID_JOYPAD_R2 },
	{ SDLK_q, IN_BINDTYPE_EMU, EACTION_MENU },
	{ SDLK_h, IN_BINDTYPE_EMU, EACTION_NEXT_SCALER },
	{ 0, 0, 0 }
};

const struct menu_keymap in_sdl_key_map[] =
{
	{ SDLK_u, PBTN_UP },
	{ SDLK_d, PBTN_DOWN },
	{ SDLK_l, PBTN_LEFT },
	{ SDLK_r, PBTN_RIGHT },
	{ SDLK_a, PBTN_MOK },
	{ SDLK_b, PBTN_MBACK },
	{ SDLK_x, PBTN_MA2 },
	{ SDLK_y, PBTN_MA3 },
	{ SDLK_m, PBTN_L },
	{ SDLK_n, PBTN_R },
	{ SDLK_q, PBTN_MENU },
};

const struct menu_keymap in_sdl_joy_map[] =
{
	{ SDLK_u, PBTN_UP },
	{ SDLK_d, PBTN_DOWN },
	{ SDLK_l, PBTN_LEFT },
	{ SDLK_r, PBTN_RIGHT },
	{ SDLK_WORLD_0, PBTN_MOK },
	{ SDLK_WORLD_1, PBTN_MBACK },
	{ SDLK_WORLD_2, PBTN_MA2 },
	{ SDLK_WORLD_3, PBTN_MA3 },
};

static const char * const in_sdl_key_names[SDLK_LAST] = {
	[SDLK_u]         = "UP",
	[SDLK_d]         = "DOWN",
	[SDLK_l]         = "LEFT",
	[SDLK_r]         = "RIGHT",
	[SDLK_x]         = "X",
	[SDLK_b]         = "B",
	[SDLK_a]         = "A",
	[SDLK_y]         = "Y",
	[SDLK_s]         = "START",
	[SDLK_k]         = "FN",
	[SDLK_m]         = "L",
	[SDLK_n]         = "R",
	[SDLK_v]         = "FN+L",
	[SDLK_o]         = "FN+R",
	[SDLK_1]         = "MENU+UP",
	[SDLK_2]         = "MENU+DOWN",
	[SDLK_3]         = "MENU+LEFT",
	[SDLK_4]         = "MENU+RIGHT",
	[SDLK_5]         = "MENU+B",
	[SDLK_6]         = "MENU+A",
	[SDLK_7]         = "MENU+X",
	[SDLK_8]         = "MENU+Y",
	[SDLK_9]         = "MENU+START",
	[SDLK_0]         = "MENU+SELECT",
	[SDLK_TAB]       = "MENU+L",
	[SDLK_BACKSLASH] = "MENU+R",
	[SDLK_q]         = "MENU",
};

static const struct mod_keymap in_sdl_mod_keymap[] = {
	{ SDLK_u,         SDLK_1 },
	{ SDLK_d,         SDLK_2 },
	{ SDLK_l,         SDLK_3 },
	{ SDLK_r,         SDLK_4 },
	{ SDLK_b,         SDLK_5 },
	{ SDLK_a,         SDLK_6 },
	{ SDLK_x,         SDLK_7 },
	{ SDLK_y,         SDLK_8 },
	{ SDLK_s,         SDLK_9 },
	{ SDLK_k,         SDLK_0 },
	{ SDLK_TAB,       SDLK_TAB },          /* mod+L1 = L2 */
	{ SDLK_BACKSPACE, SDLK_BACKSLASH },  /* mod+R1 = R2 */
};

static const struct in_pdata in_sdl_platform_data = {
	.defbinds  = in_sdl_defbinds,
	.key_map   = in_sdl_key_map,
	.kmap_size = array_size(in_sdl_key_map),
	.joy_map   = in_sdl_joy_map,
	.jmap_size = array_size(in_sdl_joy_map),
	.key_names = in_sdl_key_names,
	/* .mod_key      = SDLK_ESCAPE,
	 * .mod_keymap   = in_sdl_mod_keymap,
	 * .modmap_size  = array_size(in_sdl_mod_keymap), */
};

#include "plat_sdl.c"
