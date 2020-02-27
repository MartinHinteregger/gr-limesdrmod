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

#ifndef INCLUDED_LIMESDRMOD_TAGGED_MUX_BB_IMPL_H
#define INCLUDED_LIMESDRMOD_TAGGED_MUX_BB_IMPL_H

#include <limesdrmod/tagged_mux_bb.h>

namespace gr {
  namespace limesdrmod {

    class tagged_mux_bb_impl : public tagged_mux_bb
    {
     private:
        std::string tag_in0_;
        std::string tag_in1_;

        std::vector<int> guard_;
        int guard_len_;

        bool next_tag_ = false;
        bool set_fixed_rate_ = false;

     public:
      tagged_mux_bb_impl(std::string tag_in0, std::string tag_in1,
    		  const std::vector<int> &guard);
      ~tagged_mux_bb_impl();

      int general_work(int noutput_items,
    		  gr_vector_int &ninput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items);
    };

  } // namespace limesdrmod
} // namespace gr

#endif /* INCLUDED_LIMESDRMOD_TAGGED_MUX_BB_IMPL_H */

