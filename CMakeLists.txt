cmake_minimum_required(VERSION 3.14)
project(VCS.Rainbomizer)

add_subdirectory(psp)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
  src/*.cc
  lib/*.cc
  #crt0_prx2.c
)

add_executable(${PROJECT_NAME} ${SOURCES})
add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND psp-fixup-imports ${PROJECT_NAME}
        COMMAND psp-prxgen ${PROJECT_NAME} ${PROJECT_NAME}.prx
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# we have two different gp values in game functions and in the prx module.
# We must not use gp ourselves.
target_compile_options(${PROJECT_NAME} PRIVATE
  -fno-exceptions
  -mno-gpopt
  -fno-use-cxa-atexit
  -lgcc
)
target_link_options(${PROJECT_NAME} PRIVATE
  -lgcc
  -fno-use-cxa-atexit
  -nostartfiles
  -specs=${PSPDEV}/psp/sdk/lib/prxspecs
  #-specs=${CMAKE_SOURCE_DIR}/prxspecs
  -Wl,-q,-T${PSPDEV}/psp/sdk/lib/linkfile.prx
  ${PSPDEV}/psp/sdk/lib/prxexports.o
  -L -Wl,-zmax-page-size=128
)
target_include_directories(${PROJECT_NAME} PUBLIC lib psp)
target_link_libraries(${PROJECT_NAME} PRIVATE psp_thirteenag_utils)
