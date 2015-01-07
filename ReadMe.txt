//////////////////////////////////////////////////////////////////////

                         -- I G N I T I O N --

                              Version 1.3
                            (30. Jan. 2001)

         Copyright (C) 2000, 2001 Bruno 'Beosil' Heidelberger



///////////////////////////////////////////////// Table of Contents //

   1 License

   2 Introduction
      2.1 What is Ignition?
      2.2 Functioning
      2.3 Current Features
      2.4 Website
      2.5 Author

   3 Package Overview
      3.1 Directory Structure
      3.2 Special/Important Files

   4 Installation
      4.1 Installation
      4.2 Configuration

   5 History/Bugs/Feedback
      5.1 History
      5.2 Current Bugs
      5.3 Feedback



/////////////////////////////////////////////////////// License (1) //

   This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

   You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA



////////////////////////////////////////////////// Introduction (2) //

2.1 What is Ignition?

   Ignition is an educational tool demonstrating how somebody could
potentially manipulate applications in memory at runtime.


2.2 Functioning

   Ignition executes the following steps when started:

     1) Ignition reads the configuration file

     2) The target application is searched

     3) A checksum/length test is done to identify the matching patch

     4) The target application is executed

     5) The patch is applied to the running target application

     6) Modules (plugins) are loaded and initialized


2.3 Current Features

   - runs on all Windows platforms (Win9x/NT/2000).

   - target application can be specified via commandline, path or
     registry-entry.

   - a checksum/length test identifies and chooses the correct patch
     for different versions of a target application.

   - special features can be added with plugins


2.4 Website

   The official website can be found at: http://www.beosil.com


2.5 Author

   Bruno 'Beosil' Heidelberger (beosil@swileys.com)



////////////////////////////////////////////// Package Overview (3) //

3.1 Directory Structure

   - Ignition        : root directory of Ignition
      - Source       : source (VC6 workspace)
         - Ignition  : source-code of the executable
         - Dll       : source-code of the DLL

   Note: The actual content of your package depends on the type of
         release. If you miss the source or any other part mentioned
         in this document, visit the official website at:

         http://www.beosil.com


3.2 Special/Important Files

   - Ignition/Ignition.cfg
     This is the configuration file for Ignition. See
     "4.2 Configuration" for details.

   - Ignition/License.txt
     This file describes the license for Ignition. Read it carefully!



////////////////////////////////////////////////// Installation (4) //

4.1 Installation

   The installation of Ignition is straightforward: Unpack the whole
zip-file to your "Program Files" directory.


4.2 Configuration

   All information about the target application is stored in the
configuration file of Ignition (Ignition.cfg). This file contains two
parts: The first part defines the location of the target application
and a list of plugins to load. The second part is a list of
memory-patches for the target application.

   Lines starting with a # are comment lines.

   A target application location can be defined in three different
ways:

     - !commandline
       The commandline parameter of Ignition is an absolute or
       relative path to the target file.
       Example: !commandline

     - !file <filename>
       <filename> is an absolute or relative path to the target file.
       Example: !file "C:\Foo\app.exe"

     - !registry <key> <value> <add-on>
       The path to the target file is <add-on> appended to the value
       stored in <value> under <key> in the registry.
       Example: !registry "Software\Foo\1.0" "InstallPath" "app.exe"

It is possible to have several target locations specified. The first
match (existing file) will be used in this case.

   Modules (plugins) are special dynamic loaded libraries (.dll) that
are loaded into the address-space of the target application and
initialized there. Such modules are included like this:

     !module <filename>
     Example: !module "foobar.dll"

   For every version of the target application there should be a
memory patch line defined like this:

     <name> <checksum> <length> : <address> = <byte> ... <byte> , ...

       - <name> is the name/description of the specific version.

       - <checksum> is the CRC32 checksum of the target file in hex.

       - <length> is the length of the target file in hex.

       - <address> is the virtual address of a memory patch element.

       - <byte> ... <byte> is a list of bytes in hex that are written
         to the specified <address>.

     Example: "app.exe" a1b2c3d4 1a00 : 401022 = 1f 2e , 4013ae = ff



///////////////////////////////////////// History/Bugs/Feedback (5) //

5.1 History

   Version 1.3 (30. Jan. 2001)

     - Added more detailed error-messages.

   Version 1.2 (22. Dec. 2000)

     - Added plugin functionality.

     - "Smoother" shutdown of target application after a patch error.

     - Fixed a bug that prevented "empty" patch lines.

   Version 1.1 (20. Jun. 2000)

     - Fixed a timing problem with CreateProcess().

     - MFC libraries are now statically linked. This should solve the
       incompatibility issues with older MFC42.dll libraries (aka.
       "missing ordinal" bug).

   Version 1.0 (14. Jun. 2000)

     - Initial release.


5.2 Current Bugs

   No known bugs right now. ;)


5.3 Feedback

   If you find any bugs or have suggestions or questions, just drop a
mail to beosil@swileys.com. Do *NOT* ask me questions about specific
configuration files, I will ignore them.



//////////////////////////////////////////////////////////////////////
