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
#include "data_rate_bb_impl.h"

namespace gr {
  namespace limesdrmod {

    data_rate_bb::sptr
    data_rate_bb::make()
    {
      return gnuradio::get_initial_sptr
        (new data_rate_bb_impl());
    }

    /*
     * The private constructor
     */
    data_rate_bb_impl::data_rate_bb_impl()
      : gr::block("data_rate_bb",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(2, 2, sizeof(float)))
    {
    	results_buffer_[0] = 'c';
    	results_buffer_[1] = 'a';
    	results_buffer_[2] = 'f';
    	results_buffer_[3] = 'e';

    	timer_.restart();
    }

    /*
     * Our virtual destructor.
     */
    data_rate_bb_impl::~data_rate_bb_impl()
    {
    }

    int
    data_rate_bb_impl::general_work(int noutput_items,
  		  gr_vector_int &ninput_items,
		  gr_vector_const_void_star &input_items,
		  gr_vector_void_star &output_items)
    {
    	const unsigned char *in = (const unsigned char*) input_items[0];
    	float *out0 = (float *) output_items[0];
    	float *out1 = (float *) output_items[1];

    	//std::cout << "d: ";

    	//std::cout << timer_.elapsed() << std::endl;
    	if (timer_.elapsed() >= avg_time_)
    	{
    		// calc
    		float data_rate = 8 * (float)nr_all_bits_ / (avg_time_ * 1000);
    		float bit_error_rate = 100 * ((float)nr_all_bits_-(float)nr_correct_bits_) /
    				(float)nr_all_bits_;

        	//std::cout << "Result: " << nr_correct_bits_ << "/" << nr_all_bits_ << std::endl;
        	std::cout << "Data Rate: " << data_rate << ", BER: " << bit_error_rate << std::endl;

    		out0[0] = data_rate;
    		out1[0] = bit_error_rate;

    		nr_correct_bits_ = 0;
    		nr_all_bits_ = 0;

    		timer_.restart();
    		timer_overflow_ = true;
    		produce(0, 1);
    		produce(1, 1);
    	}

    	for (int i = 0; i < ninput_items[0]; i++)
    	{
    		switch (in[i])
    		{
    		//case results_buffer_[0]:
    		case 'c':
			nr_correct_bits_ += 8;
			nr_all_bits_ += 8;
    		last_char_ = 0;
    		break;

    		//case results_buffer_[1]:
    		case 'a':
			nr_correct_bits_ += 8;
			nr_all_bits_ += 8;
    		last_char_ = 1;
    		break;

    		//case results_buffer_[2]:
    		case 'f':
			nr_correct_bits_ += 8;
			nr_all_bits_ += 8;
    		last_char_ = 2;
    		break;

    		//case results_buffer_[3]:
    		case 'e':
			nr_correct_bits_ += 8;
			nr_all_bits_ += 8;
    		last_char_ = 3;
    		break;

    		default:
    	    	char diff;

    			diff = in[i] ^ !results_buffer_[(last_char_+1)%4];

    			for (int bit = 0; bit < 8; bit++)
    			{
    				if (!((diff >> i) & 0x1))
    					nr_correct_bits_++;
    			}
    			last_char_ = (last_char_+1)%4;
    			nr_all_bits_ += 8;
    		}
    	}

    	//std::cout << "done! " << std::endl;
    	//nr_all_bits_ = 8*ninput_items[0];

    	consume(0, ninput_items[0]);
    	if (timer_overflow_)
    		return WORK_CALLED_PRODUCE;
    	else
    		return 0;
    }

  } /* namespace limesdrmod */
} /* namespace gr */

