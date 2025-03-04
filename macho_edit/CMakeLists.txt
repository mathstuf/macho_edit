add_library(macho_edit-lib OBJECT)
target_sources(macho_edit-lib
  PRIVATE
    cpuinfo.cpp
    fileutils.cpp
    load_command.cpp
    macho.cpp
    macho_arch.cpp
    magicnames.cpp
  PUBLIC
    FILE_SET HEADERS FILES
      cpuinfo.h
      fileutils.h
      load_command.h
      macho.h
      macho_arch.h
      magicnames.h
      menu.h)
target_compile_features(macho_edit-lib
  PUBLIC
    cxx_std_11)

add_executable(macho_edit)
target_sources(macho_edit
  PRIVATE
    main.cpp
    menu.cpp
  PRIVATE
    FILE_SET HEADERS FILES
      menu.h)
target_link_libraries(macho_edit
  PRIVATE
    macho_edit-lib)
