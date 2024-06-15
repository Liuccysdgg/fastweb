cmake_minimum_required(VERSION 3.10)
project(lua)

#set(LUA_VERSION_MAJOR 5)
#set(LUA_VERSION_MINOR 4)
#set(LUA_VERSION_PATCH 4)

set(PJN_LUAEXE "lua")
set(PJN_LUALIB "lualib")

set(LUA_SRC
    src/lapi.c
    src/lcode.c
    src/lctype.c
    src/ldebug.c
    src/ldo.c
    src/ldump.c
    src/lfunc.c
    src/lgc.c
    src/llex.c
    src/lmem.c
    src/lobject.c
    src/lopcodes.c
    src/lparser.c
    src/lstate.c
    src/lstring.c
    src/ltable.c
    src/ltm.c
    src/lundump.c
    src/lvm.c
    src/lzio.c
    src/lauxlib.c
    src/lbaselib.c
    src/lcorolib.c
    src/ldblib.c
    src/liolib.c
    src/lmathlib.c
    src/loslib.c
    src/lstrlib.c
    src/ltablib.c
    src/lutf8lib.c
    src/loadlib.c
    src/linit.c
)
include_directories(${PJN_LUALIB} include)
include_directories(${PJN_LUAEXE} include)
add_library(${PJN_LUALIB} STATIC  ${LUA_SRC})

add_executable(${PJN_LUAEXE} src/lua.c)
target_link_libraries(${PJN_LUAEXE} ${PJN_LUALIB})

set_target_properties(${PJN_LUAEXE} PROPERTIES OUTPUT_NAME lua)
set_target_properties(${PJN_LUALIB} PROPERTIES OUTPUT_NAME lua)
#set_target_properties(${PJN_LUAEXE} PROPERTIES OUTPUT_NAME_DEBUG "${PJN_LUAEXE}_d" OUTPUT_NAME_RELEASE ${PJN_LUAEXE})
#set_target_properties(${PJN_LUALIB} PROPERTIES OUTPUT_NAME_DEBUG "${PJN_LUALIB}_d" OUTPUT_NAME_RELEASE ${PJN_LUALIB})

set_property(TARGET ${PJN_LUALIB} PROPERTY FOLDER "3rdparty")
set_property(TARGET ${PJN_LUAEXE} PROPERTY FOLDER "3rdparty")

install(TARGETS ${PJN_LUAEXE} DESTINATION $<IF:$<CONFIG:Debug>,bin/debug/3rdparty,bin/release/3rdparty>)
install(DIRECTORY include DESTINATION bin/debug/3rdparty)
install(DIRECTORY include DESTINATION bin/release/3rdparty)


install(TARGETS ${PJN_LUALIB} DESTINATION $<IF:$<CONFIG:Debug>,bin/debug/3rdparty/lib,bin/release/3rdparty/lib>)


