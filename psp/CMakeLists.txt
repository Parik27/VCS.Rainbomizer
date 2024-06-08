add_library(psp_thirteenag_utils
  log.c
  injector.c
  patterns.c
  inireader.c
  gvm.c
  mips.c
  rini.c
)

target_compile_options(psp_thirteenag_utils PRIVATE  -Wno-implicit-function-declaration -Wno-int-conversion)
