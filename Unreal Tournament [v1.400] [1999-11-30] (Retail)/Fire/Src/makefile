#=============================================================================
# Unreal Fire makefile for Linux.
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

SRC_DIRS	= $(FIRE_SRC)
BUILD_DIR	= $(FIRE_BUILD)

#-----------------------------------------------------------------------------
# Compiler/linker options.
#-----------------------------------------------------------------------------

CXX		= $(STD_CXX)
CXXFLAGS	= $(STD_CXXFLAGS) -fPIC -I$(FIRE_INC) -I$(ENGINE_INC) \
			-I$(CORE_INC) -DGPackage=GPackageFire
LDFLAGS		= $(STD_LDFLAGS) -shared -Wl,-soname,$(FIRE_SONAME)
LIBS		= $(STD_LIBS) $(CORE) $(ENGINE)

#-----------------------------------------------------------------------------
# Files.
#-----------------------------------------------------------------------------

OBJS =			\
	UnFractal.o

OUT = $(FIRE)

#-----------------------------------------------------------------------------
# Rules.
#-----------------------------------------------------------------------------

# Use general rules.
include $(UNREAL_DIR)/makefile-general

#-----------------------------------------------------------------------------
# The End.
#-----------------------------------------------------------------------------
