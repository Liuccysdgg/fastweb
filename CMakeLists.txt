cmake_minimum_required(VERSION 3.5)
project("fastweb")

# 设置全局属性
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# 设置自定义配置类型
if(MSVC)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Build config types" FORCE)
endif()
# C++等级
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(FASTWEBCORE ${PROJECT_NAME}core)
# 安装复制
set(CMAKE_INSTALL_ALWAYS_COPY TRUE)
# 设置根目录
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# 添加三方模块
if(MSVC)
	add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/lua)
 endif()


# Recursively get all source files
file(GLOB_RECURSE SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
file(GLOB_RECURSE HEADER_FILES
    "${PROJECT_SOURCE_DIR}/src/*.h"
)
foreach(source IN LISTS SOURCE_FILES)
    get_filename_component(source_path "${source}" PATH)
    file(RELATIVE_PATH source_path_rel "${PROJECT_SOURCE_DIR}" "${source_path}")
    string(REPLACE "/" "\\" source_path_rel_win "${source_path_rel}") 
    source_group(${source_path_rel_win} FILES "${source}")
endforeach()
foreach(header IN LISTS HEADER_FILES)
    get_filename_component(header_path "${header}" PATH)
    file(RELATIVE_PATH header_path_rel "${PROJECT_SOURCE_DIR}" "${header_path}")
    string(REPLACE "/" "\\" header_path_rel_win "${header_path_rel}")
    source_group(${header_path_rel_win} FILES "${header}")
endforeach()

set(YLIB ${CMAKE_INSTALL_PREFIX}/../ylib)
include_directories(${PROJECT_SOURCE_DIR}/src)
if(MSVC)
	
	include_directories(${PROJECT_SOURCE_DIR}/src)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty)
	include_directories(${PROJECT_SOURCE_DIR}/3rdparty/lua/include)
	include_directories(${PROJECT_SOURCE_DIR}/3rdparty/soci/include)
	include_directories(${PROJECT_SOURCE_DIR}/3rdparty/mysql/include/jdbc)
	include_directories(${PROJECT_SOURCE_DIR}/3rdparty/HP-Socket/Windows/Include)
	include_directories(${YLIB}/include)
	add_compile_options(/W3 /wd4819)
else()
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty)
    include_directories(/usr/include/lua5.3/)
endif()


# 根据构建类型添加定义
add_library(${FASTWEBCORE} SHARED ${SOURCE_FILES}  ${HEADER_FILES})
set_target_properties(${FASTWEBCORE} PROPERTIES OUTPUT_NAME_DEBUG "${FASTWEBCORE}_d" OUTPUT_NAME_RELEASE ${FASTWEBCORE})






link_directories(/usr/lib/x86_64-linux-gnu)
link_directories(/usr/local/lib)

if(MSVC)
	target_link_libraries(${FASTWEBCORE} PRIVATE
			odbc32.lib
			User32.lib
			Advapi32.lib
			IPHLPAPI.lib
			WS2_32.lib
			Shell32.lib
			lualib
			${YLIB}/lib/libcrypto_static_win64.lib
			$<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/3rdparty/HP-Socket/Lib/HPSocket_D.lib>
			$<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/3rdparty/mysql/lib/Debug/mysqlcppconn.lib>
			$<$<CONFIG:Debug>:${YLIB}/lib/leveldb_d.lib>
			$<$<CONFIG:Debug>:${YLIB}/lib/libzip_d.lib>
			$<$<CONFIG:Debug>:${YLIB}/lib/sqlite3_d.lib>
			$<$<CONFIG:Debug>:${YLIB}/lib/ylib_d.lib>
			$<$<CONFIG:Debug>:${YLIB}/lib/zlib_d.lib>
			$<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/3rdparty/soci/lib/Debug/libsoci_core_4_1.lib>
			$<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/3rdparty/soci/lib/Debug/libsoci_empty_4_1.lib>
			$<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/3rdparty/soci/lib/Debug/libsoci_odbc_4_1.lib>
			$<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/3rdparty/soci/lib/Debug/soci_core_4_1.lib>
			$<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/3rdparty/soci/lib/Debug/soci_empty_4_1.lib>
			$<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/3rdparty/soci/lib/Debug/soci_odbc_4_1.lib>
		
			$<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/3rdparty/HP-Socket/Lib/HPSocket.lib>
			$<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/3rdparty/mysql/lib/Release/mysqlcppconn.lib>
			$<$<CONFIG:Release>:${YLIB}/lib/leveldb.lib>
			$<$<CONFIG:Release>:${YLIB}/lib/libzip.lib>
			$<$<CONFIG:Release>:${YLIB}/lib/sqlite3.lib>
			$<$<CONFIG:Release>:${YLIB}/lib/ylib.lib>
			$<$<CONFIG:Release>:${YLIB}/lib/zlib.lib>
		
			$<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/3rdparty/soci/lib/Release/libsoci_core_4_1.lib>
			$<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/3rdparty/soci/lib/Release/libsoci_empty_4_1.lib>
			$<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/3rdparty/soci/lib/Release/libsoci_odbc_4_1.lib>
			$<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/3rdparty/soci/lib/Release/soci_core_4_1.lib>
			$<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/3rdparty/soci/lib/Release/soci_empty_4_1.lib>
			$<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/3rdparty/soci/lib/Release/soci_odbc_4_1.lib>
	)

