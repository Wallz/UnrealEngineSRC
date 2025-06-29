#=============================================================================
# Unreal Core makefile for Linux.
#
# Revision history:
# 	* Created by Mike Danylchuk
#=============================================================================

# Unreal directory. (Required by makefile-header.)
UNREAL_DIR	= ../..

# Include global definitions.
include $(UNREAL_DIR)/makefile-header

#-----------------------------------------------------------------------------
# Directories.
#-----------------------------------------------------------------------------

SRC_DIRS	= $(SDLLAUNCH_SRC)
INC_DIRS    = $(SDLLAUNCH_INC)
BUILD_DIR	= $(SDLLAUNCH_BUILD)

#-----------------------------------------------------------------------------
# Compiler/linker options.
#-----------------------------------------------------------------------------

CXX		= $(STD_CXX)

ifeq ($(LINKSTYLE),shared)
CXXFLAGS	= $(STD_CXXFLAGS) $(SDL_CFLAGS) -I$(CORE_INC) -I$(ENGINE_INC) \
              -I$(SDL12_INC)
LDFLAGS		= $(STD_LDFLAGS) -export-dynamic
LIBS		= $(SDL12_LIBS) $(ENGINE) $(CORE) $(KARMA_LIBS) $(SDL_LIBS) \
              $(ZLIB) $(OGG) $(VORBIS)
endif

ifeq ($(LINKSTYLE),static)
CXXFLAGS	= $(STD_CXXFLAGS) $(SDL_CFLAGS) \
			-I$(SDLLAUNCH_INC) -I$(CORE_INC) \
			-I$(FIRE_INC) -I$(ENGINE_INC) \
			-I$(IPDRV_INC) -I$(UWEB_INC) \
			-I$(SDLDRV_INC) -I$(AUDIO_INC) \
			-I$(RENDER_INC) -I$(GLIDEDRV_INC) \
			$(GLIDEINC) -Wl,E
LIBS		= $(STD_LIBS) \
			../../System/GlideDrv.a \
			../../System/Render.a ../../System/Audio.a \
			../../System/SDLDrv.a ../../System/UWeb.a \
			../../System/IpDrv.a ../../System/Fire.a \
			../../System/Engine.a ../../System/Core.a \
			$(GLIDELIBS) $(SDL_LIBS)
endif

#-----------------------------------------------------------------------------
# Files.
#-----------------------------------------------------------------------------

OBJS =		\
	SDLLaunch.o

OUT = $(SDLLAUNCH)

#-----------------------------------------------------------------------------
# Rules.
#-----------------------------------------------------------------------------

# Use general rules.
EXECUTABLE	= 1
include $(UNREAL_DIR)/makefile-general

#-----------------------------------------------------------------------------
# The End.
#-----------------------------------------------------------------------------
