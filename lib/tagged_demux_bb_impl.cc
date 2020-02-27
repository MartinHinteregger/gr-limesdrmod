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
#include "tagged_demux_bb_impl.h"

namespace gr {
  namespace limesdrmod {

    tagged_demux_bb::sptr
    tagged_demux_bb::make(std::string tag_out0, std::string tag_out1,
    		std::string tag_identifier)
    {
      return gnuradio::get_initial_sptr
        (new tagged_demux_bb_impl(tag_out0, tag_out1, tag_identifier));
    }

    /*
     * The private constructor
     */
    tagged_demux_bb_impl::tagged_demux_bb_impl(std::string tag_out0, std::string tag_out1,
    		std::string tag_identifier)
      : gr::block("tagged_demux_bb",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(2, 2, sizeof(char)))
    {
    	tag_out0_ = tag_out0;
    	tag_out1_ = tag_out1;
    	tag_identifier_ = tag_identifier;

        set_tag_propagation_policy(TPP_DONT);
        this->set_fixed_rate(true);
        //set_output_multiple(56);
    }

    /*
     * Our virtual destructor.
     */
    tagged_demux_bb_impl::~tagged_demux_bb_impl()
    {
    }

    int tagged_demux_bb_impl::fixed_rate_ninput_to_noutput(int ninput)
    {
    	if (ninput)
    		return buffer_multiplier_ * (ninput+2);
    	else
    		return buffer_multiplier_ * 3;
    }

    int tagged_demux_bb_impl::general_work(int noutput_items,
  		  gr_vector_int &ninput_items,
		  gr_vector_const_void_star &input_items,
		  gr_vector_void_star &output_items)
    {
    	const unsigned char *in = (const unsigned char*) input_items[0];
    	unsigned char *out0 = (unsigned char *) output_items[0];
    	unsigned char *out1 = (unsigned char *) output_items[1];

    	// Get all tags
    	std::vector <tag_t> tags;
    	get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0)+ninput_items[0]);
    	int nr_tags = tags.size();

    	// No tags in range
    	if (!nr_tags)
    	{
    		consume(0, ninput_items[0]);
    		return 0;
    	}

    	uint64_t tag0_value;
    	uint64_t tag1_value;
    	uint64_t tag0_offset;
    	uint64_t tag1_offset;
    	uint64_t in_position = 0;
    	int out0_position_ = 0;
    	int out1_position_ = 0;
    	tag_t identifier_tag;

    	identifier_tag.key = pmt::string_to_symbol(tag_identifier_);

		// First run: Set the output buffer rate
		if (!fixed_rate_)
		{
			if ((nr_tags < 2) ||
					(strcmp(pmt::symbol_to_string(tags[0].key).c_str(), tag_out0_.c_str()) &&
					 strcmp(pmt::symbol_to_string(tags[1].key).c_str(), tag_out0_.c_str())) ||
					(strcmp(pmt::symbol_to_string(tags[0].key).c_str(), tag_out1_.c_str()) &&
					 strcmp(pmt::symbol_to_string(tags[1].key).c_str(), tag_out1_.c_str())) ||
					 !pmt::to_uint64(tags[0].value) || !pmt::to_uint64(tags[1].value))
			{
				throw(std::runtime_error("tagged_demux_bb Error: First run setting output buffer size failed."));
			}
			buffer_multiplier_ = pmt::to_uint64(tags[0].value) + pmt::to_uint64(tags[1].value);
			set_output_multiple(buffer_multiplier_ * 3);
			fixed_rate_ = true;
			return 0;
		}

    	for (int tag_id = 0; tag_id < nr_tags; tag_id++)
    	{
    		// Tag 0 is next
    		if (!next_tag_)
    		{
    			// Check name
    			if (strcmp(pmt::symbol_to_string(tags[tag_id].key).c_str(), tag_out0_.c_str()))
    			{
        			in_position = tags[tag_id].offset - nitems_read(0);
    				continue;
    			}
    			// Check tag value
    			if (!pmt::to_uint64(tags[tag_id].value))
    			{
        			in_position = tags[tag_id].offset - nitems_read(0);
    				continue;
    			}
        		tag0_value = pmt::to_uint64(tags[tag_id].value);
    			tag0_offset = tags[tag_id].offset - nitems_read(0);
    			// Check for buffer overflow
    			if ((out0_position_ + tag0_value) > noutput_items ||
    				(tag0_offset + tag0_value) > ninput_items[0])
    			{
    				in_position = tag0_offset-1;
    				break;
    			}

        		tags[tag_id].offset = out0_position_ + nitems_written(0);
    			// Setup the identifier tag
        		identifier_tag.offset = tags[tag_id].offset;
        		identifier_tag.value = pmt::mp(identifier_);

    			// Save tag and pass data
        		add_item_tag(0, identifier_tag);
        		add_item_tag(0, tags[tag_id]);

        		in_position = tag0_offset;

        		for (int i = 0; i < tag0_value; i++)
        			out0[out0_position_ + i] = in[in_position + i];

        		out0_position_ += tag0_value;
        		in_position += tag0_value;
        		next_tag_ = true;
    		}
    		// Tag 1 is next
    		else
    		{
    			// Check name
    			if (strcmp(pmt::symbol_to_string(tags[tag_id].key).c_str(), tag_out1_.c_str()))
    			{
    				in_position = tags[tag_id].offset - nitems_read(0);
    			    continue;
    			}
    			// Check tag value
    			if (!pmt::to_uint64(tags[tag_id].value))
    			{
    				in_position = tags[tag_id].offset - nitems_read(0);
    				continue;
    			}
        		tag1_value = pmt::to_uint64(tags[tag_id].value);
    			tag1_offset = tags[tag_id].offset - nitems_read(0);
    			// Check for buffer overflow
    			if ((out1_position_ + tag1_value) > noutput_items ||
    				(tag1_offset + tag1_value) > ninput_items[0])
    			{
    				in_position = tag1_offset-1;
    				break;
    			}

        		tags[tag_id].offset = out1_position_ + nitems_written(1);
    			// Setup the identifier tag
        		identifier_tag.offset = tags[tag_id].offset;
        		identifier_tag.value = pmt::mp(identifier_);

    			// Save tag and pass data
        		add_item_tag(1, identifier_tag);
        		add_item_tag(1, tags[tag_id]);

        		in_position = tag1_offset;

        		for (int i = 0; i < tag1_value; i++)
        			out1[out1_position_ + i] = in[in_position + i];

        		out1_position_ += tag1_value;
        		in_position += tag1_value;
//        		if (in[in_position] == 'w')
//        			std::cout << "ob " << tag1_value << "| ";
        		identifier_++;
        		next_tag_ = false;
    		}

    	}

    	consume(0, in_position);
    	produce(0, out0_position_);
    	produce(1, out1_position_);
    	return WORK_CALLED_PRODUCE;
    }

  } /* namespace limesdrmod */
} /* namespace gr */

