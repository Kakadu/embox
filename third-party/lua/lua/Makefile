
PKG_NAME := lua
PKG_VER  := 5.3.0

PKG_SOURCES := http://www.lua.org/ftp/$(PKG_NAME)-$(PKG_VER).tar.gz
PKG_MD5     := a1b0a7e92d0c85bbff7a8d27bf29f8af

PKG_PATCHES := pkg_patch.txt

include $(EXTBLD_LIB)

define option_get
$(shell echo OPTION_BOOLEAN_third_party__cmd__lua__$(1) | gcc -P -E -include $(SRCGEN_DIR)/include/config/third_party/cmd/lua.h -)
endef

LUA_CPPFLAGS :=
ifneq (0,$(call option_get,luasocket_support))
LUA_CPPFLAGS += -DLUASOCKET_SUPPORT
endif

$(BUILD) :
	cd $(PKG_SOURCE_DIR) && ( \
		make posix MAKEFLAGS='$(EMBOX_IMPORTED_MAKEFLAGS)' \
			CC=$(EMBOX_GCC) \
			CPPFLAGS="-I$(ROOT_DIR)/third-party/lua/lua/include/ \
				$(LUA_CPPFLAGS)"; \
	)
	touch $@

$(INSTALL) :
	cp $(PKG_SOURCE_DIR)/src/lua $(PKG_INSTALL_DIR)/lua.o
	mkdir -p $(PKG_INSTALL_DIR)/include
	cp $(PKG_SOURCE_DIR)/src/lua.h $(PKG_INSTALL_DIR)/include/
	cp $(PKG_SOURCE_DIR)/src/luaconf.h $(PKG_INSTALL_DIR)/include/
	cp $(PKG_SOURCE_DIR)/src/lauxlib.h $(PKG_INSTALL_DIR)/include/
	touch $@
