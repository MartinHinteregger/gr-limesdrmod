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

#ifndef INCLUDED_LIMESDRMOD_CHECKSUM_CRC_HEADER_CHECKER_BB_IMPL_H
#define INCLUDED_LIMESDRMOD_CHECKSUM_CRC_HEADER_CHECKER_BB_IMPL_H

#include <limesdrmod/checksum_crc_header_checker_bb.h>
#include <boost/crc.hpp>

namespace gr {
  namespace limesdrmod {

    class checksum_crc_header_checker_bb_impl : public checksum_crc_header_checker_bb
    {
     private:
      std::string tag_key_;
      int tag_length_;
      int crc_bytes_;
      std::string identifier_key_;

      boost::crc_optimal<8,  0x24, 0, 0, true, true> crc8_;
      boost::crc_optimal<16, 0x1024, 0, 0, true, true> crc16_;
      boost::crc_optimal<24, 0x241024, 0, 0, true, true> crc24_;
      boost::crc_optimal<32, 0x10241024, 0, 0, true, true> crc32_;

      bool in1_packet_found_ = false;

//      uint64_t current_id_ = 0;
//      bool header_id_tag_found_ = false;
//      bool data_id_tag_first_ = false;
      //bool buffer_rate_set_ = false;

     public:
      checksum_crc_header_checker_bb_impl(std::string tag_key, int tag_length,
    		  int crc_bytes, std::string identifier_key);
      ~checksum_crc_header_checker_bb_impl();

      int general_work(int noutput_items,
    		  gr_vector_int &ninput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items);
    };

  } // namespace limesdrmod
} // namespace gr

#endif /* INCLUDED_LIMESDRMOD_CHECKSUM_CRC_HEADER_CHECKER_BB_IMPL_H */

