bin_PROGRAMS = labwc-prompt

#dist_bin_SCRIPTS = hdmi-audio-select

labwc_prompt_CFLAGS = \
	-I$(top_srcdir) \
	-DPACKAGE_LIB_DIR=\""$(libdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)/labwc-prompt"\" \
	-DPACKAGE_UI_DIR=\""$(datadir)/labwc-prompt/ui"\" \
	-DPACKAGE_BIN_DIR=\""$(bindir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	$(PACKAGE_CFLAGS) \
	$(G_CAST_CHECKS) \
	-Wall

labwc_prompt_SOURCES = labwc-prompt.c

labwc_prompt_includedir = $(includedir)/labwc-prompt

labwc_prompt_include_HEADERS =

labwc_prompt_LDFLAGS = $(DYNAMIC_FLAGS)
labwc_prompt_DEPENDENCIES_EXTRA = $(BUILTIN_PLUGINS)

labwc_prompt_LDADD = \
		$(BUILTIN_PLUGINS) \
		$(PACKAGE_LIBS) \
		$(X11_LIBS) \
		$(INTLLIBS)
		
EXTRA_DIST =
