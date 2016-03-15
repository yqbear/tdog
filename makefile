#############################################################################
#  ADXM         : Angry Dog Cross-platform Make (5.3)
#  COPYRIGHT    : Andy Thomas (c) 2016
#  WEBSITE      : bigangrydog.com
#  LICENSE      : Apache 2.0
#############################################################################

#############################################################################
# INFORMATION - DO NOT MODIFY THIS FILE
# To configure the project, modify "makefile.conf" and set the option
# variables accordingly. Simply call "make" (or "ming32-make") to build.
# For information, see "makefile.readme".
#############################################################################

#############################################################################
# Copyright 2016 Andy Thomas
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#############################################################################

##################################################
# 1. DEFINE ADXM VARIABLES
##################################################

################################
# 1a. INFORMATION
################################
ADXM_VERSION := 5
ADXM_FULLVERSION := 5.3
ADXM_DESC := ADXM: Angry Dog Cross-Platform Make
ADXM_COPYRIGHT := Copyright (c) Andy Thomas 2016

################################
# 1b. MAKEFILE PATH
################################

# Derive fully qualified path to makefile (not the working directory).
MAKE_FILENAME := $(lastword $(MAKEFILE_LIST))
MAKE_ROOT := $(abspath $(MAKE_FILENAME))__make_uniqueAHUWE
temp := $(notdir $(MAKE_ROOT))
MAKE_ROOT := $(strip $(subst $(temp), , $(MAKE_ROOT)))

################################
# 1c. TARGETS
################################

# Portable ASCII case conversions (these must be on single line)
# NB: Use reference assignment for commands taking $1 variable
ucase_cmd = $(subst a,A,$(subst b,B,$(subst c,C,$(subst d,D,$(subst e,E,$(subst f,F,$(subst g,G,$(subst h,H,$(subst i,I,$(subst j,J,$(subst k,K,$(subst l,L,$(subst m,M,$(subst n,N,$(subst o,O,$(subst p,P,$(subst q,Q,$(subst r,R,$(subst s,S,$(subst t,T,$(subst u,U,$(subst v,V,$(subst w,W,$(subst x,X,$(subst y,Y,$(subst z,Z,$1))))))))))))))))))))))))))

lcase_cmd = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))

# Provide default and case variants
ifdef TARGET
UTARGET := $(call ucase_cmd,$(TARGET))
LTARGET := $(call lcase_cmd,$(TARGET))
else
override TARGET := release
UTARGET := RELEASE
LTARGET := release
endif # TARGET

# Override for TARGET=all
targinit := $(TARGET)
ifeq ($(LTARGET), all)
multitarg0 := debug
override TARGET := release
UTARGET := RELEASE
LTARGET := release
endif # LTARGET

# Short variant
ifeq ($(LTARGET), release)
STARGET := rel
endif
ifeq ($(LTARGET), debug)
STARGET := dbg
endif

# Check option. The error is acted upon later.
ifndef STARGET
conferr := Invalid TARGET value
confinfo := You must specify release, debug or all
endif # !STARGET

################################
# 1d. PLATFORM
################################

# Automatically detect using ComSpec variable
# to distnguish between Windows and LINUX.

ifdef ComSpec
# Windows (mingw)
BUILD_PLATOS := msw
BUILD_PLATOS_VAR := win
BUILD_PLATOS_UVAR := WIN
BUILD_PLATOS_LONG := windows
BUILD_PLATOS_ULONG := WINDOWS
else
# LINUX (g++)
BUILD_PLATOS := nix
BUILD_PLATOS_VAR := linux
BUILD_PLATOS_UVAR := LINUX
BUILD_PLATOS_LONG := linux
BUILD_PLATOS_ULONG := LINUX
endif # !ComSpec

# User platform default and case variants
ifndef PLATOS
override PLATOS := $(BUILD_PLATOS)
endif # PLATOS

UPLATOS := $(call ucase_cmd,$(PLATOS))
LPLATOS := $(call lcase_cmd,$(PLATOS))

################################
# 1e. ARCHITECTURE
################################

# Auto-detect bitness
arch_init := $(ARCH)

ifndef ARCH
arch_autodetect := 1

ifeq ($(BUILD_PLATOS), nix)

temp := $(shell uname -m)
ifeq ($(temp), x86_64)
override ARCH := 64
endif # x86_64
ifeq ($(temp), i386)
override ARCH := 32
endif # i386
ifeq ($(temp), i686)
override ARCH := 32
endif # i686

else

ifdef PROCESSOR_ARCHITEW6432
override ARCH := 64
endif # PROCESSOR_ARCHITEW6432
ifeq ($(PROCESSOR_ARCHITECTURE), AMD64)
override ARCH := 64
endif # AMD64
ifeq ($(PROCESSOR_ARCHITECTURE), X86)
override ARCH := 32
endif # X86

endif # BUILD_PLATOS
endif # !ARCH

# Override for all
ifeq ($(ARCH), all)
multiarch0 := 32
arch_init := 64
override ARCH := 64
endif # ARCH all
ifeq ($(ARCH), ALL)
multiarch0 := 32
arch_init := 64
override ARCH := 64
endif # ARCH ALL

################################
# 1f. OTHERS
################################

# Literal space character. We need
# it for below.
SPACE :=
SPACE +=

################################
# 1g. TOOLS FOR NIX
################################

ifeq ($(BUILD_PLATOS), nix)

# Build tools
maker := make
compiler := g++
linker := g++
archiver := ar
ranlib := ranlib
debugger := gdb
res_compiler :=

# Output extensions & prefix
exe_ext :=
lib_ext := .a
lib_pfx := lib
so_ext  := .so
so_pfx  := lib

# Standard flags
a_flag := -rc
c_flag := -c
o_flag := -o
fpic_flag = -fPIC

# Shell tools
# NB: Use reference assignment for commands taking $1 variable
shcmd_rm = rm -f $1
shcmd_md = mkdir -p $1
shcmd_newline = echo

# Date and time variables
SHELL_DATE = $(shell date "+%D")
SHELL_TIME = $(shell date "+%R")
SHELL_YEAR = $(shell date "+%Y")

# Hostname from shell
SHELL_HOSTNAME = $(HOSTNAME)

# Blank line (or no line). Use echo $(null_out)
null_out :=

# Shell quote to wrap echo output on LINUX.
# Preserves multiples spaces and other characters.
quote := "

endif # nix

################################
# 1h. TOOLS FOR MSW
################################

ifeq ($(BUILD_PLATOS), msw)

# Build tools
maker := mingw32-make.exe
compiler := g++
linker := g++
archiver := ar
ranlib := ranlib
debugger := gdb
res_compiler := windres

# Output extensions & prefixes
exe_ext := .exe
lib_ext := .a
lib_pfx := lib
so_ext  := .dll
so_pfx :=

# Standard flags
a_flag = -rc
c_flag = -c
o_flag = -o
fpic_flag =

# Shell tools
# Explicitly set SHELL, as MSYS seems to override this if it is in the path.
# NB: Use reference assignment for commands taking $1 variable
SHELL = $(ComSpec)
shcmd_rm = del $(subst /,\, $1)
shcmd_md = mkdir $(subst /,\, $1)
shcmd_newline = echo.

