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

#ifndef INCLUDED_LIMESDRMOD_CORRELATE_AND_SYNC_BB_IMPL_H
#define INCLUDED_LIMESDRMOD_CORRELATE_AND_SYNC_BB_IMPL_H

#include <limesdrmod/correlate_and_sync_bb.h>
#include <boost/crc.hpp>

namespace gr {
  namespace limesdrmod {

    class correlate_and_sync_bb_impl : public correlate_and_sync_bb
    {
     private:
    	// The binary sync word
    	std::vector<int> sync_;

    	// Packet length and tag
    	// lengths are in bytes.
    	int packet_len_ = 50;
    	std::string length_tag_name_;

    	// Header data
    	// lengths are in bytes.
    	std::string header_tag_name_;
    	int header_len_ = 1;
    	int checksum_len_ = 1;

    	// The syncword represented as long format
    	unsigned long syncword_ = 0;
    	// length mask, basically all 1s times number of bits
    	unsigned long length_mask_ = 0;
    	int syncword_len_bits_ = 0;

    	bool first_run_ = true;
    	unsigned long inp_mask_ = 0;

    	// The bitshift to sync the chars
    	int bitshift_ = 0;
    	// remaining bytes to process if packet overflows to next general_work call
    	int bytes_to_process_ = 0;

     public:
      correlate_and_sync_bb_impl(const std::vector<int> &sync,
    		  int packet_len, const char* length_tag_name,
			  const char* header_tag_name, int header_len, int checksum_len);
      ~correlate_and_sync_bb_impl();

      int parseSyncword();

      int general_work(int noutput_items,
              gr_vector_int &ninput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items);
    };

  } // namespace limesdrmod
} // namespace gr

#endif /* INCLUDED_LIMESDRMOD_CORRELATE_AND_SYNC_BB_IMPL_H */

