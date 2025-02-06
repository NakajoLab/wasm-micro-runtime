# FindGMP.cmake

# GMP のヘッダを探す
find_path(GMP_INCLUDE_DIR NAMES gmp.h
  HINTS /usr/include /usr/local/include
)

# GMP ライブラリを探す
find_library(GMP_LIBRARY NAMES gmp
  HINTS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu
)

if (GMP_INCLUDE_DIR AND GMP_LIBRARY)
  set(GMP_FOUND TRUE)
  set(GMP_LIBRARIES ${GMP_LIBRARY})
else()
  set(GMP_FOUND FALSE)
endif()

mark_as_advanced(GMP_INCLUDE_DIR GMP_LIBRARY)
