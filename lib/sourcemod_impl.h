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

#ifndef INCLUDED_LIMESDRMOD_SOURCEMOD_IMPL_H
#define INCLUDED_LIMESDRMOD_SOURCEMOD_IMPL_H

#include <time.h>
#include <limesdrmod/sourcemod.h>
#include "common/device_handler.h"

namespace gr {
  namespace limesdrmod {

    class sourcemod_impl : public sourcemod
    {
     private:
    	lms_stream_t streamId[2];

    	long t = std::time(0);

    	int LMS_CH_0 = 0;
    	int LMS_CH_1 = 1;
    	int source_block = 1;
    	int agcValue = 0;

    	struct constant_data
    	{
    	  std::string serial;
    	  int device_number;
    	  int device_type;
    	  int chip_mode;
    	  int channel;
    	  float samp_rate;
    	  bool use_agc;
    	  bool stream_analyzer;
    	} stored;

     public:
      sourcemod_impl(std::string serial,
  		    int device_type,
  		    int chip_mode,
  		    int channel,
			int use_agc,
			int stream_analyzer,
  		    int file_switch,
  		    const char *filename,
  		    double rf_freq,
  		    double samp_rate,
  		    size_t oversample,
  		    int calibration_ch0,
  		    double calibr_bandw_ch0,
  		    int calibration_ch1,
  		    double calibr_bandw_ch1,
  		    int lna_path_mini,
  		    int lna_path_ch0,
  		    int lna_path_ch1,
  		    int analog_filter_ch0,
  		    double analog_bandw_ch0,
  		    int analog_filter_ch1,
  		    double analog_bandw_ch1,
  		    int digital_filter_ch0,
  		    double digital_bandw_ch0,
  		    int digital_filter_ch1,
  		    double digital_bandw_ch1,
  		    int gain_dB_ch0,
  		    int gain_dB_ch1,
  		    float nco_freq_ch0,
  		    float nco_freq_ch1,
  		    int cmix_mode_ch0,
  		    int cmix_mode_ch1);
      ~sourcemod_impl();

  	int general_work(int noutput_items,
  			gr_vector_int &ninput_items,
  			gr_vector_const_void_star &input_items,
  			gr_vector_void_star &output_items);

  	bool start(void);

  	bool stop(void);

  	inline gr::io_signature::sptr args_to_io_signature(int channel_number);

  	void init_stream(int device_number, int channel, float samp_rate);

  	void set_rf_freq(float rf_freq);

  	void set_lna_path(int lna_path, int channel);

  	void set_nco(float nco_freq, int cmix_mode, int channel);

  	void set_analog_filter(int analog_filter, float analog_bandw, int channel);

  	void set_digital_filter(int digital_filter, float digital_bandw, int channel);

  	void set_gain(int gain_dB, int channel);

  	void set_agc(int agc);
    };
  } // namespace limesdrmod
} // namespace gr

#endif /* INCLUDED_LIMESDRMOD_SOURCEMOD_IMPL_H */

