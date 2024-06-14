add_library(psp_thirteenag_utils STATIC
  injector.c
  patterns.c
  mips.c
)

target_compile_options(psp_thirteenag_utils PRIVATE -Wno-implicit-function-declaration -Wno-int-conversion -mno-gpopt)
