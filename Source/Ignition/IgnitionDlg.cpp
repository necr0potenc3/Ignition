////////////////////////////////////////////////////////////////////////////////
//
// IgnitionDlg.cpp
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
//	This class handles the dialog and the starter thread.
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Ignition.h"
#include "IgnitionDlg.h"
#include "../Dll/Dll.h"

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

BEGIN_MESSAGE_MAP(CIgnitionDlg, CDialog)
	//{{AFX_MSG_MAP(CIgnitionDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



////////////////////////////////////////////////////////////////////////////////
//
//	The one and only dialog instance and worker thread
//
////////////////////////////////////////////////////////////////////////////////

CIgnitionDlg theDlg;
UINT WorkerThread(LPVOID pParam);


////////////////////////////////////////////////////////////////////////////////
//
//	Create a new dialog instance
//
//	PARAMETERS:
//		CWnd* pParent		Parent window
//
//	RETURNS:
//		-none-
//
////////////////////////////////////////////////////////////////////////////////

CIgnitionDlg::CIgnitionDlg(CWnd* pParent) : CDialog(CIgnitionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIgnitionDlg)
	//}}AFX_DATA_INIT
}



////////////////////////////////////////////////////////////////////////////////
//
//	Initialize dialog
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		BOOL	"TRUE" if successful, "FALSE" if not
//
////////////////////////////////////////////////////////////////////////////////

BOOL CIgnitionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	theDlg.SetStatusLine(IGNITION_VERSION);

	SetTimer(1, 1000, NULL);

	return TRUE;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Timer Callback: Adjust state of application
//
//	PARAMETERS:
//		UINT nIDEvent		Event identification
//
//	RETURNS:
//		-none-
//
////////////////////////////////////////////////////////////////////////////////

void CIgnitionDlg::OnTimer(UINT nIDEvent) 
{
	int state;
	state = theApp.GetState();

	switch(state)
	{
	case IGNITION_STATE_STARTUP:
		AfxBeginThread(WorkerThread, 0);
		theApp.SetState(IGNITION_STATE_RUNNING, 0);
		break;
	case IGNITION_STATE_RUNNING:
		::SetCursor(theApp.LoadStandardCursor(IDC_ARROW));
		break;
	case IGNITION_STATE_SHUTDOWN:
		theApp.PostThreadMessage(WM_QUIT, theApp.GetStateError(), 0);
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}



////////////////////////////////////////////////////////////////////////////////
//
//	Set status line of dialog
//
//	PARAMETERS:
//		char *strFormat, ...	printf() style parameters
//
//	RETURNS:
//		-none-
//
////////////////////////////////////////////////////////////////////////////////

void CIgnitionDlg::SetStatusLine(char *strFormat, ...)
{
	va_list arg;
	va_start(arg, strFormat);

	CString strText;
	strText.FormatV(strFormat, arg);

	GetDlgItem(IDC_STATUS)->SetWindowText(strText);
}



////////////////////////////////////////////////////////////////////////////////
//
//	Update progress bar of dialog
//
//	PARAMETERS:
//		int start		Start value
//		int end			End value
//		int step		Step value
//
//	RETURNS:
//		-none-
//
////////////////////////////////////////////////////////////////////////////////

void CIgnitionDlg::UpdateProgressBar(int start, int end, int step)
{
	CProgressCtrl *pProgressCtrl = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS);

	int current;
	for(current = start; current <= end; current += step)
	{
		pProgressCtrl->SetPos(current);

		SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		Sleep(200);
	}
}



////////////////////////////////////////////////////////////////////////////////
//
//	Workerthread function calling the DLL hooking function
//
//	PARAMETERS:
//		LPVOID pParam		Passed parameters from controlling function
//
//	RETURNS:
//		UINT				Return value of thread
//
////////////////////////////////////////////////////////////////////////////////

UINT WorkerThread(LPVOID pParam)
{
	theDlg.SetStatusLine("Initializing...");
	theDlg.UpdateProgressBar(0, 50, 10);

	theDlg.SetStatusLine("Executing...");

	CString strCommandline = theApp.m_lpCmdLine;

	CString strPatch;
	int err = Startup(CString("Ignition.cfg"), strCommandline, strPatch);
	
	if(err != IGNITION_ERROR_NONE)
	{
		CString strError;
		strError.Format("Patching of target failed: %s.", GetErrorText(err));

		theDlg.SetStatusLine("Patching ... failed");

		AfxMessageBox(strError);
		theApp.SetState(IGNITION_STATE_SHUTDOWN, err);

		return err;
	}

	theDlg.SetStatusLine("Patching (%s)...", (LPCTSTR)strPatch);
	theDlg.UpdateProgressBar(50, 100, 10);

	theDlg.SetStatusLine("Patching (%s)... done.", (LPCTSTR)strPatch);

	Sleep(3000);

	theApp.SetState(IGNITION_STATE_SHUTDOWN, err);

	return err;
}
