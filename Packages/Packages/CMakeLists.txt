# Packages_Packages/CMakeLists.txt
# Created by Robin Rowe 2024-04-07
# License MIT Open Source

set (MODULE_NAME Packages_Packages)
message("Configuring ${MODULE_NAME} 1 source file(s)")
file(STRINGS sources.cmake SOURCES)
add_executable(${MODULE_NAME} ${SOURCES})
