////////////////////////////////////////////////////////////////////////////////
//
// Patch.h
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

#ifndef _PATCH_H_
#define _PATCH_H_

#define PATCH_LINE_MAX		4096
#define PATCH_ELEMENT_MAX	4096

class CPatch
{
public:
	typedef struct
	{
		unsigned int address;
		unsigned int length;
		unsigned char *pBuffer;
	} TElement;

	typedef CList<TElement *, TElement *> TElementList;

	typedef struct
	{
		char *strFilename;
	} TModule;

	typedef CList<TModule *, TModule *> TModuleList;

protected:
	char *m_strName;
	char *m_strTarget;

	TElementList m_elementList;
	TModuleList m_moduleList;

public:
	// Construct an empty patch instance
	CPatch();

	// Destruct instance
	virtual ~CPatch();

	// Clear all information and free memory allocations
	void Clear();

	// Find target via commandline, file list or registry entry
	bool FindTarget(const char *strConfigFilename, const char *strCommandLine);

	// Get a reference to the patch element list
	TElementList& GetElementList();

	// Get a reference to the patch element list
	TModuleList& GetModuleList();

	// Get name of patch
	char *GetName();

	// Get filename of target
	char *GetTarget();

	// Load modules
	bool LoadModules(const char *strConfigFilename);

	// Load patch information for a given checksum/length pair
	bool LoadPatch(const char *strConfigFilename, unsigned int checksum, unsigned int length);

protected:
	// Create patch information from a configuration line
	bool Create(char *strInit);
};

#endif
