###################################################
# Randy(tm) makefile script
# -------------------------
# CHANGES:
# -mwindows target option changed to -mconsole
# as far as undefined references, ld syntax must be
# [ld ...objfile...liblist], not [ld ...liblist...objfile]
# -mconsole removed
# -g option added
# COPTS added


# this is needed with MS toolchain, cause it's too complicated
!include <Win32.Mak>

# environment
CTC = $(cc)
LTC = $(link)
CTCSW = /Wall /EHsc 

# general
PRJNAME = randy
EXENAME = randy.exe
RUNARGS = >_stdolog.txt

# objects
OBJS = randy.obj randydll.obj randygui.obj

# libs
LIBS = winmm.lib gdi32.lib comctl32.lib

# compiler switches
CDBG = $(cdebug)
COPTS = $(CDBG) $(CTCSW) /c $(cflags)  $(cvarsmt)

# linker switches
LDBG = $(ldebug)
LOPTS = $(LDBG) /OUT:$(EXENAME) /LIBPATH:"$(LIB)"  $(guilflags) $(guilibsmt)
DLOPTS = $(LDBG) /LIBPATH:"$(LIB)" $(dlllflags) $(guilibsmt)

# target switches
TOPTS =
DTOPTS = 



####     ####################     ####
#####   ###### TARGETS #######   #####
###### ######################## ######



# end-user targets

all: randy mcomp mseq synawts austream

randy: $(OBJS) randy.res
	$(LTC) $(TOPTS) $(LOPTS) /PDB:randy.pdb randy.obj randydll.obj randygui.obj randy.res $(LIBS)

mcomp: mcomp.obj mcompdll.obj mcomp.def
	$(LTC) $(DTOPTS) /OUT:mcomp.dll /DEF:mcomp.def $(DLOPTS)  mcomp.obj mcompdll.obj  $(LIBS)

mseq: mseq.obj mseqdll.obj mseq.def
	$(LTC) $(DTOPTS) /OUT:mseq.dll /DEF:mseq.def $(DLOPTS) mseq.obj mseqdll.obj $(LIBS)

synawts: synawts.obj synawtsdll.obj synawts.def
	$(LTC) $(DTOPTS) /OUT:awts.dll /DEF:synawts.def $(DLOPTS) synawts.obj synawtsdll.obj $(LIBS)

austream: austream.obj austreamdll.obj austream.def
	$(LTC) $(DTOPTS) /OUT:maustr.dll /DEF:austream.def $(DLOPTS) austream.obj austreamdll.obj $(LIBS)


# internal targets

randy.res: randy.rc
	$(RC) $(RCVARS) $(RCFLAGS) randy.rc

randy.obj: randy.c randy.h
	$(CTC) $(COPTS) randy.c

randydll.obj: randydll.c
	$(CTC) $(COPTS) randydll.c

randygui.obj: randygui.c resource.h
	$(CTC) $(COPTS) randygui.c	

mcomp.obj: mcomp.c mcomp.h rmidi.h
	$(CTC) $(COPTS) mcomp.c

mcompdll.obj: mcompdll.c
	$(CTC) $(COPTS) mcompdll.c

mseq.obj: mseq.c mseq.h mcomp.h rmidi.h
	$(CTC) $(COPTS) mseq.c

mseqdll.obj: mseqdll.c
	$(CTC) $(COPTS) mseqdll.c

synawts.obj: synawts.c synawts.h
	$(CTC) $(COPTS) synawts.c

synawtsdll.obj: synawtsdll.c
	$(CTC) $(COPTS) synawtsdll.c

austream.obj: austream.c austream.h
	$(CTC) $(COPTS) austream.c

austreamdll.obj: austreamdll.c
	$(CTC) $(COPTS) austreamdll.c


# non-compiled targets

run:
	$(EXENAME) $(RUNARGS)

cleanup:
	del .\*.obj 
	del .\*.exp 
	del .\*.lib 

release: all
	mkdir .\rel
	copy *.dll .\rel
	copy *.exe .\rel

installer: release
	cmd /C arandyiso.bat
	