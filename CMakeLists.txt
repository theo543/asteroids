cmake_minimum_required(VERSION 3.24)

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing executable name in this file
# for now, the project name is used as the executable name
project(asteroids)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # generate compile_commands.json for cppcheck, clang-tidy
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden) # fixes macOS warnings about translation units having different visibility settings

option(WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

# disable sanitizers when releasing executables without explicitly requested debug info
# use generator expressions to set flags correctly in both single and multi config generators
set(is_debug "$<CONFIG:Debug>")
set(is_rel_with_deb "$<CONFIG:RelWithDebInfo>")
set(debug_mode "$<OR:${is_debug},${is_rel_with_deb}>")
set(release_mode "$<NOT:${debug_mode}>")

###############################################################################

# custom functions

function(set_custom_stdlib_and_sanitizers target add_apple_asan)
    if(MSVC)
        # see https://gitlab.kitware.com/cmake/cmake/-/issues/24922
        set_target_properties(${target} PROPERTIES VS_USER_PROPS "${CMAKE_SOURCE_DIR}/disable_modules.props")
        target_compile_options(${target} PRIVATE "$<${debug_mode}:/fsanitize=address>" /experimental:module-)
        return()
    endif()

    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND NOT WIN32)
        # libc++ doesn't seem to exist on Windows
        target_compile_options(${target} PRIVATE -stdlib=libc++)
        target_link_options(${target} PRIVATE -stdlib=libc++)
    endif()

    if(APPLE)
        # first check Apple since Apple is also a kind of Unix
        if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND add_apple_asan MATCHES true)
            target_compile_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
            target_link_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
        endif()
    elseif(UNIX)
        if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
            if("${CMAKE_CXX_COMPILER_VERSION}" MATCHES "12." OR "${CMAKE_CXX_COMPILER_VERSION}" MATCHES "14.")
                # check leaks on Linux since macOS does not support the leaks sanitizer yet
                # leaks sanitizer is enabled by default on Linux, so we do not need to enable it explicitly
                target_compile_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
                target_link_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
            elseif("${CMAKE_CXX_COMPILER_VERSION}" MATCHES "13.")
                # use semi-colons instead of spaces to separate arguments
                # it is recommended to quote generator expressions in order to avoid unintentional splitting
                target_compile_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=memory,undefined;-fsanitize-recover=memory,undefined;-fsanitize-memory-track-origins>")
                target_link_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=memory,undefined;-fsanitize-recover=memory,undefined;-fsanitize-memory-track-origins;-Wl,-rpath,tools/llvm-project/build/lib>")
            else()
                message("No matching Clang version to add sanitizer flags!")
            endif()
        endif()
    endif()
endfunction()

###############################################################################

# external dependencies with FetchContent
include(FetchContent)

set(BUILD_SHARED_LIBS FALSE)
set(SFML_BUILD_NETWORK FALSE)

# NOTE: Also update SFML_VERSION env var in .github/workflows/cmake.yml:122
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG        a21c3c101cd02e41b79ffc515f93c649e230e263 # 2.6.x as of 2023-05-03
  # GIT_TAG        863fef024619da7f59b6404087b454337a172ac1 # 2.6.x as of 2022-08-30
  # GIT_TAG        72d88033e2f24be0fb1e9df614a56f3d5274154c # master as of 2022-08-30
  # GIT_TAG        f7c88ee7ef4e1c705531cd614efb7dcff1f873cb # last commit merged in master before API breakage (2022-04-21)
  # GIT_TAG        origin/master
  # GIT_TAG        origin/2.6.x
  # GIT_SHALLOW    1  # works only with branches or tags, not with arbitrary commit hashes
)


FetchContent_MakeAvailable(SFML)

set_custom_stdlib_and_sanitizers(sfml-system false)
set_custom_stdlib_and_sanitizers(sfml-window false)
set_custom_stdlib_and_sanitizers(sfml-graphics false)
set_custom_stdlib_and_sanitizers(sfml-audio false)

###############################################################################

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        a33701196adfad74917046096bf5a2aa0ab0bb50 # 9.1.0
)

FetchContent_MakeAvailable(fmt)

set_custom_stdlib_and_sanitizers(fmt false)

###############################################################################

# Header-only library
FetchContent_Declare(
        atomic_queue
        GIT_REPOSITORY https://github.com/max0x7ba/atomic_queue.git
        GIT_TAG        681544fd62c0b953a33d30bcfb1a15b9eb7da64b # 1.4
)

FetchContent_MakeAvailable(atomic_queue)

###############################################################################

FetchContent_Declare(
        cs_libguarded
        GIT_REPOSITORY https://github.com/copperspice/cs_libguarded.git
        GIT_TAG        2b0fc6537011b9f3a4b296170bdecbd28a270aa2 # 1.4.0
)

