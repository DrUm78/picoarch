#include "overrides.h"

static const struct core_override_option pcsx_rearmed_core_option_overrides[] = {
	{
		.key = "pcsx_rearmed_region",
		.info = "Specify which region the system is from. 'NTSC' is 60 Hz while 'PAL' is 50 Hz. 'Auto' will detect the region of the currently loaded content.",
	},
	{
		.key = "pcsx_rearmed_bios",
		.info = "Specify which BIOS to use. 'Auto' will attempt to load a real bios file. 'HLE' forces BIOS emulation, with lower compatibility.",
	},
	{
		.key = "pcsx_rearmed_show_bios_bootlogo",
		.info = "When using an official BIOS, whether to show the PlayStation logo at startup. Showing the boot logo may reduce compatibility.",
	},
	{
		.key = "pcsx_rearmed_drc",
		.info = "Dynamically recompile PSX CPU instructions to native instructions. Much faster, but may be less accurate.",
	},
	{
		.key = "pcsx_rearmed_frameskip_type",
		.info = "Skip frames to avoid audio crackling. Improves performance at the expense of visual smoothness. Will cause graphical glitches.",
		.default_value = "auto",
		.options = {
			[2] = {"auto_threshold", "Threshold"},
			[3] = {"fixed_interval", "Fixed"},
		}
	},
	{
		.key = "pcsx_rearmed_frameskip_threshold",
		.desc = "FS Threshold (%%)",
		.info = "When 'Frameskip' is set to 'Threshold', sets how low the audio buffer can get before frames will be skipped.",
	},
	{
		.key = "pcsx_rearmed_frameskip",
		.desc = "FS Interval",
		.info = "Specifies the maximum number of frames that can be skipped before a new frame is rendered.",
		.default_value = "4"
	},
	{
		.key = "pcsx_rearmed_memcard2",
		.desc = "2nd Memory Card",
	},
	{
		.key = "pcsx_rearmed_dithering",
		.desc = "Dithering",
		.info = "If disabled, turns off the dithering pattern the PSX applies to combat color banding.",
	},
	{
		.key = "pcsx_rearmed_duping_enable",
		.desc = "Frame Duping",
		.info = "When enabled, provides a small performance improvement by repeating the previous frame if the core has nothing new to display.",
	},
	{
		.key = "pcsx_rearmed_show_gpu_peops_settings",
		.desc = "P.E.Op.S. Settings",
	},
	{
		.key = "pcsx_rearmed_gpu_peops_odd_even_bit",
		.desc = "Odd/Even Hack",
	},
	{
		.key = "pcsx_rearmed_gpu_peops_expand_screen_width",
		.desc = "Exp. Screen Width",
	},
	{
		.key = "pcsx_rearmed_gpu_peops_ignore_brightness",
		.desc = "Ignore Brightness",
	},
	{
		.key = "pcsx_rearmed_gpu_peops_disable_coord_check",
		.desc = "Coordinate Check",
	},
	{
		.key = "pcsx_rearmed_gpu_peops_lazy_screen_update",
		.desc = "Lazy Screen Update",
	},
	{
		.key = "pcsx_rearmed_gpu_peops_old_frame_skip",
		.desc = "Old Frameskip",
	},
	{
		.key = "pcsx_rearmed_gpu_peops_repeated_triangles",
		.desc = "Rep. Triangles",
	},
	{
		.key = "pcsx_rearmed_gpu_peops_quads_with_triangles",
		.desc = "Quads w/ Triangles",
	},
	{
		.key = "pcsx_rearmed_gpu_peops_fake_busy_state",
		.desc = "Fake 'Busy' States",
	},
	{
		.key = "pcsx_rearmed_show_gpu_unai_settings",
		.desc = "GPU UNAI Settings",
		.info = "Shows or hides advanced gpu settings. Menu must be toggled for this setting to take effect."
	},
	{
		.key = "pcsx_rearmed_gpu_unai_blending",
		.desc = "Blending",
	},
	{
		.key = "pcsx_rearmed_gpu_unai_lighting",
		.desc = "Lighting",
	},
	{
		.key = "pcsx_rearmed_gpu_unai_fast_lighting",
		.desc = "Fast Lighting",
	},
	{
		.key = "pcsx_rearmed_gpu_unai_ilace_force",
		.desc = "Forced Interlace",
	},
	{
		.key = "pcsx_rearmed_gpu_unai_pixel_skip",
		.desc = "Pixel Skip",
	},
	{
		.key = "pcsx_rearmed_gpu_unai_scale_hires",
		.desc = "Hi-Res Scaling",
		.default_value = "enabled"
	},
	{
		.key = "pcsx_rearmed_gpu_slow_llists",
		.desc = "Slow Linked Lists",
	},
	{
		.key = "pcsx_rearmed_neon_interlace_enable",
		.desc = "Interlaced Video",
		.info = "When enabled, games that run in high resolution video modes (480i, 512i) produce interlaced video output.",
	},
	{
		.key = "pcsx_rearmed_spu_interpolation",
		.info = "Emulates audio interpolation. 'Gaussian' is closest to original. 'Simple' is faster, lower quality. 'Cubic' is slowest, higher clarity.",
	},
	{
		.key = "pcsx_rearmed_noxadecoding",
		.info = "Enable playback of XA audio tracks. Can be disabled to improve performance in games that include XA audio, at the expense of missing music.",
	},
	{
		.key = "pcsx_rearmed_neon_enhancement_enable",
		.blocked = true,
	},
	{
		.key = "pcsx_rearmed_neon_enhancement_no_main",
		.blocked = true,
	},
	{
		.key = "pcsx_rearmed_gpu_thread_rendering",
		.info = "Runs GPU commands in a thread. Sync waits for drawing to finish before vsync. Async will not wait unless there's another frame behind it."
	},
	{
		.key = "pcsx_rearmed_spu_reverb",
		.desc = "Reverb Effects",
		.info = "Enable emulation of the PSX reverb feature. Disable to improve performance at the expense of reduced audio quality/authenticity.",
	},
	{
		.key = "pcsx_rearmed_pe2_fix",
		.desc = "PE 2/VH 1/2 Fix",
		.info = "Fixes Parasite Eve 2 / Vandal Hearts 1 / 2."
	},
	{
		.key = "pcsx_rearmed_inuyasha_fix",
		.desc = "InuYasha Battle Fix",
	},
	{
		.key = "pcsx_rearmed_async_cd",
		.desc = "CD Access (Restart)",
		.info = "Method used to read data from disk. 'Asynchronous' can reduce stuttering on devices with slow storage. 'Precache' loads disk image into memory (CHD only)."
	},
	{
		.key = "pcsx_rearmed_spuirq",
		.desc = "SPU IRQ Always",
	},
	{
		.key = "pcsx_rearmed_nosmccheck",
		.desc = "Disable SMC Checks",
	},
	{
		.key = "pcsx_rearmed_gteregsunneeded",
		.desc = "Unneeded GTE Regs",
	},
	{
		.key = "pcsx_rearmed_nogteflags",
		.desc = "Disable GTE Flags",
	},
	{
		.key = "pcsx_rearmed_nostalls",
		.desc = "Disable Stalls",
	},
	{
		.key = "pcsx_rearmed_show_input_settings",
		.blocked = true
	},
	{
		.key = "pcsx_rearmed_input_sensitivity",
		.blocked = true
	},
	{
		.key = "pcsx_rearmed_multitap",
		.blocked = true
	},
	{
		.key = "pcsx_rearmed_vibration",
		.blocked = true
	},
	{
		.key = "pcsx_rearmed_display_internal_fps",
		.blocked = true
	},
	{
		.key = "pcsx_rearmed_icache_emulation",
		.blocked = true
	},
	{
		.key = "pcsx_rearmed_nocompathacks",
		.blocked = true
	},
	{ NULL }
};

