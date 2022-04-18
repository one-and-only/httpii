#include <cstdio>
#include <stdlib.h>
#include <gccore.h>
#include <unistd.h>
#include <wiiuse/wpad.h>
#include "httpii_network.h"
#include "utils.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

/**
 * @brief Simple HTTP/1.1 Wii Homebrew web server with SD card webroot and native internet interface support
 */
int main(int argc, char **argv)
{
	// Initialise the video system
	VIDEO_Init();
	// This function initialises the attached controllers
	WPAD_Init();
	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);
	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	// Initialise the console, required for printf
	console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);
	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);
	// Make the display visible
	VIDEO_SetBlack(FALSE);
	// Flush the video register changes to the hardware
	VIDEO_Flush();
	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if (rmode->viTVMode & VI_NON_INTERLACE)
		VIDEO_WaitVSync();

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