# CMake entry point
cmake_minimum_required(VERSION 3.0)
project(Tutorials)

find_package(OpenGL REQUIRED)
FIND_PACKAGE(ASSIMP REQUIRED)


if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)")
endif ()
if (CMAKE_SOURCE_DIR MATCHES " ")
    message("Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause.")
endif ()
if (CMAKE_BINARY_DIR MATCHES " ")
    message("Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause.")
endif ()


# Compile external dependencies
add_subdirectory(external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if (INCLUDE_DISTRIB)
    add_subdirectory(distrib)
endif (INCLUDE_DISTRIB)


LINK_DIRECTORIES(/usr/local/lib)

include_directories(
        external/glfw-3.3.4/include/
        external/glew-1.13.0/include/
        src/vendor/
)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130
        )

#link_directories(external/assimp-5.0.1/lib)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

add_executable(src

        src/shapes.h

        src/application.h
        src/application.cpp

        src/GLDebug.cpp
        src/GLDebug.h

        src/Shader.h

        src/Texture.cpp
        src/Texture.h

        src/Camera.h

        src/logger/Logger.cpp
        src/logger/Logger.h

        src/vendor/stb_image/stb_image.h
        src/vendor/stb_image/stb_image.cpp

        src/vendor/imgui/imconfig.h
        src/vendor/imgui/imgui.cpp
        src/vendor/imgui/imgui.h
        src/vendor/imgui/imgui_demo.cpp
        src/vendor/imgui/imgui_draw.cpp
        src/vendor/imgui/imgui_internal.h
        src/vendor/imgui/imgui_tables.cpp
        src/vendor/imgui/imgui_widgets.cpp
        src/vendor/imgui/imstb_rectpack.h
        src/vendor/imgui/imstb_textedit.h
        src/vendor/imgui/imstb_truetype.h
        src/vendor/imgui/imgui_impl_glfw.cpp
        src/vendor/imgui/imgui_impl_glfw.h
        src/vendor/imgui/imgui_impl_opengl3.cpp
        src/vendor/imgui/imgui_impl_opengl3.h
        src/vendor/imgui/imgui_impl_opengl3_loader.h

        src/Mesh.h src/Mesh.cpp src/Model.h src/Model.cpp
        )

target_link_libraries(src ${ALL_LIBS} ${ASSIMP_LIBRARIES})
# Xcode and Visual working directories
set_target_properties(src PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")
create_target_launcher(src WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/")
create_default_target_launcher(src WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/") # tut 1 is not the default or people would complain that tut 2 doesn't work

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*")
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$")


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode")

elseif (${CMAKE_GENERATOR} MATCHES "Xcode")

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode")

