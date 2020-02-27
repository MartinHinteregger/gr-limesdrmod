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
#include "checksum_crc_append_bb_impl.h"

namespace gr {
  namespace limesdrmod {

    checksum_crc_append_bb::sptr
    checksum_crc_append_bb::make(int nr_bytes, std::string tag_key, int tag_length)
    {
      return gnuradio::get_initial_sptr
        (new checksum_crc_append_bb_impl(nr_bytes, tag_key, tag_length));
    }

    /*
     * The private constructor
     */
    checksum_crc_append_bb_impl::checksum_crc_append_bb_impl(
    		int nr_bytes, std::string tag_key, int tag_length)
      : gr::block("checksum_crc_append_bb",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(1, 1, sizeof(char)))
    {
        nr_bytes_ = nr_bytes;
        tag_key_ = tag_key;
        tag_length_ = tag_length;
        multiple_ = tag_length_ + nr_bytes_;

        set_tag_propagation_policy(TPP_DONT);
        set_output_multiple(multiple_);
        //set_alignment(multiple_);
    }

    /*
     * Our virtual destructor.
     */
    checksum_crc_append_bb_impl::~checksum_crc_append_bb_impl()
    {
    }

    void checksum_crc_append_bb_impl::forecast(int noutput_items,
    		gr_vector_int &ninput_items_required)
    {
    	int nr_of_lengths = noutput_items / multiple_;
    	ninput_items_required[0] = tag_length_ * nr_of_lengths;
    }

    int
    checksum_crc_append_bb_impl::general_work(int noutput_items,
            gr_vector_int &ninput_items,
            gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items)
    {
    	const unsigned char *in = (const unsigned char*) input_items[0];
    	unsigned char *out = (unsigned char *) output_items[0];

    	// ===========================================================
    	uint64_t checksum = 0;
    	tag_t new_tag;
    	int in_position = 0;
    	int out_position = 0;

    	while ((in_position + tag_length_) <= ninput_items[0]
				&& (out_position + tag_length_) <= noutput_items)
    	{
    		for (int i = 0; i < tag_length_; i++)
    			out[out_position + i] = in[in_position + i];

    		out_position += tag_length_;
    		// Process bytes and calc checksum
    		if (nr_bytes_ == 1)
    		{
    			crc8_.reset();
    			for (int i = 0; i < tag_length_; i++)
    				crc8_.process_byte(in[in_position + i]);

    			checksum = crc8_.checksum();
    			out[out_position + 0] = checksum & 0xFF;
    		}
    		else if (nr_bytes_ == 2)
    		{
    			crc16_.reset();
    			for (int i = 0; i < tag_length_; i++)
    				crc16_.process_byte(in[in_position + i]);

    			checksum = crc16_.checksum();
    			out[out_position + 0] = (checksum >> 8) & 0xFF;
    			out[out_position + 1] = checksum & 0xFF;
    		}
    		else if (nr_bytes_ == 3)
    		{
    			crc24_.reset();
    			for (int i = 0; i < tag_length_; i++)
    				crc24_.process_byte(in[in_position + i]);

    			checksum = crc24_.checksum();
    			out[out_position + 0] = (checksum >> 16) & 0xFF;
    			out[out_position + 1] = (checksum >> 8) & 0xFF;
    			out[out_position + 2] = checksum & 0xFF;
    		}
    		else if (nr_bytes_ == 4)
    		{
    			crc32_.reset();
    			for (int i = 0; i < tag_length_; i++)
    				crc32_.process_byte(in[in_position + i]);

    			checksum = crc32_.checksum();
    			out[out_position + 0] = (checksum >> 24) & 0xFF;
    			out[out_position + 1] = (checksum >> 16) & 0xFF;
    			out[out_position + 2] = (checksum >> 8) & 0xFF;
    			out[out_position + 3] = checksum & 0xFF;
    		}
    		out_position += nr_bytes_;
    		in_position += tag_length_;

    		new_tag.key = pmt::string_to_symbol(tag_key_);
    		new_tag.offset = out_position - multiple_ + nitems_written(0);
    		new_tag.value = pmt::mp(tag_length_ + nr_bytes_);
    		add_item_tag(0, new_tag);
    	}

    	if (out_position > noutput_items)
    		std::cout << "checksum_crc_append_bb_impl Warning: Output buffer is too short,"
    				" might result in data loss/shift! This block should only"
    				" get the data to add crc, other data & tags might be disruptive. "
					<< ninput_items[0] << "/" << out_position << "/" << noutput_items
					<< std::endl;

    	consume(0, in_position);
    	produce(0, out_position);
    	return WORK_CALLED_PRODUCE;
    }
  } /* namespace limesdrmod */
} /* namespace gr */

