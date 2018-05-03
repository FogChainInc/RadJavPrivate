message (STATUS "Searching for RocksDB...")

if (NOT WIN32)
	set (RocksDB_SOURCE $ENV{RocksDB_SOURCE} CACHE PATH "RocksDB path")
	fixPath (RocksDB_SOURCE)
	set (RocksDB_SEARCH_PATHS ${RocksDB_SOURCE} ${RADJAV_DEPENDENCIES})

	searchForHeader (RocksDB db.h "${RocksDB_SEARCH_PATHS}" FALSE)
	searchForLibrary (RocksDB rocksdb rocksdb "${RocksDB_SEARCH_PATHS}")

	hasLibBeenFound (RocksDB)
endif ()

