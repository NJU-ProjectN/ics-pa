.DEFAULT_GOAL = app

include $(AM_HOME)/Makefile.check
$(info Building $(NAME) [$(ARCH)])

APP_DIR ?= $(shell pwd)
INC_DIR += $(APP_DIR)/include/
DST_DIR ?= $(APP_DIR)/build/$(ARCH)/
BINARY ?= $(APP_DIR)/build/$(NAME)-$(ARCH)

INC_DIR += $(addsuffix /include/, $(addprefix $(AM_HOME)/libs/, $(LIBS)))

$(shell mkdir -p $(DST_DIR))

LIBS += klib # link klib by default

include $(AM_HOME)/Makefile.compile

ifeq ($(ARCH), native)
LINKLIBS = $(filter-out klib, $(LIBS))
else
LINKLIBS = $(LIBS)
endif

LINK_FILES += $(AM_HOME)/am/build/am-$(ARCH).a $(OBJS)
LINK_FILES += $(addsuffix -$(ARCH).a, $(join \
  $(addsuffix /build/, $(addprefix $(AM_HOME)/libs/, $(LINKLIBS))), \
  $(LINKLIBS) \
))

.PHONY: app run clean
app: $(OBJS) am $(LIBS)
	@bash $(AM_HOME)/am/arch/$(ARCH)/img/build $(BINARY) $(LINK_FILES)
run: app
	@bash $(AM_HOME)/am/arch/$(ARCH)/img/run $(BINARY)

clean: 
	rm -rf $(APP_DIR)/build/
