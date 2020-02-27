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
#include "checksum_crc_header_checker_bb_impl.h"

namespace gr {
  namespace limesdrmod {

    checksum_crc_header_checker_bb::sptr
    checksum_crc_header_checker_bb::make(std::string tag_key, int tag_length,
    		int crc_bytes, std::string identifier_key)
    {
      return gnuradio::get_initial_sptr
        (new checksum_crc_header_checker_bb_impl(tag_key, tag_length,
        		crc_bytes, identifier_key));
    }

    /*
     * The private constructor
     */
    checksum_crc_header_checker_bb_impl::checksum_crc_header_checker_bb_impl(
    		std::string tag_key, int tag_length, int crc_bytes, std::string identifier_key)
      : gr::block("checksum_crc_header_checker_bb",
              gr::io_signature::make(2, 2, sizeof(char)),
              gr::io_signature::make(2, 2, sizeof(char)))
    {
    	tag_key_ = tag_key;
    	tag_length_ = tag_length;
    	crc_bytes_ = crc_bytes;
    	identifier_key_ = identifier_key;

    	set_tag_propagation_policy(TPP_DONT);
    }

    /*
     * Our virtual destructor.
     */
    checksum_crc_header_checker_bb_impl::~checksum_crc_header_checker_bb_impl()
    {
    }

