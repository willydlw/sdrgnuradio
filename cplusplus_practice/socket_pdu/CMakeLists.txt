# Copyright 2011 Free Software Foundation, Inc.
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


find_package(Boost COMPONENTS system filesystem REQUIRED)

include_directories(
	${GR_BLOCKS_INCLUDE_DIRS}
	${GNURADIO_RUNTIME_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
)

message("boost lib: ${Boost_LIBRARIES}")
message("boost inc: ${Boost_INCLUDE_DIRS}")

add_executable(send_to_socket send_to_socketpdu.cc)
target_link_libraries(send_to_socket
  gnuradio-runtime gnuradio-blocks
  ${Boost_LIBRARIES})

INSTALL(TARGETS
    send_to_socket
    RUNTIME DESTINATION bin
    DESTINATION ${}
)