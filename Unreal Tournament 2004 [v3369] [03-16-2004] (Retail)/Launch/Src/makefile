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

SRC_DIRS	= $(LAUNCH_SRC)
INC_DIRS    = $(LAUNCH_INC)
BUILD_DIR	= $(LAUNCH_BUILD)

#-----------------------------------------------------------------------------
# Compiler/linker options.
#-----------------------------------------------------------------------------

CXX		= $(STD_CXX)

#ifeq ($(LINKSTYLE),shared)
  CXXFLAGS	= $(STD_CXXFLAGS) -I$(CORE_INC) -I$(ENGINE_INC) -I$(WINDOW_INC)
  LDFLAGS = $(STD_LDFLAGS)
  LIBS = $(STD_LIBS) $(ENGINE) $(CORE) $(KARMA_LIBS) $(ZLIB) $(WINDOW)
#endif

ifeq ($(strip $(BUILD)),release)
  LDFLAGS += DbgHelp.lib
endif

ifeq ($(LINKSTYLE),static)
#CXXFLAGS	= $(STD_CXXFLAGS) $(TARGET_INC)
#LIBS		= $(STD_LIBS) $(TARGET_LIB)
#LDFLAGS     = $(STD_LDFLAGS)

CXXFLAGS += -DDIRECTINPUT_VERSION=0x0800
CXXFLAGS += -I../../$(DIRECTXBASEDIR)/include
LDFLAGS += /LIBPATH:"../../$(DIRECTXBASEDIR)/lib"
LDFLAGS += d3d9.lib d3dx9.lib ddraw.lib strmiids.lib
LDFLAGS += dsound.lib dxguid.lib dinput8.lib DxErr8.lib
LIBS += $(IPDRV) $(FIRE) $(UWEB) $(XGAME) $(XINTERFACE) $(WINDRV) $(D3D9DRV) $(OPENGLDRV) $(ALAUDIO) $(GAMERANGER_LIBS) $(OGG) $(VORBIS) $(NEWVEHICLES) $(ONSLAUGHT) $(SPEEX) $(EDITOR)
CXXFLAGS += -I$(ENGINE_SRC) -I$(FIRE_INC) -I$(IPDRV_INC) -I$(XINTERFACE_INC) -I$(WINDRV_INC) -I$(WINDRV_SRC) \
            -I$(XGAME_INC) -I$(UWEB_INC) -I$(SDLDRV_INC) -I$(OPENGLDRV_INC) -I$(D3D9DRV_SRC) \
            -I$(OPENGLLNX_INC) -I$(ALAUDIO_INC) -I$(OPENAL_INC) -I$(SPEEX_INC) \
            -I$(ONSLAUGHT_INC) -I$(EDITOR_INC)

ifeq ($(DOPIXO),true)
#LDFLAGS += -Xlinker --allow-multiple-definition
LIBS += $(PIXODRV) $(PIXOMATIC_LIBS)
CXXFLAGS += -I$(PIXODRV_INC) -I$(PIXOMATIC_INC)
endif

endif

#-----------------------------------------------------------------------------
# Files.
#-----------------------------------------------------------------------------

OBJS =		\
	Launch.o \
	Res/LaunchRes.res

OUT = $(LAUNCH)

#-----------------------------------------------------------------------------
# Rules.
#-----------------------------------------------------------------------------

# Use general rules.
EXECUTABLE	= 1
include $(UNREAL_DIR)/makefile-general

#-----------------------------------------------------------------------------
# The End.
#-----------------------------------------------------------------------------
