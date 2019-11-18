include_directories (../ext/midifile/include ../ext/rtmidi/include)

find_package (wxWidgets REQUIRED core base)
include (${wxWidgets_USE_FILE})

add_executable (cms WIN32 main.cpp pianoroll_widget.cpp midi_player.cpp)

add_dependencies (cms midifile rtmidi)

target_link_libraries (cms midifile rtmidi)
target_link_libraries (cms ${wxWidgets_LIBRARIES})
