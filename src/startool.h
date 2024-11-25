
#ifndef STARTOOL_H
#define STARTOOL_H

/*----------------------------------------------------------------------------
 --  General
 ----------------------------------------------------------------------------*/

#define VERSION "3.3.0" // Version of extractor startool

const char NameLine[] =
    "startool V" VERSION " for Stratagus (c) 2002-2022 by the Stratagus Project.\n";

/*----------------------------------------------------------------------------
 --  Includes
 ----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// C++
#include <iostream>
#include <string>

#ifdef _MSC_VER
#define PATH_MAX _MAX_PATH
#include <windows.h>
#include <direct.h>
#include <io.h>
#else
#include <limits.h>
#include <unistd.h>
#endif
#include <ctype.h>
#include <png.h>

#include "Storm.h"

#ifndef __GNUC__
	#define __attribute__(args)  // Does nothing for non GNU CC
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

using namespace std;

//----------------------------------------------------------------------------
//  Config
//----------------------------------------------------------------------------

/**
 **		Path the sound files. (default=$DIR/sounds)
 */
#define SOUND_PATH		"sounds"

/**
 **  Conversion control sturcture.
 */
typedef struct _control_
{
  int Type;          /// Entry type
  int Version;       /// Only in this version
  const char *File;    /// Save file
  const char *ArcFile;    /// File name in list file
  int Arg1;          /// Extra argument 1
  int Arg2;          /// Extra argument 2
  int Arg3;          /// Extra argument 3
  int Arg4;          /// Extra argument 4
} Control;

/**
 **  Possible entry types of archive file.
 */
enum _archive_type_
{
  S,    // Setup
  F,    // File                          (name)
  M,    // Map                           (name)
  T,    // Tileset                       (name,pal,mega,mini,map)
  R,    // RGB -> gimp                   (name,rgb)
  G,    // Graphics                      (name,pal,gfx)
  U,    // Uncompressed Graphics         (name,pal,gfu)
  I,    // Widgets                       (name,pal,gfu)
  N,    // Font                          (name,idx)
  W,    // Wav                           (name,wav)
  H,    // Pcx                           (name)
  E,    // Raw extract                   (name)
  V,    // SMK Video                     (name,video)
  L,    // Campaign Levels
  Q,    // MPQ archive
  D,    // Graphics in DDS format
  P,    // SMK Portraits
  PAL,  // Palette from pcx file
  WPE   // Palette from wpe file
};

#define __3 ,0,0,0
#define __4 ,0,0,0,0

#include "startool_mpq.h"
#include "startool_cd.h"
#include "startool_rm.h"

#endif // STARTOOL_H
