include_directories (../ext/midifile/include ../ext/rtmidi/include)

find_package (wxWidgets REQUIRED core base)
include (${wxWidgets_USE_FILE})

set (SRCS main.cpp pianoroll_widget.cpp midi_player.cpp)
 
if (WIN32)
	add_executable (cms WIN32 ${SRCS})
else ()
	add_executable (cms ${SRCS})
endif ()

add_dependencies (cms midifile rtmidi)

target_link_libraries (cms midifile rtmidi)
target_link_libraries (cms ${wxWidgets_LIBRARIES})
