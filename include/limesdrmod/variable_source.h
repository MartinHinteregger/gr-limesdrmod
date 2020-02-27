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


#ifndef INCLUDED_LIMESDR_VARIABLE_SOURCE_H
#define INCLUDED_LIMESDR_VARIABLE_SOURCE_H

#include <limesdrmod/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace limesdrmod {

    /*!
     * \brief <+description of block+>
     * \ingroup limesdrmod
     *
     */
    class LIMESDRMOD_API variable_source : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<variable_source> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of limesdrmod::variable_source.
       *
       * To avoid accidental use of raw pointers, limesdrmod::variable_source's
       * constructor is in a private implementation
       * class. limesdrmod::variable_source::make is the public interface for
       * creating new instances.
       */
      static sptr make(int value);

      // Callback function to store new value
      virtual void set_value(int value) = 0;
    };

  } // namespace limesdrmod
} // namespace gr

#endif /* INCLUDED_LIMESDR_VARIABLE_SOURCE_H */

