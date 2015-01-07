////////////////////////////////////////////////////////////////////////////////
//
// Dll.cpp
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

#include "StdAfx.h"
#include "Dll.h"
#include "Crc.h"
#include "Patch.h"

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

BEGIN_MESSAGE_MAP(CDllApp, CWinApp)
	//{{AFX_MSG_MAP(CDllApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



////////////////////////////////////////////////////////////////////////////////
//
//	The one and only DLL instance
//
////////////////////////////////////////////////////////////////////////////////

CDllApp theApp;



////////////////////////////////////////////////////////////////////////////////
//
//	Shared data declaration
//
////////////////////////////////////////////////////////////////////////////////

#pragma data_seg("Shared")
	HHOOK g_hHook = NULL;
	char g_strConfigFilename[IGNITION_MAX_BUFFERSIZE] = { "" };;
	char g_strCommandLine[IGNITION_MAX_BUFFERSIZE] = { "" };;
	char g_strTarget[IGNITION_MAX_BUFFERSIZE] = { "" };;
	char g_strPatch[IGNITION_MAX_BUFFERSIZE] = { "" };;
	char g_strError[IGNITION_MAX_BUFFERSIZE] = { "" };;
#pragma data_seg()



////////////////////////////////////////////////////////////////////////////////
//
//	Install DLL and patch target
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		int		IGNITION_ERROR_LOADDLL:		Failed to load DLL
//				IGNITION_ERROR_NOTARGET:	Target not found
//				IGNITION_ERROR_NOPATCH:		Invalid patch information
//				IGNITION_ERROR_PROTECT:		Memory protection
//				IGNITION_ERROR_MODULE:		Failed to load module
//				IGNITION_ERROR_NONE:		Success
//
////////////////////////////////////////////////////////////////////////////////

int CDllApp::InstallDll()
{
	if(!LoadLibrary("Ignition.dll"))
	{
		sprintf(g_strError, " (Ignition.dll)");
		return IGNITION_ERROR_LOADDLL;
	}

	unsigned int checksum;
	unsigned int length;

	CCrc crc;
	if(!crc.GetCrc(g_strTarget, &checksum, &length))
	{
		sprintf(g_strError, " (target=%s)", g_strTarget);
		return IGNITION_ERROR_NOTARGET;
	}

	CPatch patch;
	if(!patch.LoadPatch(g_strConfigFilename, checksum, length))
	{
		sprintf(g_strError, " (checksum=%08x, length=%x)", checksum, length);
		return IGNITION_ERROR_NOPATCH;
	}

	CPatch::TElementList& elementList = patch.GetElementList();

	POSITION position = elementList.GetHeadPosition();
	while(position)
	{
		CPatch::TElement *pElement = elementList.GetNext(position);

		DWORD oldProtect;
		if(VirtualProtect((LPVOID)(pElement->address), pElement->length, PAGE_READWRITE, &oldProtect) == 0)
		{
			sprintf(g_strError, " (address=%08x)", pElement->address);
			return IGNITION_ERROR_PROTECT;
		}

		CopyMemory((unsigned char *)pElement->address, pElement->pBuffer, pElement->length);
	}

	strcpy(g_strPatch, patch.GetName());

	if(!patch.LoadModules(g_strConfigFilename)) return IGNITION_ERROR_MODULE_LOAD;

	CPatch::TModuleList& moduleList = patch.GetModuleList();

	position = moduleList.GetHeadPosition();
	while(position)
	{
		CPatch::TModule *pModule = moduleList.GetNext(position);

		HINSTANCE hModule;
		hModule = LoadLibrary(pModule->strFilename);
		if(hModule == NULL)
		{
			sprintf(g_strError, " (module=%s)", pModule->strFilename);
			return IGNITION_ERROR_MODULE_LOAD;
		}

		InstallFunc Install = (InstallFunc)GetProcAddress(hModule, "Install");
		if((Install == NULL) || (Install(checksum, length) != IGNITION_ERROR_NONE))
		{
			sprintf(g_strError, " (module=%s)", pModule->strFilename);
			return IGNITION_ERROR_MODULE_INSTALL;
		}
	}

	return IGNITION_ERROR_NONE;
}



////////////////////////////////////////////////////////////////////////////////
//
// Get error text ( exported function )
//
//	PARAMETERS:
//		int error		Error number
//
//	RETURNS:
//		char *			Error description
//
////////////////////////////////////////////////////////////////////////////////

