; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=editentity
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MapEditor.h"
LastPage=0

ClassCount=6
Class1=CMapEditorApp
Class2=CMapEditorDoc
Class3=CMapEditorView
Class4=CMainFrame

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource3=IDR_MAINFRAME (English (U.S.))
Resource4=IDD_ABOUTBOX (English (U.S.))
Class6=editentity
Resource5=IDD_EDITENTITY_DIALOG (English (U.S.))

[CLS:CMapEditorApp]
Type=0
HeaderFile=MapEditor.h
ImplementationFile=MapEditor.cpp
Filter=N
LastObject=CMapEditorApp
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CMapEditorDoc]
Type=0
HeaderFile=MapEditorDoc.h
ImplementationFile=MapEditorDoc.cpp
Filter=N
LastObject=CMapEditorDoc
BaseClass=CDocument
VirtualFilter=DC

[CLS:CMapEditorView]
Type=0
HeaderFile=MapEditorView.h
ImplementationFile=MapEditorView.cpp
Filter=C
LastObject=CMapEditorView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_DELETEENT
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=MapEditor.cpp
ImplementationFile=MapEditor.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_APP_ABOUT
CommandCount=13
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
CommandCount=13
Command4=ID_EDIT_UNDO
Command13=ID_PREV_PANE


[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEWDOC
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_TOOLBAREDITENTITY
Command5=ID_ADDENT
Command6=ID_DELETEENT
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEWDOC
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_APP_ABOUT
CommandCount=13

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[DLG:IDD_EDITENTITY_DIALOG (English (U.S.))]
Type=1
Class=editentity
ControlCount=22
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDITX,edit,1350639744
Control4=IDC_EDITY,edit,1350639744
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_TYPECHOOSE,combobox,1344340227
Control8=IDC_STRING1,edit,1350631552
Control9=IDC_STRING2,edit,1350631552
Control10=IDC_EDIT2,edit,1350639744
Control11=IDC_EDIT3,edit,1350639744
Control12=IDC_EDIT4,edit,1350639744
Control13=IDC_EDIT5,edit,1350639744
Control14=IDC_EDIT6,edit,1350639744
Control15=IDC_EDIT7,edit,1350639744
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_EDIT8,edit,1350639744
Control21=IDC_EDIT9,edit,1350639744
Control22=IDC_STATIC,static,1342308352

[CLS:editentity]
Type=0
HeaderFile=editentity.h
ImplementationFile=editentity.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT3
VirtualFilter=dWC