FetchContent_Populate(cs_libguarded) # This lib's CMakeLists.txt is used for testing and not for building the library

###############################################################################

FetchContent_Declare(
        eternal
        GIT_REPOSITORY https://github.com/mapbox/eternal.git
        GIT_TAG        dd2f5b9ff38fcd36b59efd9d289127fa73efc6cb # latest commit as of 25.05.2023
)

FetchContent_Populate(eternal)

###############################################################################

FetchContent_Declare(
        bezier
        GIT_REPOSITORY https://github.com/oysteinmyrmo/bezier.git
        GIT_TAG        ca52f1f671b80563f480dbd08b33fd381401133b # committed at 12 July 2022
)

FetchContent_Populate(bezier)

###############################################################################

# external dependencies with find_package

find_package(Threads REQUIRED)
if(APPLE)
elseif(UNIX)
    find_package(X11)
endif()

###############################################################################

FetchContent_Declare(
        portable_file_dialogs
        GIT_REPOSITORY https://github.com/samhocevar/portable-file-dialogs.git
        GIT_TAG        8ccf2a93f70d021842e72fd950eac2a0bb244eec # 0.1.0
)

FetchContent_MakeAvailable(portable_file_dialogs)

add_library(portable_file_dialogs_wrapper STATIC src/utility/message.cpp)
target_include_directories(portable_file_dialogs_wrapper SYSTEM PRIVATE ${portable_file_dialogs_SOURCE_DIR})
set_custom_stdlib_and_sanitizers(portable_file_dialogs_wrapper false)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(portable_file_dialogs_wrapper PRIVATE -femulated-tls)
endif()


###############################################################################

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing name here
add_executable(${PROJECT_NAME}
        src/main/main.cpp
        src/main/MainLoop.h src/main/MainLoop.cpp
        src/main/Menu.h src/main/Menu.cpp
        src/menu/UI.h src/menu/UI.cpp
        src/menu/UIItem.h src/menu/UIItem.cpp
        src/menu/UILabel.h src/menu/UILabel.cpp
        src/menu/UIOption.h src/menu/UIOption.cpp
        src/utility/Defer.h
        src/world/SwitchCommand.cpp src/world/SwitchCommand.h
        src/world/SwitchFactory.h src/world/SwitchFactory.tpp
        src/world/WorldInterface.h src/world/WorldInterface.cpp
        src/world/WorldBase.h src/world/WorldBase.cpp
        src/basic_test/TimerTest.h src/basic_test/TimerTest.cpp
        src/bounce_test/BounceTest.h src/bounce_test/BounceTest.cpp
        src/bounce_test/Bouncy.cpp src/bounce_test/Bouncy.h
        src/menu/Stats.h src/menu/Stats.cpp
        src/physics/GameObject.cpp src/physics/GameObject.h
        src/physics/Physics.cpp src/physics/Physics.h
        src/bounce_test/Player.cpp src/bounce_test/Player.h
        src/main/game_exception.cpp src/main/game_exception.h
        src/menu/ui_item_not_found.cpp src/menu/ui_item_not_found.h
        src/menu/unwanted_notify_tick_call.cpp src/menu/unwanted_notify_tick_call.h
        src/world/type_not_of_claimed_base.cpp src/world/type_not_of_claimed_base.h
        src/resources/EmbeddedAccessor.h
        src/resources/ResourceLoader.h
        src/resources/OnDemandLoader.tpp src/resources/OnDemandLoader.h
        src/resources/GlobalLoaders.cpp src/resources/GlobalLoaders.h
        src/resources/singleton_instance_error.cpp src/resources/singleton_instance_error.h
        src/SFML.h
        src/physics/PhysicsData.cpp src/physics/PhysicsData.h
        src/physics/AABB.cpp src/physics/AABB.h
        src/utility/minmax.h
        src/menu/RainbowBackground.cpp src/menu/RainbowBackground.h
        src/utility/message.h
        src/physics/CollisionTest.cpp src/physics/CollisionTest.h
        src/asteroids/AsteroidShape.cpp src/asteroids/AsteroidShape.h
        src/asteroids/Asteroid.cpp src/asteroids/Asteroid.h
        src/asteroids/AsteroidsWorld.cpp src/asteroids/AsteroidsWorld.h
)
# header base path to prevent '..' in #include statements
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/")

