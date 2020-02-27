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

#ifndef INCLUDED_LIMESDRMOD_DATA_RATE_BB_IMPL_H
#define INCLUDED_LIMESDRMOD_DATA_RATE_BB_IMPL_H

#include <limesdrmod/data_rate_bb.h>
#include <boost/timer.hpp>

namespace gr {
  namespace limesdrmod {

    class data_rate_bb_impl : public data_rate_bb
    {
     private:
      float avg_time_ = 2.0;

      char results_buffer_[4];
      int last_char_ = 0;

      boost::timer timer_;
      bool timer_overflow_ = false;

      uint64_t nr_correct_bits_ = 0;
      uint64_t nr_all_bits_ = 0;

     public:
      data_rate_bb_impl();
      ~data_rate_bb_impl();

      int general_work(int noutput_items,
    		  gr_vector_int &ninput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items);
    };

  } // namespace limesdrmod
} // namespace gr

#endif /* INCLUDED_LIMESDRMOD_DATA_RATE_BB_IMPL_H */

