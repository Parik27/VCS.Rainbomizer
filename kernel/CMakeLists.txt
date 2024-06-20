# To run VCS Rainbomizer on a real psp, we need a bootstrapper that loads the module after the game.
# Otherwise, there's not enough memory for the game to load at its default address (0x8004000)

# This bootstrapper module does exactly that. It's a kernel module that's lightweight and can easily
# be loaded into memory.

add_executable(Bootstrapper main.c)
add_custom_command(
        TARGET Bootstrapper
        POST_BUILD
        COMMAND psp-fixup-imports Bootstrapper
        COMMAND psp-prxgen Bootstrapper Bootstrapper.prx
)
target_link_options(Bootstrapper PRIVATE
  -specs=${PSPDEV}/psp/sdk/lib/prxspecs
  -nostartfiles
  #-specs=${PROJECT_SOURCE_DIR}/prxspecs
  -Wl,-q,-T${PROJECT_SOURCE_DIR}/linkfile.prx
  ${PSPDEV}/psp/sdk/lib/prxexports.o
  -L -Wl,-zmax-page-size=128
  -flto
  -nostdlib
)
target_link_directories(Bootstrapper PUBLIC vendor)
target_link_libraries(Bootstrapper PRIVATE pspdebug pspdisplay_driver pspctrl_driver pspmodinfo pspsdk pspkernel)
