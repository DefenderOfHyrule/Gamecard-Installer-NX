#include <stdio.h>
#include <stdbool.h>
#include <switch.h>
#include <SDL2/SDL.h>

#include "gfx/SDL_easy.h"
#include "gfx/shape.h"
#include "gfx/font.h"

#include "sound/sound.h"
#include "sound/musicnx.h"

#include "nx/es.h"
#include "nx/ns.h"
#include "nx/ncm.h"
#include "nx/crypto.h"
#include "nx/set.h"
#include "nx/lbl.h"
#include "nx/input.h"

#include "ui/menu.h"
#include "ui/gc.h"

#include "util/dir.h"
#include "util/file.h"
#include "util/log.h"


#define APP_DIR     "sdmc:/switch/gamecard_installer"
#define APP_PATH    "sdmc:/switch/gamecard_installer/gamecard_installer.nro"


void app_init()
{
	SDL_EasyInit(SDL_WINDOW_SHOWN, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	#ifdef DEBUG
	init_log();
	#endif

	es_start_service();
	init_lbl();
	init_ns();
	ncm_init();
	init_set();
	init_crypto();
	init_sound_default();
	init_musicnx();
	init_font();
	init_input();
	init_menu();
	init_gc();
}

void app_exit()
{
	#ifdef DEBUG
	exit_log();
	#endif

	es_close_service();
	exit_menu();
	exit_gc();
	exit_crypto();
	exit_ns();
	ncm_init();
	exit_lbl();
	exit_set();
	exit_font();
	exit_musicnx();
	exit_sound();
	SDL_EasyExit();
}

void setup_app_dir(const char *nro)
{
    if (strcmp(APP_PATH, nro) == 0)
        return;
    if (!check_if_dir_exists(APP_DIR))
        create_dir(APP_DIR);
    if (check_if_file_exists(APP_PATH))
        delete_file(APP_PATH);
    move_file(nro, APP_PATH);
}

int main(int argc, char *argv[])
{
	// init everything.
	appletLockExit();
	app_init();
	
	// setup the app dir and move the nro to the folder.
	setup_app_dir(argv[0]);

	//appletBeginBlockingHomeButtonShortAndLongPressed(0);

	//appletGetAppletType();

	// goto the menu.
	start_menu();

	// cleanup before exiting.
	//appletEndBlockingHomeButtonShortAndLongPressed();
	app_exit();
	appletUnlockExit();
	return 0;
}