# Date and time variables
SHELL_DATE = $(shell date /T)
SHELL_TIME = $(shell time /T)
SHELL_YEAR = $(lastword $(subst /, $(SPACE), $(SHELL_DATE)))

# Hostname from shell
SHELL_HOSTNAME = $(shell hostname)

# Blank line (or no line). Use echo $(null_out)
# NB. We cannot use an empty string as it
# results in "ECHO OFF" message on Windows.
null_out := off

# Shell quote not needed on MSW
quote :=

endif # msw


##################################################
# 2. IMPORT USER CONFIGURATION
##################################################

################################
# 2a. IMPORT CONF FILE
################################

# Default if omitted
ifndef CONF
CONF := makefile.conf
defconf := 1
endif

# Check if it exists in $(MAKE_ROOT), otherwise it
# may be relative to the work directory, or absolute.
conf := $(MAKE_ROOT)$(CONF)
ifeq ($(wildcard $(conf)),)
conf := $(CONF)
endif # $(conf)

# Still not exist - try name variation if default
ifeq ($(wildcard $(conf)),)

ifdef defconf
CONF := makeconf.mak
conf := $(MAKE_ROOT)$(CONF)
ifeq ($(wildcard $(conf)),)
conf := $(CONF)
endif # $(conf)
endif # defconf

endif # $(conf)


# Make real (must exist)
conf := $(realpath $(conf))

# INCLUDE NOW - if exists
ifeq ($(words $(conf)), 1)
include $(conf)
endif # conf

################################
# 2b. IMPORT VERSION FILE
################################

ifdef COMMON_CONF

# Check in $(MAKE_ROOT) first.
version_file := $(MAKE_ROOT)$(COMMON_CONF)
ifeq ($(wildcard $(version_file)),)
version_file := $(COMMON_CONF)
endif

# Include
ifeq ($(words $(version_file)), 1)
include $(version_file)
endif # conf

endif # COMMON_CONF

################################
# 2c. ENSURE DIRECTORIES
################################

# We also need trailing separators on these
# as we use them in building file paths.
ifdef OUT_DIR
OUT_DIR := $(abspath $(OUT_DIR))/
endif
ifdef SRC_ROOT
SRC_ROOT := $(abspath $(SRC_ROOT))/
endif
ifdef OBJ_DIR
OBJ_DIR := $(abspath $(OBJ_DIR))/
endif

# Ensure others are consitent
INC_COM_DIRS := $(abspath $(INC_COM_DIRS))
INC_NIX_DIRS := $(abspath $(INC_NIX_DIRS))
INC_MSW_DIRS := $(abspath $(INC_MSW_DIRS))
EXTLIB_COM_DIRS := $(abspath $(EXTLIB_COM_DIRS))
EXTLIB_NIX_REL_DIRS := $(abspath $(EXTLIB_NIX_REL_DIRS))
EXTLIB_NIX_DBG_DIRS := $(abspath $(EXTLIB_NIX_DBG_DIRS))
EXTLIB_MSW_REL_DIRS := $(abspath $(EXTLIB_MSW_REL_DIRS))
EXTLIB_MSW_DBG_DIRS := $(abspath $(EXTLIB_MSW_DBG_DIRS))


################################
# 2d. PREPARE FOR NIX
################################

ifeq ($(BUILD_PLATOS), nix)

# We ignore ARCH if not explicitly specified
ifdef arch_init
arch_flag := -m$(arch_init)
endif

# Compiler flags
cxx_so_flags :=
cxx_rel_flags := $(CXX_NIX_REL_FLAGS) $(arch_flag) $(INC_NIX_DIRS:%=-I%) $(MACRO_NIX_REL_DEFS:%=-D%)
cxx_dbg_flags := $(CXX_NIX_DBG_FLAGS) $(arch_flag) $(INC_NIX_DIRS:%=-I%) $(MACRO_NIX_DBG_DEFS:%=-D%)
ifdef APP_NAME
cxx_rel_flags += -DAPP_NAME=$(APP_NAME)
cxx_dbg_flags += -DAPP_NAME=$(APP_NAME)
endif
ifdef APP_VERSION
cxx_rel_flags += -DAPP_VERSION=$(APP_VERSION)
cxx_dbg_flags += -DAPP_VERSION=$(APP_VERSION)
endif
ifdef APP_COPYRIGHT
cxx_rel_flags += -DAPP_COPYRIGHT=$(APP_COPYRIGHT)
cxx_dbg_flags += -DAPP_COPYRIGHT=$(APP_COPYRIGHT)
endif

# Linker flags
lnk_so_flags := -shared
lnk_rel_flags := $(LNK_NIX_REL_FLAGS) $(arch_flag) $(EXTLIB_NIX_REL_DIRS:%=-L%) $(EXTLIB_NIX_REL_NAMES:%=-l%)
lnk_dbg_flags := $(LNK_NIX_DBG_FLAGS) $(arch_flag) $(EXTLIB_NIX_DBG_DIRS:%=-L%) $(EXTLIB_NIX_DBG_NAMES:%=-l%)
so_more :=

ifdef NIX_SONAME
so_more := -Wl,-soname,$(NIX_SONAME)
endif

# Resource flags (n/a)
res_rel_flags :=
res_dbg_flags :=

src_files := $(SRC_NIX_FILES)
res_files :=

# Pre and post build steps
prebuild_step := $(PREBUILD_NIX)
postbuild_step := $(POSTBUILD_NIX)
distbuild_step := $(DISTBUILD_NIX)
installbuild_step := $(INSTALLBUILD_NIX)

endif # nix

################################
# 2e. PREPARE FOR MSW
################################

ifeq ($(BUILD_PLATOS), msw)

# We ignore ARCH if not explicitly specified
ifdef arch_init
arch_flag := -m$(arch_init)
endif

# Compiler flags
cxx_so_flags :=
cxx_rel_flags := $(CXX_MSW_REL_FLAGS) $(arch_flag) $(INC_MSW_DIRS:%=-I%) $(MACRO_MSW_REL_DEFS:%=-D%)
cxx_dbg_flags := $(CXX_MSW_DBG_FLAGS) $(arch_flag) $(INC_MSW_DIRS:%=-I%) $(MACRO_MSW_DBG_DEFS:%=-D%)
ifdef APP_NAME
cxx_rel_flags += -DAPP_NAME=$(APP_NAME)
cxx_dbg_flags += -DAPP_NAME=$(APP_NAME)
endif
ifdef APP_VERSION
cxx_rel_flags += -DAPP_VERSION=$(APP_VERSION)
cxx_dbg_flags += -DAPP_VERSION=$(APP_VERSION)
endif
ifdef APP_COPYRIGHT
cxx_rel_flags += -DAPP_COPYRIGHT=$(APP_COPYRIGHT)
cxx_dbg_flags += -DAPP_COPYRIGHT=$(APP_COPYRIGHT)
endif

# Linker flags
lnk_so_flags := -shared
lnk_rel_flags := $(LNK_MSW_REL_FLAGS) $(arch_flag) $(EXTLIB_MSW_REL_DIRS:%=-L%) $(EXTLIB_MSW_REL_NAMES:%=-l%)
lnk_dbg_flags := $(LNK_MSW_DBG_FLAGS) $(arch_flag) $(EXTLIB_MSW_DBG_DIRS:%=-L%) $(EXTLIB_MSW_DBG_NAMES:%=-l%)

