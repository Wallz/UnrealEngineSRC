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

SRC_DIRS	= $(UCC_SRC)
INC_DIRS	= $(UCC_INC)
BUILD_DIR	= $(UCC_BUILD)

#-----------------------------------------------------------------------------
# Compiler/linker options.
#-----------------------------------------------------------------------------

CXX		= $(STD_CXX)

ifeq ($(TARGETTYPE),psx2)
TARGET_INC	= -I$(UCC_INC) -I$(CORE_INC) -I$(ENGINE_INC) \
		  -I$(NULLNETDRIVER_INC) -I$(PSX2_INC)
TARGET_LIB	= $(UCC_OUT)/NullNetDriver.a \
		  $(UCC_OUT)/Engine.a $(UCC_OUT)/Core.a \
		  -lpc -lkernl -lm

LD		= $(STD_LD)
LDFLAGS		= $(STD_LDFLAGS)

AS		= $(STD_AS)
ASFLAGS		= $(STD_ASFLAGS)

LCFILE		= $(PSX2_LIB)/app.cmd
endif

ifeq ($(TARGETTYPE),linux)
TARGET_INC	= -I$(UCC_INC) -I$(CORE_INC) -I$(ENGINE_INC) \
		  -I$(FIRE_INC) -I$(IPDRV_INC) -I$(UWEB_INC) \
		  -I$(NULLNETDRIVER_INC)
#TARGET_LIB	= $(UCC_OUT)/UWeb.a $(UCC_OUT)/IpDrv.a $(UCC_OUT)/Fire.a \
#		  $(UCC_OUT)/Engine.a $(UCC_OUT)/Core.a
TARGET_LIB	= $(UCC_OUT)/NullNetDriver.a $(UCC_OUT)/Engine.a \
		  $(UCC_OUT)/Core.a
endif

#ifeq ($(LINKSTYLE),shared)
 CXXFLAGS	= $(STD_CXXFLAGS) -I$(CORE_INC) -I$(ENGINE_INC)
 LDFLAGS = $(STD_LDFLAGS)
 ifeq ($(TARGETTYPE),macosx)
  LDFLAGS += -framework Carbon
 endif
 #ifeq ($(COMPILER),gcc)
 # LDFLAGS += -export-dynamic
 #endif
 ifeq ($(COMPILER),icc)
  LDFLAGS += -Qoption,link,--export-dynamic
 endif

  LIBS		= $(STD_LIBS) $(ENGINE) $(CORE) $(ZLIB) $(KARMA_LIBS)
#endif

ifeq ($(LINKSTYLE),static)
#CXXFLAGS	= $(STD_CXXFLAGS) $(TARGET_INC)
#LIBS		= $(STD_LIBS) $(TARGET_LIB)
#LDFLAGS     = $(STD_LDFLAGS)
LIBS += $(IPDRV) $(FIRE) $(UWEB) $(XGAME) $(XINTERFACE) $(ONSLAUGHT) $(NEWVEHICLES) $(EDITOR)
CXXFLAGS += -I$(ENGINE_SRC) -I$(FIRE_INC) -I$(IPDRV_INC) -I$(XINTERFACE_INC) \
            -I$(XGAME_INC) -I$(UWEB_INC) -I$(ONSLAUGHT_INC) -I$(EDITOR_INC) -I$(EDITOR_SRC)
endif


#-----------------------------------------------------------------------------
# Files.
#-----------------------------------------------------------------------------

OBJS =		\
	UCC.o

OUT = $(UCC)

#-----------------------------------------------------------------------------
# Rules.
#-----------------------------------------------------------------------------

# Use general rules.
EXECUTABLE	= 1
WINSUBSYSTEM := CONSOLE
include $(UNREAL_DIR)/makefile-general

#-----------------------------------------------------------------------------
# The End.
#-----------------------------------------------------------------------------
