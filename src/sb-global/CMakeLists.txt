configure_file(
    sb-global.h.in
    "${CMAKE_CURRENT_BINARY_DIR}/sb-global.h"
    @ONLY
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/sb-global.h"
    DESTINATION include/sb-global
)
