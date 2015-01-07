; CLW-Datei enthält Informationen für den MFC-Klassen-Assistenten

[General Info]
Version=1
LastClass=CIgnitionDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Ignition.h"

ClassCount=2
Class1=CIgnitionApp
Class2=CIgnitionDlg

ResourceCount=3
Resource2=IDD_IGNITION_DIALOG
Resource1=IDR_MAINFRAME
Resource3=IDD_IGNITION_DIALOG (Englisch (USA))

[CLS:CIgnitionApp]
Type=0
HeaderFile=Ignition.h
ImplementationFile=Ignition.cpp
Filter=N
LastObject=CIgnitionApp

[DLG:IDD_IGNITION_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CIgnitionDlg

[DLG:IDD_IGNITION_DIALOG (Englisch (USA))]
Type=1
Class=CIgnitionDlg
ControlCount=3
Control1=IDC_PROGRESS,msctls_progress32,1350565888
Control2=IDC_STATIC,static,1342179342
Control3=IDC_STATUS,static,1342308353

[CLS:CIgnitionDlg]
Type=0
HeaderFile=IgnitionDlg.h
ImplementationFile=IgnitionDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CIgnitionDlg
VirtualFilter=dWC

