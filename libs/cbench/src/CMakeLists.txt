add_library(cbench cbench.c)
if (UNIX)
  target_link_libraries(cbench m)
endif(UNIX)

set_property(TARGET cbench PROPERTY C_STANDARD 11)

install(TARGETS cbench DESTINATION lib)
