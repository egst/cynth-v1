
set( CYNTH_DIR_SYSTEM ${PROJECT_SOURCE_DIR}/bin )
set( CYNTH_DIR_3RDPARTY ${PROJECT_SOURCE_DIR}/thirdparty )

find_package(Python3 COMPONENTS Interpreter Development)
if(NOT Python3_FOUND)
	message(FATAL_ERROR "Python 3 has not been found")
endif()