else()
	target_link_libraries(${FASTWEBCORE} 
			hpsocket
			ylib
			leveldb
			soci_core
			soci_firebird
			soci_mysql
			soci_odbc
			soci_postgresql
			soci_sqlite3
			crypto
			lua5.3
			mysqlcppconn
			pthread
	)

endif()

# 编译测试调用示例
add_executable(${PROJECT_NAME} tests/main.cpp)
target_link_libraries(${PROJECT_NAME} ${FASTWEBCORE})
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

# 查找并添加所有模块
file(GLOB MODULES RELATIVE ${CMAKE_SOURCE_DIR}/module ${CMAKE_SOURCE_DIR}/module/*)
foreach(MODULE ${MODULES})
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/module/${MODULE})
        add_subdirectory(${CMAKE_SOURCE_DIR}/module/${MODULE})
    endif()
endforeach()


########################  安装  ########################
install(TARGETS ${FASTWEBCORE} DESTINATION $<IF:$<CONFIG:Debug>,bin/debug,bin/release>)
install(TARGETS ${PROJECT_NAME} DESTINATION $<IF:$<CONFIG:Debug>,bin/debug,bin/release>)
if(MSVC)
	install(FILES ${PROJECT_SOURCE_DIR}/3rdparty/HP-Socket/Lib/HPSocket_D.dll DESTINATION bin/debug)
	install(FILES ${PROJECT_SOURCE_DIR}/3rdparty/mysql/lib/Debug/libcrypto-3-x64.dll DESTINATION bin/debug)
	install(FILES ${PROJECT_SOURCE_DIR}/3rdparty/mysql/lib/Debug/libssl-3-x64.dll DESTINATION bin/debug)
	install(FILES ${PROJECT_SOURCE_DIR}/3rdparty/mysql/lib/Debug/mysqlcppconn-9-vs14.dll DESTINATION bin/debug)

	install(FILES ${PROJECT_SOURCE_DIR}/3rdparty/HP-Socket/Lib/HPSocket.dll DESTINATION bin/release)
	install(FILES ${PROJECT_SOURCE_DIR}/3rdparty/mysql/lib/Release/libcrypto-3-x64.dll DESTINATION bin/release)
	install(FILES ${PROJECT_SOURCE_DIR}/3rdparty/mysql/lib/Release/libssl-3-x64.dll DESTINATION bin/release)
	install(FILES ${PROJECT_SOURCE_DIR}/3rdparty/mysql/lib/Release/mysqlcppconn-9-vs14.dll DESTINATION bin/release)
endif()
install(FILES config.ini DESTINATION bin/debug)
install(FILES config.ini DESTINATION bin/release)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/www DESTINATION bin/debug)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/config DESTINATION bin/debug)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/www DESTINATION bin/release)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/config DESTINATION bin/release) 
install(FILES ${PROJECT_SOURCE_DIR}/src/core/entry.h DESTINATION include)




