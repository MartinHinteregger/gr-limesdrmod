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


#ifndef INCLUDED_LIMESDRMOD_CORRELATE_AND_SYNC_BB_H
#define INCLUDED_LIMESDRMOD_CORRELATE_AND_SYNC_BB_H

#include <limesdrmod/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace limesdrmod {

    /*!
     * \brief <+description of block+>
     * \ingroup limesdrmod
     *
     */
    class LIMESDRMOD_API correlate_and_sync_bb : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<correlate_and_sync_bb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of limesdrmod::correlate_and_sync_bb.
       *
       * To avoid accidental use of raw pointers, limesdrmod::correlate_and_sync_bb's
       * constructor is in a private implementation
       * class. limesdrmod::correlate_and_sync_bb::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::vector<int> &sync,
    		  int packet_len, const char* length_tag_name,
			  const char* header_tag_name, int header_len, int checksum_len);
    };

  } // namespace limesdrmod
} // namespace gr

#endif /* INCLUDED_LIMESDRMOD_CORRELATE_AND_SYNC_BB_H */

