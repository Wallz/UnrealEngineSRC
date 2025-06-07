#=============================================================================
# Unreal UTV makefile for non-Windows platforms.
#=============================================================================

# Unreal directory. (Required by makefile-header.)
UNREAL_DIR		= ../..

# Include global definitions.
include $(UNREAL_DIR)/makefile-header

#-----------------------------------------------------------------------------
# Directories.
#-----------------------------------------------------------------------------

SRC_DIRS		= $(UTV_SRC)
INC_DIRS		= $(UTV_INC)
BUILD_DIR		= $(UTV_BUILD)

#-----------------------------------------------------------------------------
# Compiler/linker options.
#-----------------------------------------------------------------------------

CXX			= $(STD_CXX)
CXXFLAGS		= $(STD_CXXFLAGS) $(EXT_CXXFLAGS) -I$(UTV_INC) -I$(CORE_INC) \
	    			-I$(ENGINE_INC) -I$(IPDRV_INC) -DGPackage=GPackageUTV -DThisPackage=UTV
LDFLAGS			= $(STD_LDFLAGS) $(EXT_LDFLAGS)$(UTV_SONAME)
LIBS			= $(STD_LIBS) $(CORE) $(ENGINE) $(IPDRV)
AS			= $(STD_AS)
ASFLAGS			= $(STD_ASFLAGS)

#-----------------------------------------------------------------------------
# Files.
#-----------------------------------------------------------------------------

WINDLLEXP := UTV_API
#PRECOMPILED_HEADER := $(INC_DIRS)/UTV.h
#PRECOMPILE_ON := UTV.cpp

OBJS = \
	BunchDelayer.o \
	ReplicatorEngine.o \
	UtvChannel.o \
	UTVCommandlet.o \
	UTVPackageMap.o \
	UTVRemoteControll.o \
	UTVUplink.o \
	UtvVoiceChannel.o \
	DemoPassthrough.o

OUT = $(UTV)

#-----------------------------------------------------------------------------
# Rules.
#-----------------------------------------------------------------------------

# Use general rules.
include $(UNREAL_DIR)/makefile-general

#-----------------------------------------------------------------------------
# The End.
#-----------------------------------------------------------------------------
