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
#include "tagged_mux_bb_impl.h"

namespace gr {
  namespace limesdrmod {

    tagged_mux_bb::sptr
    tagged_mux_bb::make(std::string tag_in0, std::string tag_in1,
    		const std::vector<int> &guard)
    {
      return gnuradio::get_initial_sptr
        (new tagged_mux_bb_impl(tag_in0, tag_in1, guard));
    }

    /*
     * The private constructor
     */
    tagged_mux_bb_impl::tagged_mux_bb_impl(std::string tag_in0, std::string tag_in1,
    		const std::vector<int> &guard)
      : gr::block("tagged_mux_bb",
              gr::io_signature::make(2, 2, sizeof(char)),
              gr::io_signature::make(1, 1, sizeof(char)))
    {
        tag_in0_ = tag_in0;
        tag_in1_ = tag_in1;
        guard_ = guard;
        guard_len_ = guard_.size();

        set_tag_propagation_policy(TPP_DONT);
    }

    /*
     * Our virtual destructor.
     */
    tagged_mux_bb_impl::~tagged_mux_bb_impl()
    {
    }

    int
    tagged_mux_bb_impl::general_work(int noutput_items,
            gr_vector_int &ninput_items,
            gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items)
    {
    	const unsigned char *in0 = (const unsigned char*) input_items[0];
    	const unsigned char *in1 = (const unsigned char*) input_items[1];
    	unsigned char *out = (unsigned char *) output_items[0];

    	// Get all tags
    	std::vector <tag_t> tags0;
    	get_tags_in_range(tags0, 0, nitems_read(0), nitems_read(0)+ninput_items[0]);
    	int nr_tags0 = tags0.size();
    	int tag_index0 = 0;
    	int in_index0 = 0;
    	uint64_t tag_value0 = 0;
    	tag_t tag0;

    	std::vector <tag_t> tags1;
    	get_tags_in_range(tags1, 1, nitems_read(1), nitems_read(1)+ninput_items[1]);
    	int nr_tags1 = tags1.size();
    	int tag_index1 = 0;
    	int in_index1 = 0;
    	uint64_t tag_value1 = 0;
    	tag_t tag1;

    	// No tags on input0
    	if (!nr_tags0)
    	{
    		std::cout << "tagged_mux_bb Warning: Input0 has no tags." << std::endl;
    		consume(0, ninput_items[0]);
    		return 0;
    	}
    	// No tags on input1
    	if (!nr_tags1)
    	{
    		std::cout << "tagged_mux_bb Warning: Input1 has no tags." << std::endl;
    		consume(1, ninput_items[1]);
    		return 0;
    	}

    	uint64_t out_position = 0;
    	while(true)
    	{
    		// Tag 0 is next
    		if (!next_tag_)
    		{
    			// ===========================================
    			// Port0
    			tag0 = tags0[tag_index0];
    			// Check if name matches
    			while (strcmp(pmt::symbol_to_string(tag0.key).c_str(),
    					tag_in0_.c_str()))
    			{
    				tag_index0++;
    				if (tag_index0 + 1 > nr_tags0)
    					break;

    				tag0 = tags0[tag_index0];
    			}

	    		in_index0 = tag0.offset - nitems_read(0);
    	    	// No valid tag found
    	    	if (tag_index0 + 1 > nr_tags0)
    	    		break;

    	    	tag_index0++;
    	    	// Check tag value
    	    	if (!(pmt::to_uint64(tag0.value)))
    	    		break;

    	    	tag_value0 = pmt::to_uint64(tag0.value);
    	    	// Check overflows
    	    	if ((out_position + tag_value0) > noutput_items ||
    	    		(in_index0 + tag_value0) > ninput_items[0])
    	    	{
    	    		//std::cout << "tagged_mux_bb Warning: tag0 overflow. " <<
    	    		//		tag_in0_ << std::endl;
    	    		in_index0 = tag0.offset - 1 - nitems_read(0);
    	    		break;
    	    	}

    	    	// Pass to output
    	    	for (int i = 0; i < tag_value0; i++)
    	    		out[out_position + i] = in0[in_index0 + i];

    	    	tag0.offset = out_position + nitems_written(0);
    	    	add_item_tag(0, tag0);

    	    	out_position += tag_value0;
    	    	in_index0 += tag_value0;

    	    	next_tag_ = true;
    		}
    		else
    		{
    			// ===========================================
    			// Port1
    			tag1 = tags1[tag_index1];
    			// Check if name matches
    			while (strcmp(pmt::symbol_to_string(tag1.key).c_str(),
    					tag_in1_.c_str()))
    			{
    				tag_index1++;
    				if (tag_index1 + 1 > nr_tags1)
    					break;

    				tag1 = tags1[tag_index1];
    			}

	    		in_index1 = tag1.offset - nitems_read(1);
    	    	// No valid tag found
    	    	if (tag_index1 + 1 > nr_tags1)
    	    		break;

    	    	tag_index1++;
    	    	// Check tag value
    	    	if (!(pmt::to_uint64(tag1.value)))
    	    		break;

    	    	tag_value1 = pmt::to_uint64(tag1.value);
    	    	// Check overflows
    	    	if ((out_position + tag_value1 + guard_len_) > noutput_items ||
    	    		(in_index1 + tag_value1) > ninput_items[1])
    	    	{
    	    		std::cout << "tagged_mux_bb Warning: tag1 overflow. " <<
    	    				tag_in1_ << std::endl;
    	    		in_index1 = tag1.offset - 1 - nitems_read(1);
    	    		break;
    	    	}

    	    	// Pass to output
    	    	for (int i = 0; i < tag_value1; i++)
    	    		out[out_position + i] = in1[in_index1 + i];

    	    	tag1.offset = out_position + nitems_written(0);
    	    	add_item_tag(0, tag1);
    	    	out_position += tag_value1;

    	    	// Add guard sequence
    	    	for (int i = 0; i < guard_len_; i++)
    	    		out[out_position + i] = guard_[i];

    	    	out_position += guard_len_;
    	    	in_index1 += tag_value1;

    	    	next_tag_ = false;
    		}

    		if (!set_fixed_rate_ && tag_value0 && tag_value1)
    		{
    			set_output_multiple(tag_value0 + tag_value1 + guard_len_);
    			set_fixed_rate_ = true;
    			break;
    		}
    	}

    	consume(0, in_index0);
    	consume(1, in_index1);
    	produce(0, out_position);
    	return WORK_CALLED_PRODUCE;
    }

  } /* namespace limesdrmod */
} /* namespace gr */

