# Pacman_Pacman/CMakeLists.txt
# Created by Robin Rowe 2024-04-07
# License MIT Open Source

set (MODULE_NAME Pacman_Pacman)
message("Configuring ${MODULE_NAME} 7 source file(s)")
file(STRINGS sources.cmake SOURCES)
add_executable(${MODULE_NAME} ${SOURCES})