me_bind_action pcsx_rearmed_ctrl_actions[] =
{
	{ "UP       ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "DOWN     ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "LEFT     ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "RIGHT    ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "CIRCLE   ",  1 << RETRO_DEVICE_ID_JOYPAD_A },
	{ "CROSS    ",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ "TRIANGLE ",  1 << RETRO_DEVICE_ID_JOYPAD_X },
	{ "SQUARE   ",  1 << RETRO_DEVICE_ID_JOYPAD_Y },
	{ "START    ",  1 << RETRO_DEVICE_ID_JOYPAD_START },
	{ "SELECT   ",  1 << RETRO_DEVICE_ID_JOYPAD_SELECT },
	{ "L1 BUTTON",  1 << RETRO_DEVICE_ID_JOYPAD_L },
	{ "R1 BUTTON",  1 << RETRO_DEVICE_ID_JOYPAD_R },
	{ "L2 BUTTON",  1 << RETRO_DEVICE_ID_JOYPAD_L2 },
	{ "R2 BUTTON",  1 << RETRO_DEVICE_ID_JOYPAD_R2 },
	{ NULL,       0 }
};

const struct core_override_fast_forward pcsx_rearmed_fast_forward = {
	.type_key = "pcsx_rearmed_frameskip_type",
	.interval_key = "pcsx_rearmed_frameskip_interval"
};

const struct core_override_startup_msg pcsx_rearmed_startup_msg = {
	.msg = "Loading...",
	.msec = 3000,
};

#define pcsx_rearmed_overrides {                               \
	.core_name = "pcsx_rearmed",                           \
	.fast_forward = &pcsx_rearmed_fast_forward,            \
	.startup_msg = &pcsx_rearmed_startup_msg,              \
	.actions = pcsx_rearmed_ctrl_actions,                  \
	.action_size = array_size(pcsx_rearmed_ctrl_actions),  \
	.options = pcsx_rearmed_core_option_overrides,         \
	.defer_frames = 3,                                     \
}
