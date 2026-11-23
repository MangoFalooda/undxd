# Linker options for Linux
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_options(-fPIC)
endif()

juce_add_binary_data(DexedResources SOURCES
    ui/dexedIcon.png
    ui/NotoSans-Bold.ttf
    ui/NotoSans-Regular.ttf
    builtin_pgm.zip
)
