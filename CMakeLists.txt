cmake_minimum_required(VERSION 3.20)
project(KebabWay LANGUAGES C CXX)

# En güncel C++ standardı (C++23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Kaynak dosyaları topla
file(GLOB SRC_FILES CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Ana uygulama hedefi
add_executable(${PROJECT_NAME} ${SRC_FILES})

# resources klasörünün tam yolu
set(RESOURCES_PATH "${CMAKE_SOURCE_DIR}/resources/")
target_compile_definitions(${PROJECT_NAME} PRIVATE
        RESOURCES_PATH="${RESOURCES_PATH}"
)

# -------------------------
# GLAD (C API)
# -------------------------
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# -------------------------
# GLFW + OpenGL
# -------------------------
if (WIN32)
    # Windows (Visual Studio, MSVC)
    # GLFW'yi vcpkg ile kurduysan:
    #   vcpkg install glfw3:x64-windows
    find_package(glfw3 CONFIG REQUIRED)

    target_link_libraries(${PROJECT_NAME} PRIVATE
            glad
            glfw
            opengl32   # Windows'ta OpenGL kütüphanesi
    )

else()
    # Linux / macOS
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW3 REQUIRED glfw3)

    target_include_directories(${PROJECT_NAME} PRIVATE
            ${GLFW3_INCLUDE_DIRS}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
            glad
            ${GLFW3_LINK_LIBRARIES}
    )

    # OpenGL (Linux/macOS)
    find_library(OPENGL_gl_LIBRARY NAMES GL OpenGL)
    if(OPENGL_gl_LIBRARY)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_gl_LIBRARY})
    endif()
endif()

# -------------------------
# Ortak include klasörleri
# -------------------------
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)
