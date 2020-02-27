/* -*- c++ -*- */
/* 
 * Copyright 2019 <+YOU OR YOUR COMPANY+>.
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
#include "counter_source_impl.h"

namespace gr {
  namespace limesdrmod {

    counter_source::sptr
    counter_source::make(int increment, int minimum, int maximum)
    {
      return gnuradio::get_initial_sptr
        (new counter_source_impl(increment, minimum, maximum));
    }

    /*
     * The private constructor
     */
    counter_source_impl::counter_source_impl(int increment, int minimum, int maximum)
      : gr::sync_block("counter_source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(char)))
    {
        min_ = minimum;
        current_value_ = minimum;
        max_ = maximum;
        inc_ = increment;
    }

    /*
     * Our virtual destructor.
     */
    counter_source_impl::~counter_source_impl()
    {
    }

    int
    counter_source_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        char *out = (char *) output_items[0];

        for (int i = 0; i < noutput_items; i++)
        {
        	out[i] = current_value_;

        	if (current_value_ + inc_ > max_)
        		current_value_ = min_;
        	else
        		current_value_ += inc_;
        }

        return noutput_items;
    }

  } /* namespace limesdrmod */
} /* namespace gr */

