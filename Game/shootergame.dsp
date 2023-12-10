# Microsoft Developer Studio Project File - Name="shootergame" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=shootergame - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "shootergame.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "shootergame.mak" CFG="shootergame - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "shootergame - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "shootergame - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "shootergame - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /Op /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 Oded.lib zlibstat.lib wsock32.lib dsound.lib dxerr9.lib strmiids.lib dinput8.lib d3d9.lib d3dx9.lib dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "shootergame - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Oded.lib zlibstat.lib wsock32.lib dsound.lib dxerr9.lib strmiids.lib dinput8.lib d3d9.lib d3dx9.lib dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "shootergame - Win32 Release"
# Name "shootergame - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "garryswrappers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\garryswrappers\GarrysGraphicsWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\garryswrappers\GarrysHelperFunctions.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Entitys.cpp
# End Source File
# Begin Source File

SOURCE=.\Landscape.cpp
# End Source File
# Begin Source File

SOURCE=.\Players.cpp
# End Source File
# Begin Source File

SOURCE=.\resources.rc
# End Source File
# Begin Source File

SOURCE=.\Weapons.cpp
# End Source File
# Begin Source File

SOURCE=.\winmain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "garryswrappers.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\garryswrappers\GarrysGraphicsWrapper.h
# End Source File
# Begin Source File

SOURCE=.\garryswrappers\GarrysHelperFunctions.h
# End Source File
# End Group
# Begin Group "directx9"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\activecf.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\amaudio.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\amparse.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\amstream.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\amva.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\Amvideo.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\atsmedia.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\audevcod.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\austream.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\aviriff.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\basetsd.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\bdaiface.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\bdamedia.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\Bdatif.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\bdatypes.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\comlite.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\control.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3d.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3d8.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3d8caps.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3d8types.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3d9.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3d9caps.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3d9types.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dcaps.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3drm.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3drmdef.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3drmobj.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3drmwin.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dtypes.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dvec.inl
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx8.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx8core.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx8effect.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx8math.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx8math.inl
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx8mesh.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx8shape.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx8tex.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx9.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx9anim.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx9core.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx9effect.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx9math.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx9math.inl
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx9mesh.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx9shader.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx9shape.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dx9tex.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dxcore.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dxerr.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dxmath.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dxmath.inl
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dxshapes.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\d3dxsprite.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\ddraw.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\ddstream.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dinput.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dinputd.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dls1.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dls2.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dmdls.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dmerror.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dmksctrl.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dmo.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dmodshow.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dmoimpl.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dmoreg.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dmort.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dmplugin.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dmusbuff.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dmusicc.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dmusicf.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dmusici.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dmusics.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dpaddr.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dplay.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dplay8.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dplobby.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dplobby8.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dpnathlp.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dsconf.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dsetup.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\DShow.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dshowasf.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dsound.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dv.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dvdevcod.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dvdmedia.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dvoice.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dvp.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dx7todx8.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\DxDiag.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dxerr8.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dxerr9.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dxfile.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dxtrans.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\dxva.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\edevctrl.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\edevdefs.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\errors.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\evcode.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\il21dec.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\Iwstdec.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\ks.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\ksguid.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\ksmedia.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\ksproxy.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\ksuuids.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\mediaerr.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\mediaobj.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\medparam.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\mixerocx.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\mmstream.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\mpconfig.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\Mpeg2Bits.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\mpeg2data.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\Mpeg2Error.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\mpegtype.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\Mstvca.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\Mstve.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\Msvidctl.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\multimon.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\playlist.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\qedit.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\qnetwork.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\regbag.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\rmxfguid.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\rmxftmpl.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\sbe.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\Segment.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\strmif.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\tune.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\tuner.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\tvratings.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\uuids.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\vfwmsgs.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\videoacc.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\vmr9.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\vpconfig.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\vpnotify.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\vptype.h
# End Source File
# Begin Source File

SOURCE=..\..\DirectX9SDK\Include\xprtdefs.h
# End Source File
# End Group
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zlib\ioapi.h
# End Source File
# Begin Source File

SOURCE=.\zlib\unzip.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zip.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zlib.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Entitys.h
# End Source File
# Begin Source File

SOURCE=.\Globals.h
# End Source File
# Begin Source File

SOURCE=.\Landscape.h
# End Source File
# Begin Source File

SOURCE=.\Players.h
# End Source File
# Begin Source File

SOURCE=.\Weapons.h
# End Source File
# Begin Source File

SOURCE=.\winmain.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icon.bmp
# End Source File
# Begin Source File

SOURCE=.\icon.ico
# End Source File
# Begin Source File

SOURCE=.\mainico.ico
# End Source File
# Begin Source File

SOURCE=.\SBAudigy.ico
# End Source File
# End Group
# End Target
# End Project
