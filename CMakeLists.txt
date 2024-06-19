cmake_minimum_required(VERSION 3.5)
project("fastweb")

# 设置全局属性
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

 
# 设置自定义配置类型 
if(MSVC)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Build config types" FORCE)
endif()  
# C++等级 
set(CMAKE_CXX_STANDARD 17)  
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
	include_directories(${PROJECT_SOURCE_DIR}/3rdparty/HP-Socket/Windows/Include)
	include_directories(${YLIB}/include)
	include_directories(D:/lib/ylib/3rdparty)
	add_compile_options(/W3 /wd4819)

else()
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty)
    include_directories(/usr/include/lua5.3/)
endif()



# 根据构建类型添加定义
add_library(${FASTWEBCORE} SHARED ${SOURCE_FILES}  ${HEADER_FILES})
set_target_properties(${FASTWEBCORE} PROPERTIES OUTPUT_NAME_DEBUG "${FASTWEBCORE}_d" OUTPUT_NAME_RELEASE ${FASTWEBCORE})








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
			$<$<CONFIG:Debug>:${YLIB}/lib/leveldb_d.lib>
			$<$<CONFIG:Debug>:${YLIB}/lib/ylib_d.lib>
			$<$<CONFIG:Debug>:${YLIB}/lib/sqlite3_d.lib>
			
			$<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/3rdparty/HP-Socket/Lib/HPSocket.lib>
			$<$<CONFIG:Release>:${YLIB}/lib/leveldb.lib>
			$<$<CONFIG:Release>:${YLIB}/lib/ylib.lib>
			$<$<CONFIG:Release>:${YLIB}/lib/sqlite3.lib>
	)

else()
	set(CMAKE_INSTALL_RPATH "/usr/local/lib")
	link_directories(/usr/lib/x86_64-linux-gnu)
	link_directories(/usr/local/lib)
	target_link_libraries(${FASTWEBCORE} 
			hpsocket
			ylib
			leveldb
			crypto
			lua5.3
			pthread
			sqlite3
			dl
			rt
			stdc++fs
	)

endif()

add_definitions(-DFASTWEB_EXE)
# 编译测试调用示例
add_executable(${PROJECT_NAME} 
	tests/main.cpp
	tests/fastweb.cpp
)
if(MSVC)
	target_link_libraries(${PROJECT_NAME} PRIVATE
				${FASTWEBCORE}
				odbc32.lib
				User32.lib
				Advapi32.lib
				IPHLPAPI.lib
				WS2_32.lib
				Shell32.lib
				${YLIB}/lib/libcrypto_static_win64.lib
				$<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/3rdparty/HP-Socket/Lib/HPSocket_D.lib>
				$<$<CONFIG:Debug>:${YLIB}/lib/ylib_d.lib>

				$<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/3rdparty/HP-Socket/Lib/HPSocket.lib>
				$<$<CONFIG:Release>:${YLIB}/lib/ylib.lib>
				
		)
else()
	target_link_libraries(${PROJECT_NAME} 
			${FASTWEBCORE}
			hpsocket
			ylib
			pthread
	)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
 


########################  安装  ########################
if(MSVC)
	install(TARGETS ${FASTWEBCORE} DESTINATION $<IF:$<CONFIG:Debug>,bin/debug,bin/release>)
	install(TARGETS ${PROJECT_NAME} DESTINATION $<IF:$<CONFIG:Debug>,bin/debug,bin/release>)
	install(FILES ${PROJECT_SOURCE_DIR}/3rdparty/HP-Socket/Lib/HPSocket_D.dll DESTINATION bin/debug)
	install(FILES ${PROJECT_SOURCE_DIR}/3rdparty/HP-Socket/Lib/HPSocket.dll DESTINATION bin/release)

	install(FILES config.ini DESTINATION bin/debug)
	install(FILES config.ini DESTINATION bin/release)
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/www DESTINATION bin/debug)
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/config DESTINATION bin/debug)
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/www DESTINATION bin/release)
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/config DESTINATION bin/release) 
	install(FILES ${PROJECT_SOURCE_DIR}/src/core/entry.h DESTINATION include)
	install(FILES ${PROJECT_SOURCE_DIR}/include/dll_interface.h DESTINATION include)
	install(FILES ${PROJECT_SOURCE_DIR}/src/module/basemodule.h DESTINATION include)

	install(DIRECTORY ${PROJECT_SOURCE_DIR}/3rdparty/lua/include/ DESTINATION include/lua)
else()
	
	install(TARGETS ${FASTWEBCORE} DESTINATION lib)
	install(TARGETS ${PROJECT_NAME} DESTINATION bin)

	install(FILES ${PROJECT_SOURCE_DIR}/src/core/entry.h DESTINATION include/${PROJECT_NAME})
	install(FILES ${PROJECT_SOURCE_DIR}/include/dll_interface.h DESTINATION include/${PROJECT_NAME})
	install(FILES ${PROJECT_SOURCE_DIR}/src/module/basemodule.h DESTINATION include/${PROJECT_NAME})


	install(FILES config.ini DESTINATION share/${PROJECT_NAME})
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/www DESTINATION share/${PROJECT_NAME})
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/config DESTINATION share/${PROJECT_NAME})

endif()




install(DIRECTORY ${PROJECT_SOURCE_DIR}/3rdparty/sol DESTINATION include)

