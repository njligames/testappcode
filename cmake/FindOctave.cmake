set(OCTAVE_FOUND TRUE)

find_path(OCTAVE_INCLUDE_DIR octave/oct.h
  HINTS
    /usr/local/Cellar/octave
  PATH_SUFFIXES
    5.1.0/include/octave-5.1.0
  )

find_library(OCTAVE_LIBRARIES
  NAMES
  octave octgui octinterp
  HINTS
    /usr/local/Cellar/octave
  PATH_SUFFIXES
    5.1.0/lib/octave/5.1.0
    )

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Octave
  REQUIRED_VARS OCTAVE_LIBRARIES OCTAVE_INCLUDE_DIR
  )

