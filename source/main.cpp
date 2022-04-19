#include <cstdio>
#include <stdlib.h>
#include <gccore.h>
#include <unistd.h>
#include <wiiuse/wpad.h>
#include "httpii_network.h"
#include "utils.h"

/**
 * @brief Initialize low-level Wii stuff
 * @details Initialize Video and Console stuff
 */
void subsystem_init();

/**
 * @brief Simple HTTP/1.1 Wii Homebrew web server with SD card webroot and native internet interface support
 */
int main(int argc, char **argv)
{
	subsystem_init();
	// initialize web server thread
	HTTPii::HTTPd::start();

	while (1)
	{
		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();
		// WPAD_ButtonsDown tells us which buttons were pressed in this loop
		// this is a "one shot" state which will not fire again until the button has been released
		u32 pressed = WPAD_ButtonsDown(0);
		// We return to the launcher application via exit
		if (pressed & WPAD_BUTTON_HOME)
		{
			HTTPii::Utils::clear_screen();
			printf("Exiting...  ");
			exit(0);
		}
		// Wait for the next frame
		VIDEO_WaitVSync();
	}

	return 0;
}

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

void subsystem_init()
{
	VIDEO_Init();
	WPAD_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if (rmode->viTVMode & VI_NON_INTERLACE)
		VIDEO_WaitVSync();
}
