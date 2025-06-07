#=============================================================================
# Unreal WinDrv makefile for Linux.
#
# Revision history:
# 	* Created by Brandon Reinhart 
#=============================================================================

# Unreal directory. (Required by makefile-header.)
UNREAL_DIR	= ../..

# Include global definitions.
include $(UNREAL_DIR)/makefile-header

#-----------------------------------------------------------------------------
# Directories.
#-----------------------------------------------------------------------------

SRC_DIRS	= $(WINDRV_SRC)
INC_DIRS    = $(WINDRV_INC)
BUILD_DIR	= $(WINDRV_BUILD)

#-----------------------------------------------------------------------------
# Compiler/linker options.
#-----------------------------------------------------------------------------

CXX		= $(STD_CXX)
CXXFLAGS	= $(STD_CXXFLAGS) $(EXT_CXXFLAGS) -I$(WINDRV_INC) -I$(ENGINE_INC) \
			-I$(CORE_INC) -DGPackage=GPackageWinDrv -I$(WINDOW_INC)
LDFLAGS		= $(STD_LDFLAGS) $(EXT_LDFLAGS)$(WINDRV_SONAME)
LIBS		= $(STD_LIBS) $(CORE) $(ENGINE) $(WINDOW)

# DirectX things...
CXXFLAGS += -I../../$(DIRECTXBASEDIR)/include
LDFLAGS += /LIBPATH:"../../$(DIRECTXBASEDIR)/lib"
LIBS += dinput8.lib DxErr8.lib dxguid.lib
CXXFLAGS += -DDIRECTINPUT_VERSION=0x0800

#-----------------------------------------------------------------------------
# Files.
#-----------------------------------------------------------------------------

WINDLLEXP := WINDRV_API
PRECOMPILED_HEADER := $(INC_DIRS)/WinDrv.h
PRECOMPILE_ON := WinDrv.cpp


OBJS =			\
    WinDrv.o \
    WinDivX.o \
    WinClient.o \
    WinSpeech.o \
    WinViewport.o \
    UnDebuggerCore.o \
    UnDelphiInterface.o

OUT = $(WINDRV)

#-----------------------------------------------------------------------------
# Rules.
#-----------------------------------------------------------------------------

# Use general rules.
include $(UNREAL_DIR)/makefile-general

#-----------------------------------------------------------------------------
# The End.
#-----------------------------------------------------------------------------
