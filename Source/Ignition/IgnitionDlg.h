////////////////////////////////////////////////////////////////////////////////
//
// IgnitionDlg.h
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

#ifndef _IGNITIONDLG_H_
#define _IGNITIONDLG_H_

class CIgnitionDlg : public CDialog
{
public:
	// Create a new dialog instance
	CIgnitionDlg(CWnd* pParent = NULL);

public:
	// Update progress bar of dialog
	void UpdateProgressBar(int start, int end, int step);

	// Set status line of dialog
	void SetStatusLine(char *strFormat, ...);

	//{{AFX_DATA(CIgnitionDlg)
	enum { IDD = IDD_IGNITION_DIALOG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CIgnitionDlg)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIgnitionDlg)
	// Initialize dialog
	virtual BOOL OnInitDialog();

	// Timer Callback: Adjust state of application
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

extern CIgnitionDlg theDlg;

//{{AFX_INSERT_LOCATION}}

#endif
