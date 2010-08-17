/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

/**
 * @brief error messages
 * Error handling based on platform.
 */

#include <StdAfx.h>
#include "errorhandler.h"

#include "LogOutput.h"
#include "Game/GameServer.h"

#ifndef DEDICATED
	#include "Sound/ISound.h"
	#include <SDL.h>

  #ifndef HEADLESS
    #ifdef WIN32
	#include <windows.h>
    #else
	// from X_MessageBox.cpp:
	void X_MessageBox(const char *msg, const char *caption, unsigned int flags);
    #endif
  #endif // ifndef HEADLESS
#endif // ifndef DEDICATED


void ErrorMessageBox(const std::string& msg, const std::string& caption, unsigned int flags)
{
	logOutput.SetSubscribersEnabled(false);
	LogObject() << caption << " " << msg;

	// not exiting threads causes another exception
	delete gameServer; gameServer = NULL;
#ifndef DEDICATED
	SDL_Quit();
	ISound::Shutdown();
#endif

#if !defined(DEDICATED) && !defined(HEADLESS)
  #ifdef WIN32
	// Windows implementation, using MessageBox.

	// Translate spring flags to corresponding win32 dialog flags
	unsigned int winFlags = 0;		// MB_OK is default (0)
	
	if (flags & MBF_EXCL)
		winFlags |= MB_ICONEXCLAMATION;
	if (flags & MBF_INFO)
		winFlags |= MB_ICONINFORMATION;

	MessageBox(GetActiveWindow(), msg.c_str(), caption.c_str(), winFlags);

  #else  // ifdef WIN32
	// X implementation
	// TODO: write Mac OS X specific message box
	X_MessageBox(msg.c_str(), caption.c_str(), flags);

  #endif // ifdef WIN32
#endif // if !defined(DEDICATED) && !defined(HEADLESS)

	exit(-1); // continuing execution when SDL_Quit has already been run will result in a crash
}

