message (STATUS "Searching for RocksDB...")

set (rocksdb_SOURCE $ENV{rocksdb_SOURCE} CACHE PATH "rocksdb path")
fixPath (rocksdb_SOURCE)
set (rocksdb_SEARCH_PATHS ${rocksdb_SOURCE} ${RADJAV_DEPENDENCIES})

searchForHeader (rocksdb rocksdb/db.h "${rocksdb_SEARCH_PATHS}" FALSE)
searchForLibrary (rocksdb rocksdb rocksdb "${rocksdb_SEARCH_PATHS}")

hasLibBeenFound (rocksdb)

