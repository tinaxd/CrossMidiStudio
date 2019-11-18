include_directories (../ext/midifile/include ../ext/rtmidi/include)

find_package (wxWidgets REQUIRED core base)
include (${wxWidgets_USE_FILE})
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")

add_executable (cms main.cpp pianoroll_widget.cpp midi_player.cpp)

add_dependencies (cms midifile rtmidi)

target_link_libraries (cms midifile rtmidi)
target_link_libraries (cms ${wxWidgets_LIBRARIES})
