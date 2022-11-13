cmake_minimum_required(VERSION 3.19)

project(Kiwifield)

# sudo dnf groupinstall "Development Tools" "Development Libraries"
# sudo dnf install glew-devel SDL2-devel SDL2_image-devel glm-devel freetype-devel
# sudo dnf install g++
set(CMAKE_CXX_STANDARD 20)
set(KIWIFIELD_VERSION 0.1)

if(${CMAKE_SYSTEM} MATCHES "Linux")
    set(LINUX 1)
endif(${CMAKE_SYSTEM} MATCHES "Linux")

file (GLOB MY_SRC "src/*"
        "src/entities/*"
        "src/utility/*"
        "src/editor/*"
        "src/world/*"
        "src/lib/*")

if (APPLE)
    add_compile_options(-stdlib=libc++)
endif(APPLE)

add_executable(Kiwifield ${MY_SRC})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/assets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)

if (UNIX)
    # X11
    if(LINUX)
        find_package(X11 REQUIRED)
        include_directories(${X11_INCLUDE_DIR})
        link_directories(${X11_LIBRARIES})
        target_link_libraries(Kiwifield ${X11_LIBRARIES})
    endif(LINUX)
    # GLUT
    if(APPLE)
        find_package(GLUT REQUIRED)
        include_directories(${GLUT_INCLUDE_DIR})
        link_directories(${GLUT_LIBRARIES})
        target_link_libraries(Kiwifield ${GLUT_LIBRARIES})
    endif(APPLE)

    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})
    link_directories(${OPENGL_LIBRARIES})
    target_link_libraries(Kiwifield ${OPENGL_LIBRARIES})

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(Kiwifield Threads::Threads)

endif (UNIX)