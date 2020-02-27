/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "bitshifter_impl.h"

namespace gr {
  namespace limesdrmod {

    bitshifter::sptr
    bitshifter::make(int num_bits)
    {
      return gnuradio::get_initial_sptr
        (new bitshifter_impl(num_bits));
    }

    /*
     * The private constructor
     */
    bitshifter_impl::bitshifter_impl(int num_bits)
      : gr::sync_block("bitshifter",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(1, 1, sizeof(char)))
    {
    	num_bits_ = num_bits;
    	old_value = 0;
    }

    /*
     * Our virtual destructor.
     */
    bitshifter_impl::~bitshifter_impl()
    {
    }

    int
    bitshifter_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
    	const char *in = (const char*) input_items[0];
    	char *out = (char *) output_items[0];

    	// Shift stream by number of bits
        for (int i = 0; i < noutput_items; i++)
        {
        	out[i] = 0;
        	out[i] = (old_value << (8-num_bits_));
        	old_value = in[i];
        	out[i] |= (old_value >> num_bits_);
        }

        return noutput_items;
    }

  } /* namespace limesdrmod */
} /* namespace gr */