find_program(CPPCHECK_EXECUTABLE NAMES cppcheck)
if(CPPCHECK_EXECUTABLE)
    message(STATUS "cppcheck found, target 'cppcheck' available")
    get_filename_component(BUILD_DIR_SUPPRESS "${CMAKE_BINARY_DIR}" ABSOLUTE)
    string(REGEX REPLACE "^[A-Za-z]:" "" BUILD_DIR_SUPPRESS "${BUILD_DIR_SUPPRESS}") # cppcheck doesn't like drive letters in suppress for some reason
    add_custom_target(cppcheck
        COMMAND ${CPPCHECK_EXECUTABLE} --enable=all
                                       --inline-suppr
                                       --project=${CMAKE_BINARY_DIR}/compile_commands.json
                                       -i${CMAKE_BINARY_DIR} --suppress=*:*${BUILD_DIR_SUPPRESS}*
                                       --suppress=missingIncludeSystem
                                       --suppress=unmatchedSuppression
                                       --suppress=useStlAlgorithm
                                       --suppress=unusedFunction
                                       --suppress=preprocessorErrorDirective
                                       --error-exitcode=1
        COMMENT "Running cppcheck"
        VERBATIM
        USES_TERMINAL # disables output buffering
    )
else()
    message(STATUS "cppcheck not found, target 'cppcheck' not available")
endif()

###############################################################################

add_executable(asteroids_resource_script src/resources/build/script.cpp src/resources/build/script_header.h)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz) # URL recommended on GitHub
FetchContent_MakeAvailable(json)
target_link_libraries(asteroids_resource_script PRIVATE nlohmann_json::nlohmann_json)
FetchContent_Declare(
        inja
        GIT_REPOSITORY https://github.com/pantor/inja.git
        GIT_TAG        b2276440be8334aeba9cd5d628c2731d0f6a5809 # 3.4
)
FetchContent_Populate(inja)
target_include_directories(asteroids_resource_script SYSTEM PRIVATE ${inja_SOURCE_DIR}/include)

file(GLOB_RECURSE RESOURCE_FILES CONFIGURE_DEPENDS "resources/*")

set(RESOURCE_EMBEDDED "${CMAKE_CURRENT_BINARY_DIR}/embedded_resources.cpp")
set(RESOURCE_SCHEMA "${CMAKE_CURRENT_SOURCE_DIR}/src/resources/build/schema.json")
set(RESOURCE_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/src/resources/build/template.inja.inc")
file(TOUCH "${RESOURCE_EMBEDDED}")

add_custom_command(
        OUTPUT ${RESOURCE_EMBEDDED}
        COMMAND asteroids_resource_script
        "${RESOURCE_EMBEDDED}"
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "${RESOURCE_SCHEMA}"
        "${RESOURCE_TEMPLATE}"
        DEPENDS "${RESOURCE_FILES}" "${RESOURCE_SCHEMA}" "${RESOURCE_TEMPLATE}" asteroids_resource_script
)

target_sources(${PROJECT_NAME} PRIVATE ${RESOURCE_EMBEDDED})

###############################################################################

if(NO_PRECOMPILED_HEADERS)
    message(STATUS "Precompiled headers disabled")
else()
    target_precompile_headers(asteroids_resource_script PRIVATE src/resources/build/script_header.h)
    target_precompile_headers(asteroids PRIVATE src/SFML.h)
endif()


###############################################################################

# target definitions

if(GITHUB_ACTIONS)
  message("NOTE: GITHUB_ACTIONS defined")
  target_compile_definitions(${PROJECT_NAME} PRIVATE GITHUB_ACTIONS)
endif()

###############################################################################

if(WARNINGS_AS_ERRORS)
    set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)
endif()

# custom compiler flags
message("Compiler: ${CMAKE_CXX_COMPILER_ID} version ${CMAKE_CXX_COMPILER_VERSION}")
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive- /wd4244 /wd4267 /wd4996 /external:anglebrackets /external:W0)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

###############################################################################

# sanitizers
set_custom_stdlib_and_sanitizers(${PROJECT_NAME} true)

###############################################################################

# use SYSTEM so clang-tidy does not report warnings from these directories
#target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ext/<SomeHppLib>/include)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${SFML_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${fmt_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${atomic_queue_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${cs_libguarded_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${eternal_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${bezier_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE portable_file_dialogs_wrapper)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system sfml-audio Threads::Threads)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

if(APPLE)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE X11)
endif()

###############################################################################

option(HIDE_TERMINAL_ON_RELEASE "Hide terminal when building a release on Windows" ON)
if(WIN32 AND HIDE_TERMINAL_ON_RELEASE)
    set(msvc "$<CXX_COMPILER_ID:MSVC>")
    set(msvc_and_release "$<AND:${msvc},${release_mode}>")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ${release_mode})
    target_link_options(${PROJECT_NAME} PRIVATE $<${msvc_and_release}:/ENTRY:mainCRTStartup>)
endif()

###############################################################################

# copy binaries to "bin" folder; these are uploaded as artifacts on each release
# update name in .github/workflows/cmake.yml:29 when changing "bin" name here
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
# install(DIRECTORY some_dir1 some_dir2 DESTINATION bin)
# install(FILES some_file1.txt some_file2.md DESTINATION bin)
