find_path(CONV_CORE_INCLUDE_DIR "converse.h")
find_library(CONV_CORE_LIBRARY "libconv-core.a" PATHS "/lib" )

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(CONV_CORE DEFAULT_MSG
    CONV_CORE_INCLUDE_DIR
    CONV_CORE_LIBRARY
)

add_library(CONV::conv_core STATIC IMPORTED)
set_target_properties(CONV::conv_core PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${CONV_CORE_INCLUDE_DIR}
    IMPORTED_LOCATION ${CONV_CORE_LIBRARY}
)

find_path(CONV_LDB_INCLUDE_DIR "topology.h" PATHS "/include" )
find_library(CONV_LDB_LIBRARY "libconv-ldb.a" PATHS "/lib" )

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(CONV_LDB DEFAULT_MSG
	CONV_LDB_INCLUDE_DIR 
	CONV_LDB_LIBRARY
)

add_library(CONV::conv_ldb STATIC IMPORTED)
set_target_properties(CONV::conv_ldb PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${CONV_LDB_INCLUDE_DIR}
    IMPORTED_LOCATION ${CONV_LDB_LIBRARY}
)