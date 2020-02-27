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

#ifndef INCLUDED_LIMESDRMOD_CHECKSUM_CRC_APPEND_BB_IMPL_H
#define INCLUDED_LIMESDRMOD_CHECKSUM_CRC_APPEND_BB_IMPL_H

#include <limesdrmod/checksum_crc_append_bb.h>
#include <boost/crc.hpp>

namespace gr {
  namespace limesdrmod {

    class checksum_crc_append_bb_impl : public checksum_crc_append_bb
    {
     private:
      int nr_bytes_;
      std::string tag_key_;
      int tag_length_;
      int multiple_;

      // Need to do this this way, even though it is ugly
      boost::crc_optimal<8,  0x24, 0, 0, true, true> crc8_;
      boost::crc_optimal<16, 0x1024, 0, 0, true, true> crc16_;
      boost::crc_optimal<24, 0x241024, 0, 0, true, true> crc24_;
      boost::crc_optimal<32, 0x10241024, 0, 0, true, true> crc32_;

     public:
      checksum_crc_append_bb_impl(int nr_bytes, std::string tag_key, int tag_length);
      ~checksum_crc_append_bb_impl();

      void forecast(int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
    		  gr_vector_int &ninput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items);
    };

  } // namespace limesdrmod
} // namespace gr

#endif /* INCLUDED_LIMESDRMOD_CHECKSUM_CRC_APPEND_BB_IMPL_H */

