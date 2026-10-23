## Haiku Generic Makefile ##

NAME = DateReplicant
TYPE = APP
APP_MIME_SIG = application/x-vnd.Be-SimpleDate

SRCS = \
	src/main.cpp \
	src/DatePreferences.cpp \
	src/PreferencesTools.cpp \
	src/DateView.cpp

RDEFS = resources/DateReplicant.rdef
RSRCS =

LIBS = be translation
LIBPATHS =
SYSTEM_INCLUDE_PATHS =
LOCAL_INCLUDE_PATHS = src

OPTIMIZE := FULL
LOCALES =
DEFINES =
WARNINGS = ALL
SYMBOLS :=
DEBUGGER :=
COMPILER_FLAGS = -std=c++17 -Woverloaded-virtual -Wwrite-strings
LINKER_FLAGS =
DRIVER_PATH =

DEVEL_DIRECTORY := \
	$(shell findpaths -r "makefile_engine" B_FIND_PATH_DEVELOP_DIRECTORY)
include $(DEVEL_DIRECTORY)/etc/makefile-engine
