#=============================================================================
# Unreal dedicated server makefile for Linux.
#
# Revision history:
# 	* Created by Mike Danylchuk
#=============================================================================

# Unreal directory. (Required by makefile-header.)
UNREAL_DIR	= .

# Include global definitions.
include $(UNREAL_DIR)/makefile-header

# No default sub-make arguments.
ARGS =

#-----------------------------------------------------------------------------
# Rules.
#-----------------------------------------------------------------------------

.PHONY : all
all : core engine ipdrv fire render ucc xdrv xlaunch xmesagldrv glidedrv \
	uweb audio nullrender

.PHONY : core
core : 
	@$(MAKE) $(ARGS) --directory=$(CORE_SRC)

.PHONY : engine
engine : core
	@$(MAKE) $(ARGS) --directory=$(ENGINE_SRC)

.PHONY : ipdrv
ipdrv : core engine
	@$(MAKE) $(ARGS) --directory=$(IPDRV_SRC)

.PHONY : fire
fire : core engine
	@$(MAKE) $(ARGS) --directory=$(FIRE_SRC)

.PHONY : editor
editor : core engine
	@$(MAKE) $(ARGS) --directory=$(EDITOR_SRC)

.PHONY : ucc
ucc : core engine
	@$(MAKE) $(ARGS) --directory=$(UCC_SRC)

.PHONY : render
render : core engine
	@$(MAKE) $(ARGS) --directory=$(RENDER_SRC)

.PHONY : xlaunch
xlaunch : core engine
	@$(MAKE) $(ARGS) --directory=$(XLAUNCH_SRC)

.PHONY : xdrv
xdrv : core engine
	@$(MAKE) $(ARGS) --directory=$(XDRV_SRC)

.PHONY : xmesagldrv
xmesagldrv : core engine render
	@$(MAKE) $(ARGS) --directory=$(XMESAGLDRV_SRC)

.PHONY : glidedrv
glidedrv : core engine render
	@$(MAKE) $(ARGS) --directory=$(GLIDEDRV_SRC)

.PHONY : galaxy
galaxy :
	@$(MAKE) $(ARGS) --directory=$(GALAXY_SRC)

.PHONY : audio
audio :
	@$(MAKE) $(ARGS) --directory=$(AUDIO_SRC)

.PHONY : uweb
uweb :
	@$(MAKE) $(ARGS) --directory=$(UWEB_SRC)

.PHONY : nullrender
nullrender :
	@$(MAKE) $(ARGS) --directory=$(NULLRENDER_SRC)

#-----------------------------------------------------------------------------
# Maintenance.
#-----------------------------------------------------------------------------

# Backup source code.
.PHONY : backup
backup :
	@-mkdir -p $(BACKUP_DIR)
	@tar zcvf $(BACKUP_DIR)/ucc-$(VERSION)-`date +%y%m%d`.tar.gz \
		$(ALL_SRC_DIRS) [Mm]akefile*

# Create tags table.
.PHONY : tags
tags : 
	@etags --c++ $(ALL_TAGS)

# Pass custom targets to module makefiles.
.DEFAULT : 
	@$(MAKE) ARGS=$@ --no-print-directory

#-----------------------------------------------------------------------------
# The End.
#-----------------------------------------------------------------------------
