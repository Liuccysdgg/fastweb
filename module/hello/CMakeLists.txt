# 获取当前目录的名称
get_filename_component(MODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 设置项目名为当前目录名
project(${MODULE_NAME})

# 搜索源文件和头文件
file(GLOB_RECURSE SOURCE_FILES "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADER_FILES 
    "${PROJECT_SOURCE_DIR}/*.h"
    "../*.h"
)

# 将源文件分配到 Source Files 文件夹
foreach(source IN LISTS SOURCE_FILES)
    get_filename_component(source_path "${source}" PATH)
    file(RELATIVE_PATH source_path_rel "${PROJECT_SOURCE_DIR}" "${source_path}")
    string(REPLACE "/" "\\" source_path_rel_win "${source_path_rel}")
    source_group("Source Files\\${source_path_rel_win}" FILES "${source}")
endforeach()

# 将头文件分配到 Header Files 文件夹
foreach(header IN LISTS HEADER_FILES)
    get_filename_component(header_path "${header}" PATH)
    file(RELATIVE_PATH header_path_rel "${PROJECT_SOURCE_DIR}" "${header_path}")
    string(REPLACE "/" "\\" header_path_rel_win "${header_path_rel}")
    source_group("Header Files\\${header_path_rel_win}" FILES "${header}")
endforeach()

include_directories(${ROOT_DIR}/module)
include_directories(${ROOT_DIR}/3rdpary)
# 添加共享库
add_library(${MODULE_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})

if(MSVC)
	target_link_libraries(${MODULE_NAME} PRIVATE
			odbc32.lib
			User32.lib
			Advapi32.lib
			IPHLPAPI.lib
			WS2_32.lib
			Shell32.lib
			${YLIB}/lib/libcrypto_static_win64.lib
			$<$<CONFIG:Debug>:${ROOT_DIR}/3rdparty/HP-Socket/Lib/HPSocket_D.lib>
			$<$<CONFIG:Debug>:${ROOT_DIR}/3rdparty/mysql/lib/Debug/mysqlcppconn.lib>
			$<$<CONFIG:Debug>:${YLIB}/lib/leveldb_d.lib>
			$<$<CONFIG:Debug>:${YLIB}/lib/libzip_d.lib>
			$<$<CONFIG:Debug>:${YLIB}/lib/lua_d.lib>
			$<$<CONFIG:Debug>:${YLIB}/lib/sqlite3_d.lib>
			$<$<CONFIG:Debug>:${YLIB}/lib/ylib_d.lib>
			$<$<CONFIG:Debug>:${YLIB}/lib/zlib_d.lib>
			$<$<CONFIG:Debug>:${ROOT_DIR}/3rdparty/soci/lib/Debug/libsoci_core_4_1.lib>
			$<$<CONFIG:Debug>:${ROOT_DIR}/3rdparty/soci/lib/Debug/libsoci_empty_4_1.lib>
			$<$<CONFIG:Debug>:${ROOT_DIR}/3rdparty/soci/lib/Debug/libsoci_odbc_4_1.lib>
			$<$<CONFIG:Debug>:${ROOT_DIR}/3rdparty/soci/lib/Debug/soci_core_4_1.lib>
			$<$<CONFIG:Debug>:${ROOT_DIR}/3rdparty/soci/lib/Debug/soci_empty_4_1.lib>
			$<$<CONFIG:Debug>:${ROOT_DIR}/3rdparty/soci/lib/Debug/soci_odbc_4_1.lib>
			$<$<CONFIG:Release>:${ROOT_DIR}/3rdparty/HP-Socket/Lib/HPSocket.lib>
			$<$<CONFIG:Release>:${ROOT_DIR}/3rdparty/mysql/lib/Release/mysqlcppconn.lib>
			$<$<CONFIG:Release>:${YLIB}/lib/leveldb.lib>
			$<$<CONFIG:Release>:${YLIB}/lib/libzip.lib>
			$<$<CONFIG:Release>:${YLIB}/lib/lua.lib>
			$<$<CONFIG:Release>:${YLIB}/lib/sqlite3.lib>
			$<$<CONFIG:Release>:${YLIB}/lib/ylib.lib>
			$<$<CONFIG:Release>:${YLIB}/lib/zlib.lib>
			$<$<CONFIG:Release>:${ROOT_DIR}/3rdparty/soci/lib/Release/libsoci_core_4_1.lib>
			$<$<CONFIG:Release>:${ROOT_DIR}/3rdparty/soci/lib/Release/libsoci_empty_4_1.lib>
			$<$<CONFIG:Release>:${ROOT_DIR}/3rdparty/soci/lib/Release/libsoci_odbc_4_1.lib>
			$<$<CONFIG:Release>:${ROOT_DIR}/3rdparty/soci/lib/Release/soci_core_4_1.lib>
			$<$<CONFIG:Release>:${ROOT_DIR}/3rdparty/soci/lib/Release/soci_empty_4_1.lib>
			$<$<CONFIG:Release>:${ROOT_DIR}/3rdparty/soci/lib/Release/soci_odbc_4_1.lib>
	)
else()
	target_link_libraries(${MODULE_NAME} 
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
# 设置生成的项目文件夹为 module
set_property(TARGET ${MODULE_NAME} PROPERTY FOLDER "module")


install(TARGETS ${MODULE_NAME} DESTINATION $<IF:$<CONFIG:Debug>,bin/debug/module,bin/release/module>)