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


#ifndef INCLUDED_LIMESDRMOD_SOURCEMOD_H
#define INCLUDED_LIMESDRMOD_SOURCEMOD_H

#include <limesdrmod/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace limesdrmod {

    /*!
     * \brief <+description of block+>
     * \ingroup limesdrmod
     *
     */
    class LIMESDRMOD_API sourcemod : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<sourcemod> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of limesdrmod::sourcemod.
       *
       * To avoid accidental use of raw pointers, limesdrmod::sourcemod's
       * constructor is in a private implementation
       * class. limesdrmod::sourcemod::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::string serial,
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

      	// Virtual callback functions
      	virtual void set_rf_freq(float rf_freq) = 0;

      	virtual void set_lna_path(int lna_path, int channel) = 0;

      	virtual void set_nco(float nco_freq, int cmix_mode, int channel) = 0;

      	virtual void set_analog_filter(int analog_filter, float analog_bandw, int channel) = 0;

      	virtual void set_digital_filter(int digital_filter, float digital_bandw, int channel) = 0;

      	virtual void set_gain(int gain_dB, int channel) = 0;
    };
  } // namespace limesdrmod
} // namespace gr

#endif /* INCLUDED_LIMESDRMOD_SOURCEMOD_H */

