# lumex/CMakeLists.txt

add_subdirectory(applied)
add_subdirectory(core)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LumexExport.hpp" DESTINATION include/lumex)
