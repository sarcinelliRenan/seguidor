include_directories(raspicam)
add_library(raspicam
	raspicam/raspicam.cpp 
	raspicam/raspicam_still.cpp 
	raspicam/private/private_impl.cpp 
	raspicam/private/threadcondition.cpp 
	raspicam/private_still/private_still_impl.cpp
)

include_directories(raspicam/dependencies raspicam/dependencies/mmal raspicam/dependencies/vcos)

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES arm*)
	MESSAGE(STATUS "Searching MMAL")
	FIND_LIBRARY(mmalcore_LIBS NAMES mmal_core PATHS /opt/vc/lib)
	FIND_LIBRARY(mmalutil_LIBS NAMES mmal_util PATHS /opt/vc/lib)
	FIND_LIBRARY(mmal_LIBS NAMES mmal PATHS /opt/vc/lib)
	IF( (NOT mmal_LIBS ) OR (NOT mmalutil_LIBS) OR (NOT mmalcore_LIBS) )
    	MESSAGE(FATAL_ERROR "Could not find mmal libraries")
	ENDIF()
ENDIF()

TARGET_LINK_LIBRARIES(raspicam ${mmalcore_LIBS} ${mmalutil_LIBS} ${mmal_LIBS})