# Define placeholder for import library (filled out later)
msw_import_lib := _MSW_IMPORT_LIB_
so_more := -Wl,--out-implib,$(msw_import_lib)

# Resource flags
res_rel_flags := $(RES_MSW_REL_FLAGS)
res_dbg_flags := $(RES_MSW_DBG_FLAGS)

src_files := $(SRC_MSW_FILES)
res_files := $(RC_MSW_FILES)

# Pre and post build steps
prebuild_step := $(PREBUILD_MSW)
postbuild_step := $(POSTBUILD_MSW)
distbuild_step := $(DISTBUILD_MSW)
installbuild_step := $(INSTALLBUILD_MSW)

endif # msw

################################
# 2f. OVERRIDE SOURCE FILES
################################

# Override source files with those
# given over command line
ifdef SRCFILES
src_files := $(SRCFILES)
res_files :=
endif

# Remove full path parts, leaving only
# relative from $(SRC_ROOT). We need this if a wildcard
# was used to define the files, and we need our
# files to be cleanly relative to $(SRC_ROOT).
unique_part = _uNique7634_
temp := $(subst //,/, $(src_files))
temp := $(temp:%=$(unique_part)%)
temp := $(subst $(unique_part)$(SRC_ROOT),, $(temp))
src_files := $(strip $(subst $(unique_part),, $(temp)))

temp := $(subst //,/, $(res_files))
temp := $(temp:%=$(unique_part)%)
temp := $(subst $(unique_part)$(SRC_ROOT),, $(temp))
res_files := $(strip $(subst $(unique_part),, $(temp)))

################################
# 2g. CHECK FOR CONFIG ERRORS
################################

# Check option. The error is acted upon later.
ifneq ($(ARCH), 64)
ifneq ($(ARCH), 32)
ifndef conferr
ifdef arch_autodetect
conferr := Could not auto-detect ARCH
else
conferr := Invalid ARCH value
endif # arch_autodetect
errinfo := You must specify 64, 32 or all
endif # !conferr
endif # 32
endif # 64

# Check CONF exists, $(conf) will be empty if not.
ifneq ($(words $(conf)), 1)
ifndef conferr
conferr := Invalid CONF: $(CONF)
errinfo := File does not exist or contains multiple items
endif # !conferr
endif # conf

# Check versions match
ifneq ($(ADXM_VERSION), $(adxm_confver))
ifndef conferr
conferr := Invalid configuration file format
errinfo := Configuration file format is not version $(ADXM_VERSION)
endif # !conferr
endif # adxm_confver

# Project base
ifneq ($(words $(BUILD_BASENAME)), 1)
ifndef conferr
conferr := Invalid BUILD_BASENAME: $(BUILD_BASENAME)
errinfo := The value is unspecified or contains multiple items
endif # !conferr
endif # adxm_confver

# Project type
ifneq ($(BUILD_TYPE), exe)
ifneq ($(BUILD_TYPE), lib)
ifneq ($(BUILD_TYPE), so)
ifneq ($(BUILD_TYPE), mst)
ifndef conferr
conferr := Invalid BUILD_TYPE: $(BUILD_TYPE)
errinfo := Specify "exe", "lib", "so" or "mst" only
endif # !conferr
endif # mst
endif # so
endif # lib
endif # exe

# Check goals enabled
ifeq ($(LTARGET), release)
ifdef (RELEASE_DISABLED
ifndef conferr
conferr := Release target is not supported with this make
errinfo := This target has been disabled in the configuration file
endif # !conferr
endif # RELEASE_DISABLED
endif # release

ifeq ($(LTARGET), debug)
ifdef DEBUG_DISABLED
ifndef conferr
conferr := Debug target is not supported with this make
errinfo := The target has been disabled in the configuration file
endif # !conferr
endif # DEBUG_DISABLED
endif # debug

# Look for multiple items in SRC_ROOT
ifneq ($(BUILD_TYPE), mst)
ifneq ($(words $(SRC_ROOT)), 1)
ifndef conferr
conferr := Invalid SRC_ROOT directory: $(SRC_ROOT)
errinfo := Either the value is unspecified or contains multiple items
endif # !conferr
endif # SRC_ROOT
endif # mst

# Source exists?
ifneq ($(BUILD_TYPE), mst)
ifeq (($wildcard $(SRC_ROOT)),)
ifndef conferr
conferr := Invalid SRC_ROOT directory: $(SRC_ROOT)
errinfo := The directory does not exist
endif # !conferr
endif # SRC_ROOT
endif # mst

# Look for multiple items in OUT_DIR
ifneq ($(BUILD_TYPE), mst)
ifneq ($(words $(OUT_DIR)), 1)
ifndef conferr
conferr := Invalid OUT_DIR directory: $(OUT_DIR)
errinfo := Either the value is unspecified or contains multiple items
endif # !conferr
endif # OUT_DIR
endif # mst

# Look for multiple items in OBJ_DIR
ifneq ($(BUILD_TYPE), mst)
ifneq ($(words $(OBJ_DIR)), 1)
ifndef conferr
conferr := Invalid OBJ_DIR directory: $(OBJ_DIR)
errinfo := Either the value is unspecified or contains multiple items
endif # !conferr
endif # OBJ_DIR
endif # mst

# Check for non-existing source files.
ifneq ($(BUILD_TYPE), mst)
check_files := $(src_files) $(res_files)
check_files := $(check_files:%=$(SRC_ROOT)%)
temp := $(wildcard $(check_files))
temp := $(filter-out $(temp), $(check_files))
ifneq ($(temp),)
ifndef conferr
conferr := Invalid source filename(s)
errinfo := The following files do not exist: $(temp)
endif # !conferr
endif # temp
endif # mst

# Check we are not trying run a library
ifneq ($(findstring run, $(MAKECMDGOALS)),)
ifneq ($(BUILD_TYPE), bin)
endif # BUILD_TYPE
ifndef conferr
conferr := You cannot run a non-executing binary
errinfo := The project build type is: $(BUILD_TYPE)
endif # !conferr
endif # run

# IMPORTANT: We must clear variables pertaining
# to rules, otherwise we won't see the error
ifdef conferr
src_files :=
res_files :=
MAKECONF_DEPS :=
endif # conferr

################################
# 2h. CONCLUDE PREPARATIONS
################################

ifneq ($(BUILD_TYPE), mst)
# Object files extensions
# i.e. Source "main.cpp" -> "main.cpp.nix32.rel.o" etc
obj_fext := .$(BUILD_PLATOS)$(ARCH).$(STARGET).o
res_fext := .$(BUILD_PLATOS)$(ARCH).$(STARGET).res.o

# Final object files
obj_files := $(src_files:%=$(OBJ_DIR)%$(obj_fext))
obj_files += $(res_files:%=$(OBJ_DIR)%$(res_fext))
endif # mst

# We use a dummy rule to call dependencies
dep_dummy_fext := _mkdep_dummy
dep_makes := $(MAKECONF_DEPS:%=%$(dep_dummy_fext))


##################################################
# 3. ASSEMBLE BUILD COMMANDS
##################################################

################################
# 3a. BINARY (EXE)
################################

ifeq ($(BUILD_TYPE), exe)

# Flags and targets
ifeq ($(LTARGET), release)
targ_out := $(OUT_DIR)$(BUILD_BASENAME)$(exe_ext)
cxx_flags := $(cxx_rel_flags)
res_flags := $(res_rel_flags)
link_cmd := $(linker) $(obj_files) $(lnk_rel_flags) $(o_flag) $(targ_out)
else
targ_out := $(OUT_DIR)$(BUILD_BASENAME)$(DOUT_SFX)$(exe_ext)
cxx_flags := $(cxx_dbg_flags)
res_flags := $(res_dbg_flags)
link_cmd := $(linker) $(obj_files) $(lnk_dbg_flags) $(o_flag) $(targ_out)
endif # release

# Not used (delete)
msw_import_lib :=

endif # exe

################################
# 3b. STATIC LIBRARY
################################

ifeq ($(BUILD_TYPE), lib)

# Flags and targets
ifeq ($(LTARGET), release)
targ_out := $(OUT_DIR)$(lib_pfx)$(BUILD_BASENAME)$(lib_ext)
cxx_flags := $(cxx_rel_flags)
res_flags := $(res_rel_flags)
link_cmd := $(archiver) $(a_flag) $(targ_out) $(obj_files)
else
targ_out := $(OUT_DIR)$(lib_pfx)$(BUILD_BASENAME)$(DOUT_SFX)$(lib_ext)
cxx_flags := $(cxx_dbg_flags)
res_flags := $(res_dbg_flags)
link_cmd := $(archiver) $(a_flag) $(targ_out) $(obj_files)
endif # release

# Create library index if ranlib defined.
ifdef ranlib
randlib_cmd := $(ranlib) $(targ_out)
endif # ranlib

# Not used (delete)
msw_import_lib :=

endif # lib

################################
# 3c. SHARED LIBRARY (DLL)
################################

ifeq ($(BUILD_TYPE), so)

# Flags and targets
ifeq ($(LTARGET), release)
targ_out := $(OUT_DIR)$(so_pfx)$(BUILD_BASENAME)$(so_ext)
cxx_flags := $(cxx_rel_flags) $(fpic_flag) $(cxx_so_flags)
res_flags := $(res_rel_flags)
else
targ_out := $(OUT_DIR)$(so_pfx)$(BUILD_BASENAME)$(DOUT_SFX)$(so_ext)
cxx_flags := $(cxx_dbg_flags) $(fpic_flag) $(cxx_so_flags)
res_flags := $(res_dbg_flags)
endif # release

$(lnk_rel_flags) += $(fpic_flag)

# Define MSW import library
# Substitute placeholder in so_more with import file, and set
# msw_import_lib to its final value.
ifdef msw_import_lib
temp_dir := $(dir $(targ_out))
temp_base := $(notdir $(basename $(targ_out)))
temp_new := $(temp_dir)$(lib_pfx)$(temp_base)$(lib_ext)
so_more := $(subst $(msw_import_lib),$(temp_new),$(so_more))
msw_import_lib := $(temp)
endif # msw_import_lib

# Link command
ifeq ($(LTARGET), release)
link_cmd := $(linker) $(lnk_so_flags) $(lnk_rel_flags) $(obj_files) $(o_flag) $(targ_out) $(so_more)
else
link_cmd := $(linker) $(lnk_so_flags) $(lnk_dbg_flags) $(obj_files) $(o_flag) $(targ_out) $(so_more)
endif # release

endif # so

################################
# 3d. CREATE AND CLEAN PATHS
################################

# Create list of directories to create, including
# output and object tree structure. Below, we sort and remove
# duplicates generated from the source list, then filter to
# leave only NON-EXISTING paths.
temp := $(src_files) $(res_files)
temp := $(temp:%=$(OBJ_DIR)%)
mkdir_objs := $(sort $(dir $(temp)))

# Filter
temp := $(wildcard $(mkdir_objs))
mkdir_objs := $(strip $(filter-out $(temp), $(mkdir_objs)))

# Output to make
temp := $(wildcard $(OUT_DIR))
mkdir_output := $(filter-out $(temp), $(OUT_DIR))

# Set clean object files that exist
clean_objs := $(obj_files)
clean_objs := $(wildcard $(clean_objs))

# Clean existing output files, provided we are not
# using file list supplied over command line
ifndef SRCFILES
clean_output := $(strip $(targ_out) $(msw_import_lib))
clean_output := $(wildcard $(clean_output))
endif

# Custom
temp := $(wildcard $(CUSTOM_DIRS))
mkdir_custom := $(filter-out $(temp), $(CUSTOM_DIRS))


##################################################
# 4. MAIN GOALS
##################################################

################################
# COMPILE AND LINK
################################

# Do everthing, except clean, and perform dist or install steps
.PHONY:all
all: impl_multimake $(dep_makes) impl_header_out check_for_error makedirs prebuild \
impl_premaking_leader $(obj_files) impl_postmaking_msg impl_linker postbuild

# Does not link, make output dir, or perform pre-build or post-build steps
.PHONY:compile
compile: impl_multimake impl_header_out check_for_error impl_mkdir_leader impl_mkdir_objs impl_postmkdir_msg \
impl_premaking_leader $(obj_files) impl_postmaking_msg

# Link only. Does not perform pre-build or post-build steps
.PHONY:link
link: impl_multimake impl_header_out check_for_error impl_mkdir_output impl_linker

################################
# CLEAN
################################

# The $(clean_objs) variable will already
# be filtered for those that exist.
.PHONY:clean
clean: impl_multimake impl_header_out check_for_error impl_clean_leader impl_clean_objs impl_clean_output \
impl_postclean_msg

.PHONY:cleanall
cleanall: impl_multimake $(dep_makes) impl_header_out check_for_error impl_clean_leader impl_clean_objs \
impl_clean_output impl_postclean_msg

################################
# REBUILDS
################################

# Logic for message output where
# nothing is to be done
ifneq ($(findstring prebuild, $(MAKECMDGOALS)),)
prebuild_info = 1
endif
ifdef prebuild_step
prebuild_info = 1
endif

ifneq ($(findstring postbuild, $(MAKECMDGOALS)),)
postbuild_info = 1
endif
ifdef postbuild_step
postbuild_info = 1
endif

ifneq ($(findstring diststep, $(MAKECMDGOALS)),)
distbuild_info = 1
endif
ifdef distbuild_step
distbuild_info = 1
endif

ifneq ($(findstring installstep, $(MAKECMDGOALS)),)
installbuild_info = 1
endif
ifdef installbuild_step
installbuild_info = 1
endif

.PHONY:rebuild
rebuild: clean compile link

.PHONY:rebuildall
rebuildall: cleanall all

.PHONY:install
install: rebuildall installstep

.PHONY:dist
dist: rebuildall diststep

################################
# INDIVIDUAL STEPS
################################

.PHONY:makedirs
makedirs: impl_multimake impl_header_out impl_mkdir_leader impl_mkdir_objs impl_mkdir_output impl_postmkdir_msg

# Pre-build step
.PHONY:prebuild
prebuild: impl_multimake impl_header_out check_for_error

ifdef prebuild_info
	@echo $(quote)PRE-BUILD STEP ...$(quote)
	@$(shcmd_newline)
endif # prebuild_info

ifdef prebuild_step
	$(prebuild_step)
endif # prebuild_step

ifdef prebuild_info
ifndef prebuild_step
	@echo $(quote)No build step specified$(quote)
endif # !prebuild_step

	@$(shcmd_newline)
endif #prebuild_info

# Post build step
.PHONY:postbuild
postbuild: impl_multimake impl_header_out check_for_error
ifdef postbuild_step
	@echo $(quote)POST-BUILD STEP ...$(quote)
	@$(shcmd_newline)

	$(postbuild_step)

	@$(shcmd_newline)
endif # postbuild_step

# Distribution step
.PHONY:diststep
diststep: impl_multimake impl_header_out check_for_error
ifdef distbuild_step
	@echo $(quote)BUILD DISTRIBUTION ...$(quote)
	@$(shcmd_newline)

	$(distbuild_step)

	@$(shcmd_newline)
endif # distbuild_step

# Install step
.PHONY:installstep
installstep: impl_multimake impl_header_out check_for_error
ifdef installbuild_step
	@echo $(quote)PERFORM INSTALL ...$(quote)
	@$(shcmd_newline)

	$(installbuild_step)

	@$(shcmd_newline)
endif # installbuild_step

################################
# RUN
################################

.PHONY:run
run: impl_header_out check_for_error
	@$(shcmd_newline)
	@echo $(quote)RUNNING BINARY ...$(quote)
	@echo $(quote)$(targ_out) $(RUNOPTS)$(quote)
ifneq ($(wildcard $(targ_out)),)
	-@$(targ_out) $(RUNOPTS)
else
	@echo $(quote)Error. The target file does not exist!
endif # targ_out
	@$(shcmd_newline)

.PHONY:dbg
dbg: impl_header_out check_for_error
	@$(shcmd_newline)
	@echo $(quote)DEBUGGING BINARY ...$(quote)
	@echo $(quote)$(debugger) $(targ_out) $(RUNOPTS)$(quote)
ifneq ($(wildcard $(targ_out)),)
	-@$(debugger) $(targ_out) $(RUNOPTS)
else
	@echo $(quote)Error. The target file does not exist!
endif # targ_out
	@$(shcmd_newline)

################################
# CHECK CONFIG & LIST VARS
################################

# Determine vars to show
ifeq ($(BUILD_PLATOS), nix)
list_nixvars := 1
else
list_mswvars := 1
endif

ifneq ($(findstring listvarsall, $(MAKECMDGOALS)),)
list_nixvars := 1
list_mswvars := 1
endif

# Check configuration
.PHONY: checkconf
checkconf: impl_multimake impl_header_out check_for_error
ifndef errinfo
	@echo $(quote)Configuration OK$(quote)
else
	@$(shcmd_newline)
	@echo $(quote)Hint: Use "listvars" to help find the problem$(quote)
endif # errinfo

.PHONY: listvars check_for_error
listvars: impl_multimake impl_header_out
	@echo $(quote)PLATFORM VARIABLES:$(quote)
	@$(shcmd_newline)
	@echo $(quote)  ADXM_VERSION          : $(ADXM_VERSION)$(quote)
	@echo $(quote)  MAKE_ROOT             : $(MAKE_ROOT)$(quote)
	@echo $(quote)  TARGET                : $(TARGET)$(quote)
	@echo $(quote)  STARGET               : $(STARGET)$(quote)
	@echo $(quote)  UTARGET               : $(UTARGET)$(quote)
	@echo $(quote)  LTARGET               : $(LTARGET)$(quote)
	@echo $(quote)  BUILD_PLATOS          : $(BUILD_PLATOS)$(quote)
	@echo $(quote)  BUILD_PLATOS_VAR      : $(BUILD_PLATOS_VAR)$(quote)
	@echo $(quote)  BUILD_PLATOS_UVAR     : $(BUILD_PLATOS_UVAR)$(quote)
	@echo $(quote)  BUILD_PLATOS_LONG     : $(BUILD_PLATOS_LONG)$(quote)
	@echo $(quote)  BUILD_PLATOS_ULONG    : $(BUILD_PLATOS_ULONG)$(quote)
	@echo $(quote)  ARCH                  : $(ARCH)$(quote)
	@echo $(quote)  PLATOS                : $(PLATOS)$(quote)
	@echo $(quote)  UPLATOS               : $(UPLATOS)$(quote)
	@echo $(quote)  LPLATOS               : $(LPLATOS)$(quote)
	@$(shcmd_newline)
	@echo $(quote)CONFIGURATION:$(quote)
	@$(shcmd_newline)
	@echo $(quote)  CONF                  : $(conf)$(quote)
	@echo $(quote)  BUILD_BASENAME        : $(BUILD_BASENAME)$(quote)
	@echo $(quote)  BUILD_TYPE            : $(BUILD_TYPE)$(quote)
	@echo $(quote)  COMMON_CONF           : $(version_file)$(quote)
	@echo $(quote)  APP_NAME              : $(APP_NAME)$(quote)
	@echo $(quote)  APP_VERSION           : $(APP_VERSION)$(quote)
ifdef list_nixvars
	@echo $(quote)  NIX_SONAME            : $(NIX_SONAME)$(quote)
endif
	@$(shcmd_newline)
	@echo $(quote)DIRECTORIES:$(quote)
	@$(shcmd_newline)
	@echo $(quote)  SRC_ROOT              : $(SRC_ROOT)$(quote)
	@echo $(quote)  OUT_DIR               : $(OUT_DIR)$(quote)
	@echo $(quote)  OBJ_DIR               : $(OBJ_DIR)$(quote)
	@echo $(quote)  INC_COM_DIRS          : $(INC_COM_DIRS)$(quote)
ifdef list_nixvars
	@echo $(quote)  INC_NIX_DIRS          : $(INC_NIX_DIRS)$(quote)
endif
ifdef list_mswvars
	@echo $(quote)  INC_MSW_DIRS          : $(INC_MSW_DIRS)$(quote)
endif
	@$(shcmd_newline)
	@echo $(quote)SOURCE FILES:$(quote)
	@$(shcmd_newline)
	@echo $(quote)  SRC_COM_FILES         : $(SRC_COM_FILES)$(quote)
ifdef list_nixvars
	@echo $(quote)  SRC_NIX_FILES         : $(SRC_NIX_FILES)$(quote)
endif
ifdef list_mswvars
	@echo $(quote)  SRC_MSW_FILES         : $(SRC_MSW_FILES)$(quote)
	@echo $(quote)  RC_MSW_FILES          : $(RC_MSW_FILES)$(quote)
endif
	@$(shcmd_newline)
	@echo $(quote)COMPILER OPTIONS:$(quote)
	@$(shcmd_newline)
	@echo $(quote)  MACRO_COM_DEFS        : $(MACRO_COM_DEFS)$(quote)
ifdef list_nixvars
	@echo $(quote)  MACRO_NIX_REL_DEFS    : $(MACRO_NIX_REL_DEFS)$(quote)
	@echo $(quote)  MACRO_NIX_DBG_DEFS    : $(MACRO_NIX_DBG_DEFS)$(quote)
endif
ifdef list_mswvars
	@echo $(quote)  MACRO_MSW_REL_DEFS    : $(MACRO_NIX_MSW_DEFS)$(quote)
	@echo $(quote)  MACRO_MSW_DBG_DEFS    : $(MACRO_NIX_MSW_DEFS)$(quote)
endif
	@echo $(quote)  EXTLIB_COM_NAME       : $(EXTLIB_COM_NAME)$(quote)
ifdef list_nixvars
	@echo $(quote)  EXTLIB_NIX_REL_NAMES  : $(EXTLIB_NIX_REL_NAME)$(quote)
	@echo $(quote)  EXTLIB_NIX_DBG_NAMES  : $(EXTLIB_NIX_DBG_NAME)$(quote)
endif
ifdef list_mswvars
	@echo $(quote)  EXTLIB_MSW_REL_NAMES  : $(EXTLIB_MSW_REL_NAME)$(quote)
	@echo $(quote)  EXTLIB_MSW_DBG_NAMES  : $(EXTLIB_MSW_DBG_NAME)$(quote)
endif
	@echo $(quote)  EXTLIB_COM_DIRS       : $(EXTLIB_COM_DIRS)$(quote)
ifdef list_nixvars
	@echo $(quote)  EXTLIB_NIX_REL_DIRS   : $(EXTLIB_NIX_REL_DIRS)$(quote)
	@echo $(quote)  EXTLIB_NIX_DBG_DIRS   : $(EXTLIB_NIX_DBG_DIRS)$(quote)
endif
ifdef list_mswvars
	@echo $(quote)  EXTLIB_MSW_REL_DIRS   : $(EXTLIB_MSW_REL_DIRS)$(quote)
	@echo $(quote)  EXTLIB_MSW_DBG_DIRS   : $(EXTLIB_MSW_DBG_DIRS)$(quote)
endif
	@echo $(quote)  CXX_COM_FLAGS         : $(CXX_COM_FLAGS)$(quote)
ifdef list_nixvars
	@echo $(quote)  CXX_NIX_REL_FLAGS     : $(CXX_NIX_REL_FLAGS)$(quote)
	@echo $(quote)  CXX_NIX_DBG_FLAGS     : $(CXX_NIX_DBG_FLAGS)$(quote)
endif
ifdef list_mswvars
	@echo $(quote)  CXX_MSW_REL_FLAGS     : $(CXX_MSW_REL_FLAGS)$(quote)
	@echo $(quote)  CXX_MSW_DBG_FLAGS     : $(CXX_MSW_DBG_FLAGS)$(quote)
endif
	@echo $(quote)  LNK_COM_FLAGS         : $(LNK_COM_FLAGS)$(quote)
ifdef list_nixvars
	@echo $(quote)  LNK_NIX_REL_FLAGS     : $(LNK_NIX_REL_FLAGS)$(quote)
	@echo $(quote)  LNK_NIX_DBG_FLAGS     : $(LNK_NIX_DBG_FLAGS)$(quote)
endif
ifdef list_mswvars
	@echo $(quote)  LNK_MSW_REL_FLAGS     : $(LNK_MSW_REL_FLAGS)$(quote)
	@echo $(quote)  LNK_MSW_DBG_FLAGS     : $(LNK_MSW_DBG_FLAGS)$(quote)
	@echo $(quote)  RES_MSW_REL_FLAGS     : $(RES_MSW_REL_FLAGS)$(quote)
	@echo $(quote)  RES_MSW_DBG_FLAGS     : $(RES_MSW_DBG_FLAGS)$(quote)
endif
	@$(shcmd_newline)
	@echo $(quote)BUILD STEPS:$(quote)
	@$(shcmd_newline)
	@echo $(quote)  MAKECONF_DEPS         : $(MAKECONF_DEPS)$(quote)
ifdef list_nixvars
	@echo $(quote)  PREBUILD_NIX          : $(PREBUILD_NIX)$(quote)
	@echo $(quote)  POSTBUILD_NIX         : $(POSTBUILD_NIX)$(quote)
	@echo $(quote)  DISTBUILD_NIX         : $(DISTBUILD_NIX)$(quote)
	@echo $(quote)  INSTALLBUILD_NIX      : $(INSTALLBUILD_NIX)$(quote)
endif
ifdef list_mswvars
	@echo $(quote)  PREBUILD_MSW          : $(PREBUILD_MSW)$(quote)
	@echo $(quote)  POSTBUILD_MSW         : $(POSTBUILD_MSW)$(quote)
	@echo $(quote)  DISTBUILD_MSW         : $(DISTBUILD_MSW)$(quote)
	@echo $(quote)  INSTALLBUILD_MSW      : $(INSTALLBUILD_MSW)$(quote)
endif
	@$(shcmd_newline)
	@echo $(quote)  OUTPUT:               : $(targ_out)$(quote)
	@$(shcmd_newline)

# LIST ALL VARIABLES
.PHONY: listvarsall
listvarsall: listvars
	@echo $(quote)INTERNAL VARIABLES$(quote)
	@$(shcmd_newline)
	@echo $(quote)  MAKECMDGOALS          : $(MAKECMDGOALS)$(quote)
	@echo $(quote)  MAKEFILE_LIST         : $(MAKEFILE_LIST)$(quote)
	@echo $(quote)  SHELL_HOSTNAME        : $(SHELL_HOSTNAME)$(quote)
	@echo $(quote)  SHELL_DATE            : $(SHELL_DATE)$(quote)
	@echo $(quote)  SHELL_TIME            : $(SHELL_TIME)$(quote)
	@echo $(quote)  SHELL_YEAR            : $(SHELL_YEAR)$(quote)
	@echo $(quote)  conf                  : $(conf)$(quote)
	@echo $(quote)  mkdir_objs            : $(mkdir_objs)$(quote)
	@echo $(quote)  mkdir_output          : $(mkdir_output)$(quote)
	@echo $(quote)  src_files             : $(src_files)$(quote)
	@echo $(quote)  res_files             : $(res_files)$(quote)
	@echo $(quote)  obj_files             : $(obj_files)$(quote)
	@echo $(quote)  clean_objs            : $(clean_objs)$(quote)
	@echo $(quote)  clean_output          : $(clean_output)$(quote)
	@echo $(quote)  cxx_rel_flags         : $(cxx_rel_flags)$(quote)
	@echo $(quote)  cxx_dbg_flags         : $(cxx_dbg_flags)$(quote)
	@echo $(quote)  cxx_flags             : $(cxx_flags)$(quote)
	@echo $(quote)  res_rel_flags         : $(res_rel_flags)$(quote)
	@echo $(quote)  res_dbg_flags         : $(res_dbg_flags)$(quote)
	@echo $(quote)  res_flags             : $(res_flags)$(quote)
	@echo $(quote)  lnk_so_flags          : $(lnk_so_flags)$(quote)
	@echo $(quote)  lnk_rel_flags         : $(lnk_rel_flags)$(quote)
	@echo $(quote)  lnk_dbg_flags         : $(lnk_dbg_flags)$(quote)
	@echo $(quote)  so_more               : $(so_more)$(quote)
	@echo $(quote)  msw_import_lib        : $(msw_import_lib)$(quote)
	@echo $(quote)  dep_makes             : $(dep_makes)$(quote)
	@echo $(quote)  link_cmd              : $(link_cmd)$(quote)
	@echo $(quote)  randlib_cmd           : $(randlib_cmd)$(quote)
	@$(shcmd_newline)

################################
# HELP AND VERSION
################################

.PHONY:help
help:
	@$(shcmd_newline)
	@echo $(quote)Usage: $(maker) -f makefile [GOALS] CONF=[makeconf] TARGET=[release/debug/all]$(quote)
	@echo $(quote)       ARCH=[64/32/all] PLATOS=[custom] SRCFILES=[files] RUNOPTS=[options]$(quote)
	@$(shcmd_newline)
	@echo $(quote)GOALS :$(quote)
	@echo $(quote)  all           - Make and link the output, default. Calls pre and post-build$(quote)
	@echo $(quote)                  steps, and make dependencies.$(quote)
	@echo $(quote)  compile       - Make sources but does not link or call pre or post-build$(quote)
	@echo $(quote)                  steps, or make dependencies.$(quote)
	@echo $(quote)  link          - Link (or archive) object files, but does not call pre or$(quote)
	@echo $(quote)                  post-build steps, or make dependencies.$(quote)
	@echo $(quote)  clean         - Clean intermediary object files and output files, but not those of$(quote)
	@echo $(quote)                  make dependencies.$(quote)
	@echo $(quote)  cleanall      - Clean intermediary object and output files, and those of$(quote)
	@echo $(quote)                  make dependencies.$(quote)
	@echo $(quote)  rebuild       - Rebuilds current project. Equivalent to: clean compile link.$(quote)
	@echo $(quote)                  Calls pre and post-build steps, but NOT make dependencies.$(quote)
	@echo $(quote)  rebuildall    - Rebuilds all. Equivalent to: cleanall all. Calls pre and$(quote)
	@echo $(quote)                  post-build steps, and make dependencies.$(quote)
	@echo $(quote)  install       - Rebuilds all followed by the install command.$(quote)
	@echo $(quote)  dist          - Rebuilds all followed by the distribution command.$(quote)
	@echo $(quote)  makedirs      - Create object and output directories required by the build.$(quote)
	@echo $(quote)                  This is normally called automatically by compile commands.$(quote)
	@echo $(quote)  prebuild      - Perform the pre-build step only.$(quote)
	@echo $(quote)  postbuild     - Perform the post-build step only.$(quote)
	@echo $(quote)  diststep      - Perform the create distribution step only.$(quote)
	@echo $(quote)  installstep   - Perform the install step only.$(quote)
	@echo $(quote)  run           - Execute the binary output with the RUNOPTS command options.$(quote)
	@echo $(quote)  dbg           - Execute the binary under the debugger with the RUNOPTS options.$(quote)
	@echo $(quote)  checkconf     - Check input and config file for errors, otherwise does nothing.$(quote)
	@echo $(quote)  listvars      - Output important configuration variables.$(quote)
	@echo $(quote)  listvarsall   - Output all configuration variables, plus internal variables.$(quote)
	@echo $(quote)  help          - Display key usage options.$(quote)
	@echo $(quote)  version       - Display ADXM version information.$(quote)
	@$(shcmd_newline)
	@echo $(quote)CONF :$(quote)
	@echo $(quote)  The make configuration filename. If omitted, defaults to: "makefile.conf"$(quote)
	@echo $(quote)  (then "makeconf.mak"). Only, a single value can be supplied, but multiple$(quote)
	@echo $(quote)  configurations can be built by creating a "master" project with multiple$(quote)
	@echo $(quote)  dependencies.$(quote)
	@$(shcmd_newline)
	@echo $(quote)TARGET :$(quote)
	@echo $(quote)  Build target as either "release" or "debug", or a case variation of.$(quote)
	@echo $(quote)  Use "all" to build both. The default value is "release".$(quote)
	@$(shcmd_newline)
	@echo $(quote)ARCH :$(quote)
	@echo $(quote)  Target architecture, either "32" or "64". Use "all" to build both. The default is$(quote)
	@echo $(quote)  that of the build machine. Although the bit architecture will usually be detected$(quote)
	@echo $(quote)  automatically from the build machine, it may be useful to specify it explicitly for$(quote)
	@echo $(quote)  projects targeting multiple architectures. The value will automatically be supplied$(quote)
	@echo $(quote)  to the compiler using the -m64 and -m32 compiler options.$(quote)
	@$(shcmd_newline)
	@echo $(quote)PLATOS :$(quote)
	@echo $(quote)  Target platform OS. This will be provided for use in configuration files$(quote)
	@echo $(quote)  as a variable that will, if omitted, default to either "msw" or "nix",$(quote)
	@echo $(quote)  according to the build machine.$(quote)
	@$(shcmd_newline)
	@echo $(quote)SRCFILES :$(quote)
	@echo $(quote)  Source filename(s) to make for use with "compile" command. Filenames must be$(quote)
	@echo $(quote)  either full paths within the root source directory, or relative to it. Wrap multile$(quote)
	@echo $(quote)  items in quotes. If omitted, all source files will be re-made.$(quote)
	@$(shcmd_newline)
	@echo $(quote)RUNOPTS :$(quote)
	@echo $(quote)  Command line options to pass to the binary on execution via the "run" and "dbg" goals.$(quote)
	@$(shcmd_newline)
	@echo $(quote)EXAMPLES :$(quote)
	@echo $(quote)  make all$(quote)
	@echo $(quote)  make all CONF=makelib.conf TARGET=debug$(quote)
	@echo $(quote)  make rebuild CONF=makeapp.conf ARCH=64 PLATOS=fedora$(quote)
	@$(shcmd_newline)
	@echo $(quote)See "makefile.readme" for further information.$(quote)
	@$(shcmd_newline)

.PHONY:version
version:
	@$(shcmd_newline)
	@echo $(quote)$(ADXM_DESC)$(quote)
	@echo $(quote)VERSION: $(ADXM_FULLVERSION)$(quote)
	@echo $(quote)COPYRIGHT: $(ADXM_COPYRIGHT)$(quote)
	@$(shcmd_newline)


##################################################
# 5. INTERNAL IMPLEMENTATION RULES
##################################################

# Nothing done messages we output when nothing to do.
# We clear these, using a hack, in the implicit build rules.
# This was found to be the only way of getting the feature to
# work as expected, as it is difficult to share data out
# from a rule. For some reason checking the variable (using ifeq)
# does not provide the same result as when we echo to screen.
# Not sure why this is so. This is a "nice to have" feature,
# but could be removed if it fails to work on all platforms.
MKDIRS_NOTHING_MSG := Already exist
CLEAN_NOTHING_MSG := Nothing to clean
MAKEFILES_NOTHING_MSG := Nothing to be done

# Display make directory info?
ifneq ($(BUILD_TYPE), mst)
mkdir_info = 1
endif
ifdef mkdir_objs
mkdir_info = 1
endif
ifdef mkdir_outputs
mkdir_info = 1
endif

################################
# HEADER AND ERROR CHECK
################################

# Write header and quit if configuration error was detected
.PHONY:impl_header_out
impl_header_out:
	@$(shcmd_newline)
	@echo $(quote)------------------------------------------------------$(quote)
	@echo $(quote)PROJECT      : $(BUILD_BASENAME)$(quote)
ifdef APP_VERSION
	@echo $(quote)VERSION      : $(APP_VERSION)$(quote)
endif
	@echo $(quote)CONFIG       : $(CONF)$(quote)
	@echo $(quote)BUILD TYPE   : $(BUILD_TYPE)$(quote)
	@echo $(quote)TARGET       : $(UTARGET)$(quote)
	@echo $(quote)PLATFORM     : $(BUILD_PLATOS_ULONG) $(ARCH) bit$(quote)
	@echo $(quote)------------------------------------------------------$(quote)
	@$(shcmd_newline)


# Output error and quit
.PHONY:check_for_error
check_for_error:

ifdef conferr

	@echo $(quote)CONFIGURATION ERROR!$(quote)
	@echo $(quote)$(conferr)$(quote)

ifdef errinfo
	@echo $(quote)$(errinfo)$(quote)
endif # errinfo

	@$(shcmd_newline)

# Exit unless info check-only goal
ifneq ($(MAKECMDGOALS), listvars)
ifneq ($(MAKECMDGOALS), listvarsall)
ifneq ($(MAKECMDGOALS), checkconfig)
	@exit 1
endif # checkconfig
endif # listvarsall
endif # listvars

endif # conferr

################################
# DIRECTORY IMPLEMENTATION
################################

# Write make directory leader
.PHONY:impl_mkdir_leader
impl_mkdir_leader:
ifdef mkdir_info
	@echo $(quote)CREATING DIRS ...$(quote)
endif

# Make object directories
# The directory variable will already be
# filtered for those that do not exist.
# NB. Clear MKDIRS_NOTHING_MSG on invocation.
.PHONY:impl_mkdir_objs
impl_mkdir_objs:
ifdef mkdir_objs
	-$(call shcmd_md, $(mkdir_objs))
	$(eval MKDIRS_NOTHING_MSG := $(null_out))
endif # mkdir_objs

# Make object directories
# The directory variable will already be
# filtered for those that do not exist.
# NB. Clear MKDIRS_NOTHING_MSG on invocation
.PHONY:impl_mkdir_output
impl_mkdir_output:
ifdef mkdir_output
	-$(call shcmd_md, $(mkdir_output))
	$(eval MKDIRS_NOTHING_MSG := $(null_out))
endif # mkdir_output
ifdef mkdir_custom
	-$(call shcmd_md, $(mkdir_custom))
	$(eval MKDIRS_NOTHING_MSG := $(null_out))
endif # mkdir_custom

# Write result of mkdirs
.PHONY:impl_postmkdir_msg
impl_postmkdir_msg:
ifdef mkdir_info
	@echo $(quote)$(MKDIRS_NOTHING_MSG)$(quote)
	@$(shcmd_newline)
endif

################################
# CLEAN IMPLEMENTAION
################################

# Write make directory leader
.PHONY:impl_clean_leader
impl_clean_leader:
ifneq ($(BUILD_TYPE), mst)
	@echo $(quote)CLEANING ...$(quote)
endif # mst

# Clean object directories
# The clean files variable will already be
# filtered for those that exist.
# NB. Clear CLEAN_NOTHING_MSG on invocation.
.PHONY:impl_clean_objs
impl_clean_objs:
ifneq ($(BUILD_TYPE), mst)
ifneq ($(strip $(clean_objs)),)
	@echo $(quote)$(call shcmd_rm, $(clean_objs))$(quote)
	-@$(call shcmd_rm, $(clean_objs))
	$(eval CLEAN_NOTHING_MSG := $(null_out))
endif # clean_output
endif # mst

# Make object directories
# The clean files variable will already be
# filtered for those that exist.
# NB. Clear CLEAN_NOTHING_MSG on invocation
.PHONY:impl_clean_output
impl_clean_output:
ifneq ($(BUILD_TYPE), mst)
ifneq ($(clean_output),)
	@echo $(quote)$(call shcmd_rm, $(clean_output))$(quote)
	-@$(call shcmd_rm, $(clean_output))
	$(eval CLEAN_NOTHING_MSG := $(null_out))
endif # clean_output
endif # mst

# Write result of mkdirs
.PHONY:impl_postclean_msg
impl_postclean_msg:
ifneq ($(BUILD_TYPE), mst)
	@echo $(quote)$(CLEAN_NOTHING_MSG)$(quote)
	@$(shcmd_newline)
endif

################################
# MAKING AND LINKING
################################

# Write make leader
.PHONY:impl_premaking_leader
impl_premaking_leader:
ifneq ($(BUILD_TYPE), mst)
	@echo $(quote)MAKING FILES ...$(quote)
endif

# Write result of making
.PHONY:impl_postmaking_msg
impl_postmaking_msg:
ifneq ($(BUILD_TYPE), mst)
	@echo $(quote)$(MAKEFILES_NOTHING_MSG)$(quote)
	@$(shcmd_newline)
endif

# Link output
.PHONY:impl_linker
impl_linker:
ifneq ($(BUILD_TYPE), mst)
	@echo $(quote)OUTPUT: $(targ_out)$(quote)
	@$(shcmd_newline)
	$(link_cmd)
ifdef randlib_cmd
	@$(shcmd_newline)
	$(randlib_cmd)
endif # randlib_cmd
	@$(shcmd_newline)

endif # mst


################################
# MULTIPLE MAKES
################################

# Implement multiple TARGET or ARCH is builds
.PHONY:impl_multimake
impl_multimake:
ifdef multiarch0
	@$(maker) -f $(MAKE_FILENAME) $(MAKECMDGOALS) CONF=$(conf) TARGET=$(targinit) \
ARCH=$(multiarch0) PLATOS=$(PLATOS) SRCFILES=$(SRCFILES) RUNOPTS=$(RUNOPTS)
endif # multiarch0
ifdef multitarg0
	@$(maker) -f $(MAKE_FILENAME) $(MAKECMDGOALS) CONF=$(conf) TARGET=$(multitarg0) \
ARCH=$(arch_init) PLATOS=$(PLATOS) SRCFILES=$(SRCFILES) RUNOPTS=$(RUNOPTS)
endif # multitarg0


##################################################
# 7. IMPLICIT BUILD RULES
##################################################

# Object rule.
# NB. Clear MAKEFILES_NOTHING_MSG on invocation
$(OBJ_DIR)%$(obj_fext): $(SRC_ROOT)%
ifneq ($(BUILD_TYPE), mst)
	$(compiler) $(c_flag) $(cxx_flags) $(o_flag) $@ $(SRC_ROOT)$*
	$(eval MAKEFILES_NOTHING_MSG := $(null_out))
endif

# Resource rule
# NB. Clear MAKEFILES_NOTHING_MSG on invocation
$(OBJ_DIR)%$(res_fext): $(SRC_ROOT)%
ifneq ($(BUILD_TYPE), mst)
	$(res_compiler) $(res_flags) $(o_flag) $@ $(SRC_ROOT)$*
	$(eval MAKEFILES_NOTHING_MSG := $(null_out))
endif

# Call dependencies
%$(dep_dummy_fext): %
	@$(maker) -f $(MAKE_FILENAME) $(MAKECMDGOALS) CONF=$* TARGET=$(TARGET) \
ARCH=$(arch_init) PLATOS=$(PLATOS) SRCFILES=$(SRCFILES) RUNOPTS=$(RUNOPTS)

#############################################################################
# EOF
#############################################################################
