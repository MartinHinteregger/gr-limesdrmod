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
#include "correlate_and_sync_bb_impl.h"

namespace gr {
  namespace limesdrmod {

    correlate_and_sync_bb::sptr
    correlate_and_sync_bb::make(const std::vector<int> &sync,
  		  int packet_len, const char* length_tag_name,
		  const char* header_tag_name, int header_len, int checksum_len)
    {
      return gnuradio::get_initial_sptr
        (new correlate_and_sync_bb_impl(sync,
        		packet_len, length_tag_name,
				header_tag_name, header_len, checksum_len));
    }

    /*
     * The private constructor
     */
    correlate_and_sync_bb_impl::correlate_and_sync_bb_impl(
    		const std::vector<int> &sync,
			int packet_len, const char* length_tag_name,
			const char* header_tag_name, int header_len, int checksum_len)
      : gr::block("correlate_and_sync_bb",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(1, 1, sizeof(char)))
    {
    	this->sync_ = sync;
    	this->packet_len_ = packet_len;
    	this->length_tag_name_ = length_tag_name;
    	this->header_tag_name_ = header_tag_name;
    	this->header_len_ = header_len;
    	this->checksum_len_ = checksum_len;

    	// Parse the syncword into the long member variable
    	this->syncword_len_bits_ = parseSyncword();
    	if (this->syncword_len_bits_ == -1)
    	{
    		std::cout << "correlate_and_pass Warning: Syncword was to long, clipping at: " << sizeof(long)*8 << std::endl;
    		this->syncword_len_bits_ = sizeof(long)*8;
    	}
    	if (this->syncword_len_bits_ < 8)
    		std::cout << "correlate_and_pass Warning: Syncword is very short, this might cause lag." << std::endl;

    	if (this->syncword_len_bits_ % 8)
    		std::cout << "correlate_and_pass Warning: Syncword is not modulo 8, might lead to inaccurate correlation." << std::endl;

    	//syncword_len_bytes_ = syncword_len_bits_ / 8;
    	bitshift_ = 0;
    	set_output_multiple(packet_len + header_len + checksum_len);
    }

    /*
     * Our virtual destructor.
     */
    correlate_and_sync_bb_impl::~correlate_and_sync_bb_impl()
    {
    }

    // Creates the syncword in long format
    // Returns the length (= number of bits) of the syncword
    int correlate_and_sync_bb_impl::parseSyncword()
    {
    	std::vector<int>::iterator it = sync_.begin();

    	syncword_ = 0;

    	for (int len = 0; len < sizeof(long)*8; len++)
    	{
			length_mask_ |= 1;
			if ((*it) != 0)
				syncword_ |= 1;

			if (it == sync_.end())
			{
				syncword_ = syncword_ >> 1;
				length_mask_ = length_mask_ >> 1;
				return len;
			}

			length_mask_ = length_mask_ << 1;
			syncword_ = syncword_ << 1;
			it++;
    	}

    	// Syncword was too long
    	return -1;
    }

    int
    correlate_and_sync_bb_impl::general_work(int noutput_items,
            gr_vector_int &ninput_items,
            gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items)
    {
    	const unsigned char *in = (const unsigned char*) input_items[0];
    	unsigned char *out = (unsigned char *) output_items[0];

		// still some bits to process from prev. call of general_work?
    	if (bytes_to_process_)
    	{
    		int out_position = 0;
    		int at_sample = 0;
    		while ((bytes_to_process_ > 0)
    				&& at_sample < ninput_items[0])
    		{
    			out[out_position] = (in[at_sample] << (bitshift_ + 1));
    			out[out_position] |= (in[at_sample+1] >> (7 - bitshift_));

//    			if (!at_sample)
//    				std::cout << out[out_position] << ". " << std::endl;

    			bytes_to_process_--;
    			out_position++;
    			at_sample++;
    		}

    		consume(0, at_sample-1);
    		produce(0, out_position);
    		return WORK_CALLED_PRODUCE;
    	}

    	bitshift_ = 0;
		// Shift mask by 1
		inp_mask_ = inp_mask_ << 1;
		// Clip to the length of the syncword
		inp_mask_ = inp_mask_ & length_mask_;
		// Add the new bit
		inp_mask_ |= (in[0] >> 7) & 1;


    	// For every input item
    	for (int at_sample = 0; at_sample < ninput_items[0];)
    	{
    		// CORRELATE
    		if (~(inp_mask_ ^ ~syncword_) == 0)
    		{
    			// FOUND the syncword
    			int out_position = 0;

    			// Add header tag
    			add_item_tag(0, nitems_written(0),
    				pmt::mp(header_tag_name_), pmt::mp(header_len_+checksum_len_));

    			// Pass the header and the checksum to the output
    			for (int i = 0; i < (header_len_+checksum_len_); i++)
    			{
    				out[out_position] = (in[at_sample-header_len_] << (bitshift_ + 1));
    				out[out_position] |= (in[at_sample-header_len_+1] >> (7 - bitshift_));

    				out_position++;
    				at_sample++;
    			}

    			// Calculate the number of bytes to process
    			bytes_to_process_ = packet_len_;

    			// Bitshift and pass the input to the output
    			while (bytes_to_process_ > 0
					&& at_sample < ninput_items[0])
    			{
    				out[out_position] = (in[at_sample-header_len_] << (bitshift_ + 1));
    				out[out_position] |= (in[at_sample-header_len_+1] >> (7 - bitshift_));

    				bytes_to_process_--;
    				out_position++;
    				at_sample++;
    			}

    			// All work done, add the packet tag
    			add_item_tag(0, nitems_written(0) + header_len_ + checksum_len_,
					pmt::mp(length_tag_name_), pmt::mp(packet_len_));

    			// Shift the remaining bits into the mask
//    			for (int i = bitshift_; i < 7; ++i)
//    			{
//    	    		// Shift mask by 1
//    	    		inp_mask_ = inp_mask_ << 1;
//    	    		// Clip to the length of the syncword
//    	    		inp_mask_ = inp_mask_ & length_mask_;
//    	    		inp_mask_ |= (in[at_sample-header_len_+1] >> (7 - bitshift_)) & 1;
//    			}

//    			if (bytes_to_process_)
//    			{
//    				std::cout << "by: " << bytes_to_process_ << ", " << at_sample << "/"
//    						<< ninput_items[0] << ", l: " << out[out_position-1] << ", shft: "
//							<< bitshift_ << "| ";
//    			}

    			// Next general_work call
    			consume(0, at_sample-1);
    			produce(0, out_position+1);
    			return WORK_CALLED_PRODUCE;
    		}

    		// Check bitshift
    		if (bitshift_ == 7)
    		{
    			bitshift_ = 0;
    			at_sample++;
    		}
    		else
    			bitshift_++;

    		// Shift mask by 1
    		inp_mask_ = inp_mask_ << 1;
    		// Clip to the length of the syncword
    		inp_mask_ = inp_mask_ & length_mask_;
    		// Add the new bit
    		inp_mask_ |= (in[at_sample] >> (7 - bitshift_)) & 1;
    	}

		// Did not find the syncword, get next set of inputs.
    	consume(0, ninput_items[0]);
    	return WORK_CALLED_PRODUCE;
    }

  } /* namespace limesdrmod */
} /* namespace gr */

