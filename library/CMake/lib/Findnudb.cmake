message (STATUS "Searching for NuDB...")

set (nudb_SOURCE $ENV{nudb_SOURCE} CACHE PATH "NuDB path")
fixPath (nudb_SOURCE)
set (nudb_SEARCH_PATHS ${nudb_SOURCE} ${RADJAV_DEPENDENCIES})

searchForHeader (nudb nudb/nudb.hpp "${nudb_SEARCH_PATHS}" FALSE)

hasHeaderBeenFound (nudb)
