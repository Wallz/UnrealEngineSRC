#=============================================================================
# Unreal XInterface makefile for non-Windows platforms.
#=============================================================================

# Unreal directory. (Required by makefile-header.)
UNREAL_DIR		= ../..

# Include global definitions.
include $(UNREAL_DIR)/makefile-header

#-----------------------------------------------------------------------------
# Directories.
#-----------------------------------------------------------------------------

SRC_DIRS		= $(XINTERFACE_SRC)
INC_DIRS		= $(XINTERFACE_INC)
BUILD_DIR		= $(XINTERFACE_BUILD)

#-----------------------------------------------------------------------------
# Compiler/linker options.
#-----------------------------------------------------------------------------

CXX			= $(STD_CXX)
CXXFLAGS		= $(STD_CXXFLAGS) $(EXT_CXXFLAGS) -I$(XINTERFACE_INC) \
                    -I$(CORE_INC) -I$(ENGINE_INC) -I$(XGAME_INC) \
                    -I$(IPDRV_INC) \
                    -DGPackage=GPackageXInterface -DThisPackage=XInterface
LDFLAGS			= $(STD_LDFLAGS) $(EXT_LDFLAGS)$(XINTERFACE_SONAME)
LIBS			= $(STD_LIBS) $(CORE) $(IPDRV) $(XGAME)
AS			= $(STD_AS)
ASFLAGS			= $(STD_ASFLAGS)

#-----------------------------------------------------------------------------
# Files.
#-----------------------------------------------------------------------------

OBJS =				\
	XInterface.o    		\
	AHudBase.o  \
	UnGUICore.o  \
    UnGUI.o

OUT = $(XINTERFACE)

#-----------------------------------------------------------------------------
# Rules.
#-----------------------------------------------------------------------------

# Use general rules.
include $(UNREAL_DIR)/makefile-general

#-----------------------------------------------------------------------------
# The End.
#-----------------------------------------------------------------------------