char *GetErrorText(int error)
{
	static char strError[IGNITION_MAX_BUFFERSIZE];

	switch(error)
	{
	case IGNITION_ERROR_NONE:
		sprintf(strError, "No error%s", g_strError);
		break;
	case IGNITION_ERROR_OVERFLOW:
		sprintf(strError, "Parameters are too long%s", g_strError);
		break;
	case IGNITION_ERROR_NOTARGET:
		sprintf(strError, "Target not found%s", g_strError);
		break;
	case IGNITION_ERROR_CREATEPROCESS:
		sprintf(strError, "Process creation failed%s", g_strError);
		break;
	case IGNITION_ERROR_LOADDLL:
		sprintf(strError, "Failed to load DLL%s", g_strError);
		break;
	case IGNITION_ERROR_NOPATCH:
		sprintf(strError, "Invalid patch information%s", g_strError);
		break;
	case IGNITION_ERROR_PROTECT:
		sprintf(strError, "Memory protection%s", g_strError);
		break;
	case IGNITION_ERROR_MODULE_LOAD:
		sprintf(strError, "Failed to load modules%s", g_strError);
		break;
	case IGNITION_ERROR_MODULE_INSTALL:
		sprintf(strError, "Failed to install module%s", g_strError);
		break;
	default:
		sprintf(strError, "Undefined error%s", g_strError);
		break;
	}

	return strError;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Windows message hook
//
//	PARAMETERS:
//		int nCode		Hook code
//		WPARAM wParam	Removal flag
//		LPARAM lParam	Address of structure with message
//
//	RETURNS:
//		LRESULT			Result of next hook function
//
////////////////////////////////////////////////////////////////////////////////

LRESULT WINAPI GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(((MSG *)lParam)->message == WM_USER)
	{
		int retValue = theApp.InstallDll();

		PostThreadMessage(((MSG *)lParam)->wParam, WM_USER, retValue, 0);

		UnhookWindowsHookEx(g_hHook);
	}

	return(CallNextHookEx(g_hHook, nCode, wParam, lParam));
}



////////////////////////////////////////////////////////////////////////////////
//
//	Start target process and hook it ( exported function )
//
//	PARAMETERS:
//		CString& strConfigFilename		Configuration filename
//		CString& strCommandLine			Commandline of starter application
//		CString& strPatch				Patch name will be returned here
//
//	RETURNS:
//		int		IGNITION_ERROR_NONE:			Success
//		   		IGNITION_ERROR_OVERFLOW:		Parameters are too long
//				IGNITION_ERROR_NOTARGET:		Target not found
//				IGNITION_ERROR_CREATEPROCESS:	Process creation failed
//
////////////////////////////////////////////////////////////////////////////////

int Startup(CString& strConfigFilename, CString& strCommandLine, CString& strPatch)
{
	if((strConfigFilename.GetLength() >= IGNITION_MAX_BUFFERSIZE)
		|| (strCommandLine.GetLength() >= IGNITION_MAX_BUFFERSIZE)
		|| (strPatch.GetLength() >= IGNITION_MAX_BUFFERSIZE)) return IGNITION_ERROR_OVERFLOW;

	strcpy(g_strConfigFilename, strConfigFilename);
	strcpy(g_strCommandLine, strCommandLine);
	strcpy(g_strPatch, "none");

	strPatch.Empty();

	CPatch patch;
	if(!patch.FindTarget(g_strConfigFilename, strCommandLine))
	{
		sprintf(g_strError, " (target=%s)", patch.GetTarget());
		return IGNITION_ERROR_NOTARGET;
	}

	strcpy(g_strTarget, patch.GetTarget());

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	if(!CreateProcess(g_strTarget, g_strTarget, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		sprintf(g_strError, " (target=%s)", g_strTarget);
		return IGNITION_ERROR_CREATEPROCESS;
	}

	WaitForInputIdle(pi.hProcess, INFINITE);

	g_hHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, theApp.m_hInstance, pi.dwThreadId);

	PostThreadMessage(pi.dwThreadId, WM_USER, GetCurrentThreadId(), 0);

	MSG msg;
	GetMessage(&msg, NULL, WM_USER, WM_USER);

	if(msg.wParam != IGNITION_ERROR_NONE) TerminateProcess(pi.hProcess, -1);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	strPatch = g_strPatch;

	return msg.wParam;
}
