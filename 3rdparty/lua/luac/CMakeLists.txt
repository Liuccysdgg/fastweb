cmake_minimum_required(VERSION 3.5)

# 获取上级目录名做为库名
get_filename_component(CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
get_filename_component(LUA_DIR ${CMAKE_CURRENT_BINARY_DIR} ABSOLUTE)
get_filename_component(LIBRARY_NAME ${CURRENT_DIR} NAME)

file(GLOB SOURCE_LIB_FILES "${PROJECT_SOURCE_DIR}/3rdparty/lua/*.c")
file(GLOB SOURCE_FILES "${PROJECT_SOURCE_DIR}/3rdparty/lua/luac/*.c")



# 创建LUAC
add_executable(${LIBRARY_NAME} WIN32 ${SOURCE_FILES} ${SOURCE_LIB_FILES})
# 设置 Windows 应用程序类型
if (WIN32)
    set_target_properties(${LIBRARY_NAME} PROPERTIES WIN32_EXECUTABLE FALSE)
endif()
# 将包含目录与目标相关联，这样只有在编译此库时才会包含这些目录
target_include_directories(${LIBRARY_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/3rdparty/lua")
if(MSVC)
target_link_libraries(${LIBRARY_NAME} PRIVATE kernel32.lib Shell32.lib)
else()
target_link_libraries(${LIBRARY_NAME} PRIVATE m)
endif()


# 设置 Visual Studio 中的文件夹
set_property(TARGET ${LIBRARY_NAME} PROPERTY FOLDER "3rdparty")