    int
    checksum_crc_header_checker_bb_impl::general_work(int noutput_items,
  		  gr_vector_int &ninput_items,
            gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items)
    {
    	const unsigned char *in0 = (const unsigned char*) input_items[0];
    	const unsigned char *in1 = (const unsigned char*) input_items[1];
    	unsigned char *out0 = (unsigned char *) output_items[0];
    	unsigned char *out1 = (unsigned char *) output_items[1];

    	// Get all tags
    	std::vector <tag_t> tags0;
    	get_tags_in_range(tags0, 0, nitems_read(0), nitems_read(0)+ninput_items[0]);
    	int nr_tags0 = tags0.size();
    	int tag_index0 = 0;
    	int in_index0 = 0;
    	bool in0_second_tag_is_identifier = false;
    	uint64_t in0_identifier_value = 0;

    	std::vector <tag_t> tags1;
    	get_tags_in_range(tags1, 1, nitems_read(1), nitems_read(1)+ninput_items[1]);
    	int nr_tags1 = tags1.size();
    	int tag_index1 = 0;
    	int in_index1 = 0;
    	bool in1_second_tag_is_identifier = false;
    	uint64_t in1_identifier_value = 0;
    	uint64_t in1_data_value = 0;

    	int out0_position = 0;
    	int out1_position = 0;

    	uint64_t checksum_input;
    	uint64_t checksum_calc;

    	if (nr_tags0 == 0)
    	{
    		std::cout << "checksum_crc_header_checker Warning: No tags on input0: "
    				<< ninput_items[0] << std::endl;

    		consume(0, ninput_items[0]);
    		return 0;
    	}
    	if (nr_tags1 == 0)
    	{
    		std::cout << "checksum_crc_header_checker Warning: No tags on input1: "
    				<< ninput_items[1] << std::endl;
    		consume(1, ninput_items[1]);
    		return 0;
    	}
    	if (nr_tags0 % 2)
    	{
    		std::cout << "checksum_crc_header_checker Warning: Number of tags @ input0 not modulo 2: "
    				<< nr_tags0 << std::endl;
    	}

    	if (nr_tags1 % 2)
    	{
    		std::cout << "checksum_crc_header_checker Warning: Number of tags @ input1 not modulo 2: "
    				<< nr_tags1 << std::endl;
    	}

    	for (tag_index0 = 0; tag_index0 < nr_tags0; tag_index0 += 2)
    	{
    		// ==========================================
    		// Check if first tag0 tag is identifier
    		if (!strcmp(pmt::symbol_to_string(tags0[tag_index0].key).c_str(),
    				identifier_key_.c_str()))
    			in0_second_tag_is_identifier = false;
    		// Check if second tag0 tag is identifier
    		else if (!strcmp(pmt::symbol_to_string(tags0[tag_index0+1].key).c_str(),
    				identifier_key_.c_str()))
    			in0_second_tag_is_identifier = true;
    		else
    		{
    			// tag0 identifier not found
        		std::cout << "checksum_crc_header_checker Warning: Identifier @ input0 not found."
        				<< std::endl;
        		continue;
    		}

    		// ==========================================
    		// Check offset, values and overflow
    		if (tags0[tag_index0].offset != tags0[tag_index0+1].offset)
    		{
    			std::cout << "checksum_crc_header_checker Warning: Tags @ input0 have different offset."
    					<< std::endl;
    			continue;
    		}
    		if (!pmt::to_uint64(tags0[tag_index0].value) ||
    				!pmt::to_uint64(tags0[tag_index0+1].value))
    			throw(std::runtime_error("checksum_crc_header_checker Error: At least one tag @ input0 has no value."));
    		//if (overflow)

    		// ==========================================
    		// Check CRC data
    		in_index0 = tags0[tag_index0].offset - nitems_read(0);
    		switch (crc_bytes_)
    		{
    		case 1:
    			crc8_.reset();
    			for (int i = 0; i < (tag_length_); i++)
    				crc8_.process_byte(in0[in_index0 + i]);

    			checksum_calc = crc8_.checksum();
    			checksum_input = in0[in_index0 + tag_length_];
    			break;
    		case 2:
    			crc16_.reset();
    			for (int i = 0; i < (tag_length_); i++)
    				crc16_.process_byte(in0[in_index0 + i]);

    			checksum_calc = crc16_.checksum();
    			checksum_input = in0[in_index0 + tag_length_];
    			//checksum_input << 8;
    			checksum_input = (checksum_input << 8) | in0[in_index0 + tag_length_ + 1];
    			break;
    		case 3:
    			crc24_.reset();
    			for (int i = 0; i < (tag_length_); i++)
    				crc24_.process_byte(in0[in_index0 + i]);

    			checksum_calc = crc24_.checksum();
    			checksum_input = in0[in_index0 + tag_length_];
//    			checksum_input << 8;
    			checksum_input = (checksum_input << 8) | in0[in_index0 + tag_length_ + 1];
//    			checksum_input << 8;
    			checksum_input = (checksum_input << 8) | in0[in_index0 + tag_length_ + 2];
    			break;
    		case 4:
    			crc32_.reset();
    			for (int i = 0; i < (tag_length_); i++)
    				crc32_.process_byte(in0[in_index0 + i]);

    			checksum_calc = crc32_.checksum();
    			checksum_input = in0[in_index0 + tag_length_];
//    			checksum_input << 8;
    			checksum_input = (checksum_input << 8) | in0[in_index0 + tag_length_ + 1];
//    			checksum_input << 8;
    			checksum_input = (checksum_input << 8) | in0[in_index0 + tag_length_ + 2];
//    			checksum_input << 8;
    			checksum_input = (checksum_input << 8) | in0[in_index0 + tag_length_ + 3];
    			break;
    		}
    		in_index0 += tag_length_ + crc_bytes_;

			in0_identifier_value =
					pmt::to_uint64(tags0[tag_index0 + (int)in0_second_tag_is_identifier].value);


    		if (~(checksum_calc ^ ~checksum_input) == 0)
    		{
    			for (tag_index1 = 0; tag_index1 < nr_tags1; tag_index1 += 2)
    			{
    				// ==========================================
    				// Check if first tag1 tag is identifier
    				if (!strcmp(pmt::symbol_to_string(tags1[tag_index1].key).c_str(),
    						identifier_key_.c_str()))
    					in1_second_tag_is_identifier = false;
    				// Check if second tag1 tag is identifier
    				else if (!strcmp(pmt::symbol_to_string(tags1[tag_index1+1].key).c_str(),
    						identifier_key_.c_str()))
    					in1_second_tag_is_identifier = true;
    				else
    				{
    					// tag1 identifier not found
    					std::cout << "checksum_crc_header_checker Warning: Identifier @ input1 not found."
    							<< std::endl;
    					continue;
    				}

    	    		// ==========================================
    	    		// Check offset, values and overflow
    	    		if (tags1[tag_index1].offset != tags1[tag_index1+1].offset)
    	    		{
    	    			std::cout << "checksum_crc_header_checker Warning: Tags @ input1 have different offset."
    	    					<< std::endl;
    	    			continue;
    	    		}
    				if (!pmt::to_uint64(tags1[tag_index1].value) ||
    						!pmt::to_uint64(tags1[tag_index1+1].value))
    	    			throw(std::runtime_error("checksum_crc_header_checker Error: At least one tag @ input1 has no value."));
    				// overflow

    				in1_identifier_value = pmt::to_uint64(tags1[tag_index1 +
						(int)in1_second_tag_is_identifier].value);

    				if (in0_identifier_value < in1_identifier_value)
    				{
    					std::cout << "checksum_crc_header_checker Warning: Overflow @ input1!"
    							<< std::endl;
    					break;
    				}
    				if (in0_identifier_value > in1_identifier_value)
    					continue;
    				if (in0_identifier_value == in1_identifier_value)
    				{
    					// Found!
    					in1_data_value = pmt::to_uint64(tags1[tag_index1 +
							(int)!in1_second_tag_is_identifier].value);
    					in_index1 = tags1[tag_index1 + (int)!in1_second_tag_is_identifier].offset
    							- nitems_read(1);

    					for (int i = 0; i < in1_data_value; i++)
    						out0[out0_position + i] = in1[in_index1 + i];

    					out0_position += in1_data_value;
    					in_index1 += in1_data_value;
    					in1_packet_found_ = true;
    					break;
    				}
    			}
    			if (!in1_packet_found_)
    			{
    				in_index0 -= tag_length_ + crc_bytes_;
    			}
    			in1_packet_found_ = false;
    		}
    		else
    		{
    			// For now just throw a warning
//    			std::cout << "checksum_crc_header_checker Warning: crc Checksum was incorrect, Packet: "
//    					<< in0_identifier_value << std::endl;
    		}



    	}

    	consume(0, in_index0);
    	consume(1, in_index1);
    	produce(0, out0_position);
    	produce(1, 0);
    	return WORK_CALLED_PRODUCE;
    }

  } /* namespace limesdrmod */
} /* namespace gr */

