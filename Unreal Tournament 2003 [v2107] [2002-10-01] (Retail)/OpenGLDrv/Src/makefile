#=============================================================================
# Unreal OpenGLDrv makefile for Linux.
#
# Revision history:
# 	* Created by Daniel Vogel (based on XDrv).
# 
#=============================================================================

# Unreal directory. (Required by makefile-header.)
UNREAL_DIR	= ../..

# Include global definitions.
include $(UNREAL_DIR)/makefile-header

#-----------------------------------------------------------------------------
# Directories.
#-----------------------------------------------------------------------------

SRC_DIRS	= $(OPENGLDRV_SRC)
INC_DIRS    = $(OPENGLDRV_INC)
BUILD_DIR	= $(OPENGLDRV_BUILD)

#-----------------------------------------------------------------------------
# Compiler/linker options.
#-----------------------------------------------------------------------------

CXX		= $(STD_CXX)
CXXFLAGS    = $(STD_CXXFLAGS) $(EXT_CXXFLAGS) -I$(OPENGLLNX_INC) \
              -I$(OPENGLDRV_INC) -I$(ENGINE_INC) \
              -I$(CORE_INC) -I$(SDL12_INC) -DGPackage=OpenGLDrv
LDFLAGS     = $(STD_LDFLAGS) $(EXT_LDFLAGS)$(OPENGL_SONAME)
LIBS        = $(STD_LIBS)
AS          = $(STD_AS)
ASFLAGS     = $(STD_ASFLAGS)

#-----------------------------------------------------------------------------
# Files.
#-----------------------------------------------------------------------------

OBJS =			\
	OpenGLDrv.o		\
	OpenGLMaterialState.o		\
	OpenGLRenderDevice.o		\
	OpenGLRenderInterface.o		\
	OpenGLResource.o

OUT = $(OPENGLDRV)

#-----------------------------------------------------------------------------
# Rules.
#-----------------------------------------------------------------------------

# Use general rules.
include $(UNREAL_DIR)/makefile-general

#-----------------------------------------------------------------------------
# The End.
#-----------------------------------------------------------------------------
