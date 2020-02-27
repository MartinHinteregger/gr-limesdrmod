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
install(FILES
    limesdrmod_sinkmod.xml
    limesdrmod_sourcemod.xml
    limesdrmod_bitshifter.xml
    limesdrmod_variable_source.xml
    limesdrmod_correlate_and_sync_bb.xml
    limesdrmod_timed_blocker_bb.xml
    limesdrmod_checksum_crc_append_bb.xml
    limesdrmod_tagged_mux_bb.xml
    limesdrmod_delayed_blocker_bb.xml
    limesdrmod_tagged_demux_bb.xml
    limesdrmod_checksum_crc_header_checker_bb.xml
    limesdrmod_data_rate_bb.xml
    limesdrmod_counter_source.xml DESTINATION share/gnuradio/grc/blocks
)
