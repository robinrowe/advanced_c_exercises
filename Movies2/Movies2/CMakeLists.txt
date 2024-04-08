# Movies2_Movies2/CMakeLists.txt
# Created by Robin Rowe 2024-04-07
# License MIT Open Source

set (MODULE_NAME Movies2_Movies2)
message("Configuring ${MODULE_NAME} 1 source file(s)")
file(STRINGS sources.cmake SOURCES)
add_executable(${MODULE_NAME} ${SOURCES})
