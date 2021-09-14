# This cmake file is to be included
# There are must be 
# 
# LINKLIBS
# COMPILE_OPTION
# SRC
# INCL
#
# variables

# This git submoudle must be located
# in (project_root)/lib
#########################################
#                                       #
#        Serial Port Driver             #
#   https://github.com/wjwwood/serial   #
#                                       #
#########################################

if(APPLE)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(FOUNDATION_LIBRARY Foundation)
endif()

## Sources
list(APPEND SRC
    lib/serial/src/serial.cc
    lib/serial/include/serial/serial.h
    lib/serial/include/serial/v8stdint.h
    )
if(APPLE)
    # If OSX
    list(APPEND SRC lib/serial/src/impl/unix.cc)
    list(APPEND SRC lib/serial/src/impl/list_ports/list_ports_osx.cc)
elseif(UNIX)
    # If unix
    list(APPEND SRC lib/serial/src/impl/unix.cc)
    list(APPEND SRC lib/serial/src/impl/list_ports/list_ports_linux.cc)
else()
    # If windows
    set(SRC ${SRC} lib/serial/src/impl/win.cc )
    set(SRC ${SRC} lib/serial/src/impl/list_ports/list_ports_win.cc )
endif()

## Add serial library
if(APPLE)
    set(LINKLIBS ${LINKLIBS} ${FOUNDATION_LIBRARY} ${IOKIT_LIBRARY} )
elseif(UNIX)
    set(LINKLIBS ${LINKLIBS} rt pthread )
else()
    set(LINKLIBS ${LINKLIBS} setupapi )
endif()

list(APPEND INCL
	${CMAKE_CURRENT_SOURCE_DIR}/lib/serial/include
)

message("CMAKE file for MB1434 is included")