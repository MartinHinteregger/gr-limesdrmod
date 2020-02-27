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
#include "variable_source_impl.h"

namespace gr {
  namespace limesdrmod {

  	variable_source::sptr
	variable_source::make(int value)
    {
      return gnuradio::get_initial_sptr
        (new variable_source_impl(value));
    }

    /*
     * The private constructor
     */
  	variable_source_impl::variable_source_impl(int value)
      : gr::sync_block("variable_source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(int)))
    {
    	val_ = value;
    }

    /*
     * Our virtual destructor.
     */
  	variable_source_impl::~variable_source_impl()
    {
    }

    int
	variable_source_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      int *out = (int *) output_items[0];

      out[0] = val_;

      return 1;
    }


    void variable_source_impl::set_value(int value)
    {
    	val_ = value;
    }

  } /* namespace limesdrmod */
} /* namespace gr */

