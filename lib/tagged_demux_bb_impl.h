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

#ifndef INCLUDED_LIMESDRMOD_TAGGED_DEMUX_BB_IMPL_H
#define INCLUDED_LIMESDRMOD_TAGGED_DEMUX_BB_IMPL_H

#include <limesdrmod/tagged_demux_bb.h>

namespace gr {
  namespace limesdrmod {

    class tagged_demux_bb_impl : public tagged_demux_bb
    {
     private:
        std::string tag_out0_;
        std::string tag_out1_;

        int next_tag_ = 0;
        bool fixed_rate_ = false;

        uint64_t identifier_ = 1;
        std::string tag_identifier_;

        int buffer_multiplier_ = 1;

        //uint64_t identifier_header_ = 1;
        //uint64_t identifier_data_ = 1;

     public:
      tagged_demux_bb_impl(std::string tag_out0, std::string tag_out1,
    		  std::string tag_identifier);
      ~tagged_demux_bb_impl();

      int fixed_rate_ninput_to_noutput(int ninput);

      int general_work(int noutput_items,
    		  gr_vector_int &ninput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items);
    };

  } // namespace limesdrmod
} // namespace gr

#endif /* INCLUDED_LIMESDRMOD_TAGGED_DEMUX_BB_IMPL_H */

