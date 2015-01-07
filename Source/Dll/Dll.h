////////////////////////////////////////////////////////////////////////////////
//
// Dll.h
//
// Copyright (C) 2000 Bruno 'Beosil' Heidelberger
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	This is the main DLL class. It handles all startup and hooking.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _DLL_H_
#define _DLL_H_

#include "resource.h"

// Error codes
#define IGNITION_ERROR_NONE				0
#define IGNITION_ERROR_OVERFLOW			1
#define IGNITION_ERROR_NOTARGET			2
#define IGNITION_ERROR_CREATEPROCESS	3
#define IGNITION_ERROR_LOADDLL			4
#define IGNITION_ERROR_NOPATCH			5
#define IGNITION_ERROR_PROTECT			6
#define IGNITION_ERROR_MODULE_LOAD		7
#define IGNITION_ERROR_MODULE_INSTALL	8

#define IGNITION_MAX_BUFFERSIZE	1024

typedef int (*InstallFunc)(unsigned int checksum, unsigned int length);

class CDllApp : public CWinApp
{
public:
	// Install DLL and patch target
	int InstallDll();

	//{{AFX_VIRTUAL(CDllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDllApp)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

// Start target process and hook it ( exported function )
extern int Startup(CString& strConfigFilename, CString& strCommandLine, CString& strPatch);

// Get error text ( exported function )
extern char *GetErrorText(int error);

//{{AFX_INSERT_LOCATION}}

#endif
