////////////////////////////////////////////////////////////////////////////////
//
// Ignition.h
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
//	This is the main application class. It handles the dialog creation.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _IGNITION_H_
#define _IGNITION_H_

#include "resource.h"

// Application states
#define IGNITION_STATE_STARTUP	0
#define IGNITION_STATE_RUNNING	1
#define IGNITION_STATE_SHUTDOWN	2

// Version string
#define IGNITION_VERSION	"Ignition 1.3 © 2000, 2001 Bruno 'Beosil' Heidelberger"

class CIgnitionApp : public CWinApp
{
public:
	// Get the current application state
	int GetState();

	// Get the current error of the application state
	int GetStateError();

	// Set the current application state
	void SetState(int state, int error);

protected:
	int m_state;
	int m_stateError;

	//{{AFX_VIRTUAL(CIgnitionApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIgnitionApp)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

extern CIgnitionApp theApp;

//{{AFX_INSERT_LOCATION}}

#endif
