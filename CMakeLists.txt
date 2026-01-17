cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")


project(${MAIN_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

set(SFML_WARNINGS_AS_ERRORS OFF)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.2
        GIT_SHALLOW    1
)

FetchContent_MakeAvailable(SFML)


find_package(Threads REQUIRED)
if(APPLE)
elseif(UNIX)
    find_package(X11)
endif()

add_executable(${MAIN_EXECUTABLE_NAME}
        main.cpp
        Era.cpp
        Era.h
        Resursa.cpp
        Resursa.h
        Pozitie.cpp
        Pozitie.h
        CampDeLupta.cpp
        CampDeLupta.h
        Cladire.cpp
        Cladire.h
        Unitate.cpp
        Unitate.h
        Jucator.cpp
        Jucator.h
        Tile.cpp
        Tile.h
        MapRenderer.cpp
        MapRenderer.h
        Ferma.cpp
        Ferma.h
        Turn.cpp
        Turn.h
        Unitaticoncrete.h
        Cazarma.cpp
        Cazarma.h
        Game.cpp
        Game.h
        Exceptions.h
        Hud.cpp
        Hud.h
        ActionPanel.cpp
        ActionPanel.h
        Unitaticoncrete.cpp
        MathUtils.cpp
        MathUtils.h
        EntityFactory.cpp
        EntityFactory.h
        ManagerEntitati.cpp
        ManagerEntitati.h
        Observer.cpp
        Observer.h
        Piata.cpp
        Piata.h
)

set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ${SFML_SOURCE_DIR}/include)
target_link_directories(${MAIN_EXECUTABLE_NAME} PRIVATE ${SFML_BINARY_DIR}/lib)
target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System Threads::Threads)

if(APPLE)
elseif(UNIX)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
endif()


install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()


copy_files(FILES tastatura.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
copy_files(DIRECTORY fonts texturi COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})

set(ASSETS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/texturi")
set(ASSETS_DEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/texturi")
