////////////////////////////////////////////////////////////////////////////////
//
// Ignition.cpp
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

#include "StdAfx.h"
#include "Ignition.h"
#include "IgnitionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



////////////////////////////////////////////////////////////////////////////////
//
//	Windows message map
//
////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CIgnitionApp, CWinApp)
	//{{AFX_MSG_MAP(CIgnitionApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()



////////////////////////////////////////////////////////////////////////////////
//
//	The one and only application instance
//
////////////////////////////////////////////////////////////////////////////////

CIgnitionApp theApp;



////////////////////////////////////////////////////////////////////////////////
//
//	Windows message hook
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		BOOL	"false" -> application will exit after dialog is closed
//
////////////////////////////////////////////////////////////////////////////////

BOOL CIgnitionApp::InitInstance()
{
#ifdef _AFXDLL
	Enable3dControls();
#else
	Enable3dControlsStatic();
#endif

	m_state = IGNITION_STATE_STARTUP;
	m_stateError = 0;

	m_pMainWnd = &theDlg;

	theDlg.DoModal();

	return FALSE;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Get the current application state
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		int		Current state
//
////////////////////////////////////////////////////////////////////////////////

int CIgnitionApp::GetState()
{
	return m_state;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Get the current error of the application state
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		int		Current state
//
////////////////////////////////////////////////////////////////////////////////

int CIgnitionApp::GetStateError()
{
	return m_stateError;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Set the current application state
//
//	PARAMETERS:
//		int state		New state
//		int error		Error code of new state
//
//	RETURNS:
//		-none-
//
////////////////////////////////////////////////////////////////////////////////

void CIgnitionApp::SetState(int state, int error)
{
	m_state = state;
	m_stateError = error;
}
