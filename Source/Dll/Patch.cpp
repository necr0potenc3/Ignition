////////////////////////////////////////////////////////////////////////////////
//
// Patch.cpp
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
// This class handles the whole patching information.
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Patch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



////////////////////////////////////////////////////////////////////////////////
//
//	Construct an empty patch instance
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		-none-
//
////////////////////////////////////////////////////////////////////////////////

CPatch::CPatch()
{
	m_strName = 0;
	m_strTarget = 0;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Destruct instance
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		-none-
//
////////////////////////////////////////////////////////////////////////////////

CPatch::~CPatch()
{
	Clear();
}



////////////////////////////////////////////////////////////////////////////////
//
//	Clear all information and free memory allocations
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		-none-
//
////////////////////////////////////////////////////////////////////////////////

void CPatch::Clear()
{
	delete [] m_strName;
	delete [] m_strTarget;

	while(!m_moduleList.IsEmpty())
	{
		TModule *pModule = m_moduleList.RemoveHead();

		delete [] pModule->strFilename;
		delete pModule;
	}

	while(!m_elementList.IsEmpty())
	{
		TElement *pElement = m_elementList.RemoveHead();

		delete [] pElement->pBuffer;
		delete pElement;
	}
}



////////////////////////////////////////////////////////////////////////////////
//
//	Create patch information from a configuration line
//
//	PARAMETERS:
//		char *strLine		Configuration line
//
//	RETURNS:
//		bool				"true" if successful, "false" if not
//
////////////////////////////////////////////////////////////////////////////////

bool CPatch::Create(char *strLine)
{
	Clear();

	char *pToken = strtok(strLine, ",\n\r");

	while(pToken != 0)
	{
		pToken += strspn(pToken, " \t");
		if(*pToken == 0) return true;

		unsigned int address;
		if(sscanf(pToken, "%x", &address) != 1) return false;

		if((pToken = strchr(pToken, '=')) == 0) continue;
		pToken++;

		pToken += strspn(pToken, " \t");

		unsigned int length = 0;
		unsigned char buffer[PATCH_ELEMENT_MAX];

		while(*pToken != 0)
		{
			unsigned int value;

			if(sscanf(pToken, "%x", &value) != 1) return false;

			if((value > 255) || (length >= PATCH_ELEMENT_MAX)) return false;

			buffer[length++] = (unsigned char)value;

			pToken += strspn(pToken, "01234567890abcdefABCDEF");
			pToken += strspn(pToken, " \t");
		}

		if(length > 0)
		{
			TElement *pElement = new TElement;
			pElement->address = address;
			pElement->length = length;
			pElement->pBuffer = new unsigned char[length];

			memcpy(pElement->pBuffer, buffer, length);

			m_elementList.AddTail(pElement);
		}

		pToken = strtok(0, ",\n\r");
	}

	return true;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Find target via commandline, file list or registry entry
//
//	PARAMETERS:
//		const char *strConfigFilename	Filename of configuration file
//		const char *strCommandLine		Commandline of application
//
//	RETURNS:
//		bool							"true" if successful, "false" if not
//
////////////////////////////////////////////////////////////////////////////////

bool CPatch::FindTarget(const char *strConfigFilename, const char *strCommandLine)
{
	FILE *hFile = fopen(strConfigFilename, "rb");
	if(hFile == 0) return false;

	while(true)
	{
		char buffer[PATCH_LINE_MAX];
		fgets(buffer, PATCH_LINE_MAX, hFile);

		if(feof(hFile)) break;

		char *pToken = &buffer[strspn(buffer, " \t\n\r")];

		if(*pToken++ != '!') continue;

		char strTarget[_MAX_PATH];

		if(strncmp(pToken, "commandline", 11) == 0)
		{
			if(strCommandLine && (strlen(strCommandLine) > 0))
			{
				strcpy(strTarget, (char *)(LPCTSTR)strCommandLine);
			}
			else continue;
		}
		else if(strncmp(pToken, "file", 4) == 0)
		{
			pToken += 4;
			pToken = &pToken[strspn(pToken, " \t\n\r")];

			if(*pToken++ != '\"') continue;

			char *strFilename = pToken;

			if((pToken = strchr(pToken, '\"')) == 0) continue;

			*pToken++ = 0;
			strcpy(strTarget, strFilename);
		}
		else if(strncmp(pToken, "registry", 8) == 0)
		{
			pToken += 8;
			pToken = &pToken[strspn(pToken, " \t\n\r")];

			if(*pToken++ != '\"') continue;

			char *strKey = pToken;

			if((pToken = strchr(pToken, '\"')) == 0) continue;

			*pToken++ = 0;

			HKEY tempKey;
			if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKey, NULL, KEY_READ, &tempKey) != ERROR_SUCCESS) continue;

			pToken = &pToken[strspn(pToken, " \t\n\r")];
			
			if(*pToken++ != '\"') continue;

			char *strValue = pToken;

			if((pToken = strchr(pToken, '\"')) == 0) continue;

			*pToken++ = 0;

			DWORD pathLength = _MAX_PATH;
			if(RegQueryValueEx(tempKey, strValue, NULL, NULL, (LPBYTE)strTarget, &pathLength) != ERROR_SUCCESS) continue;

			RegCloseKey(tempKey);

			pToken = &pToken[strspn(pToken, " \t\n\r")];
			
			if(*pToken++ != '\"') continue;

			char *strAdd = pToken;

			if((pToken = strchr(pToken, '\"')) == 0) continue;

			*pToken++ = 0;

			strcat(strTarget, strAdd);
		}

		FILE *hTargetFile = fopen(strTarget, "rb");
		if(hTargetFile != 0)
		{
			fclose(hTargetFile);

			delete [] m_strTarget;
			m_strTarget = new char[strlen(strTarget) + 1];
			strcpy(m_strTarget, strTarget);

			fclose(hFile);
			return true;
		}
	}

	fclose(hFile);

	return false;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Get a reference to the patch element list
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		CPatch::TElementList&	Reference to list
//
////////////////////////////////////////////////////////////////////////////////

CPatch::TElementList& CPatch::GetElementList()
{
	return m_elementList;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Get a reference to the module list
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		CPatch::TModuleList&	Reference to list
//
////////////////////////////////////////////////////////////////////////////////

CPatch::TModuleList& CPatch::GetModuleList()
{
	return m_moduleList;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Get name of patch
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		char *		Name of patch
//
////////////////////////////////////////////////////////////////////////////////

char *CPatch::GetName()
{
	return m_strName;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Get filename of target
//
//	PARAMETERS:
//		-none-
//
//	RETURNS:
//		char *		Fileame of target
//
////////////////////////////////////////////////////////////////////////////////

char *CPatch::GetTarget()
{
	return m_strTarget;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Load modules
//
//	PARAMETERS:
//		const char *strConfigFilename	Filename of configuration file
//
//	RETURNS:
//		bool							"true" if successful, "false" if not
//
////////////////////////////////////////////////////////////////////////////////

bool CPatch::LoadModules(const char *strConfigFilename)
{
	FILE *hFile;
	if((hFile = fopen(strConfigFilename, "rb")) == 0) return false;

	while(true)
	{
		char buffer[PATCH_LINE_MAX];
		fgets(buffer, PATCH_LINE_MAX, hFile);

		if(feof(hFile)) break;

		char *pToken = &buffer[strspn(buffer, " \t\n\r")];

		if(*pToken++ != '!') continue;

		if(!strncmp(pToken, "module", 6) == 0) continue;

		pToken += 6;
		pToken = &pToken[strspn(pToken, " \t\n\r")];

		if(*pToken++ != '\"') continue;

		char *strFilename = pToken;

		if((pToken = strchr(pToken, '\"')) == 0) continue;

		*pToken++ = 0;

		TModule *pModule = new TModule;
		pModule->strFilename = new char[strlen(strFilename) + 1];

		strcpy(pModule->strFilename, strFilename);

		m_moduleList.AddTail(pModule);
	}

	fclose(hFile);

	return true;
}



////////////////////////////////////////////////////////////////////////////////
//
//	Load patch information for a given checksum/length pair
//
//	PARAMETERS:
//		const char *strConfigFilename	Filename of configuration file
//		unsigned int checksum			Checksum of target
//		unsigned int length				Length of target
//
//	RETURNS:
//		bool							"true" if successful, "false" if not
//
////////////////////////////////////////////////////////////////////////////////

bool CPatch::LoadPatch(const char *strConfigFilename, unsigned int checksum, unsigned int length)
{
	FILE *hFile;
	if((hFile = fopen(strConfigFilename, "rb")) == 0) return false;

	while(true)
	{
		char buffer[PATCH_LINE_MAX];
		fgets(buffer, PATCH_LINE_MAX, hFile);

		if(feof(hFile)) break;

		char *pToken = &buffer[strspn(buffer, " \t\n\r")];

		if(*pToken++ != '\"') continue;

		char *strName = pToken;

		if((pToken = strchr(pToken, '\"')) == 0) continue;

		*pToken++ = 0;

		unsigned int currentChecksum;
		unsigned int currentLength;

		if(sscanf(pToken, "%x %x", &currentChecksum, &currentLength) != 2) continue;

		if((currentChecksum != checksum) || (currentLength != length)) continue;

		if((pToken = strchr(pToken, ':')) == 0) continue;

		if(!Create(pToken + 1)) continue;

		m_strName = new char[strlen(strName) + 1];
		strcpy(m_strName, strName);

		return true;
	}

	fclose(hFile);

	return false;
}
