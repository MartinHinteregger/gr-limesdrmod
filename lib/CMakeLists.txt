# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR} 
		    ${LIMESUITE_INCLUDE_DIRS} 
		    ${CMAKE_CURRENT_BINARY_DIR})
link_directories(${Boost_LIBRARY_DIRS} 
		 ${LIMESDR_PKG_LIBRARY_DIRS})
list(APPEND limesdrmod_sources
    sinkmod_impl.cc
    sourcemod_impl.cc
    common/device_handler.cc
    common/lms7_customAPI.cc
    bitshifter_impl.cc
    variable_source_impl.cc
    correlate_and_sync_bb_impl.cc
    checksum_crc_append_bb_impl.cc
    tagged_mux_bb_impl.cc
    tagged_demux_bb_impl.cc
    checksum_crc_header_checker_bb_impl.cc
    data_rate_bb_impl.cc
    counter_source_impl.cc
)

set(limesdrmod_sources "${limesdrmod_sources}" PARENT_SCOPE)
if(NOT limesdrmod_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT limesdrmod_sources)

add_library(gnuradio-limesdrmod SHARED ${limesdrmod_sources})
target_link_libraries(
	gnuradio-limesdrmod 
	${Boost_LIBRARIES} 
	${GNURADIO_ALL_LIBRARIES} 
  	${LIMESUITE_LIB})
  	
set_target_properties(
	gnuradio-limesdrmod PROPERTIES DEFINE_SYMBOL "gnuradio_limesdrmod_EXPORTS")

if(APPLE)
    set_target_properties(
    	gnuradio-limesdrmod PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-limesdrmod
	LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
	ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
	RUNTIME DESTINATION bin              # .dll file
)

