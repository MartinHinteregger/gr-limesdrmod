#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2018 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

import numpy
from gnuradio import gr
import time

class delayed_blocker_bb(gr.sync_block):
    """
    docstring for block delayed_blocker_bb
    """
    def __init__(self, delay_ms):
        self.delay_ms = delay_ms
        self.millis = int(round(time.time() * 1000))
        gr.sync_block.__init__(self,
            name="delayed_blocker_bb",
            in_sig=[numpy.byte],
            out_sig=[numpy.byte])


    def work(self, input_items, output_items):
        in0 = input_items[0]
        out = output_items[0]

        current = int(round(time.time() * 1000))
        if (current < (self.millis + self.delay_ms)):
            return 0
        
        out[:] = in0
        return len(output_items[0])

