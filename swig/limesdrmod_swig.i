/* -*- c++ -*- */

#define LIMESDRMOD_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "limesdrmod_swig_doc.i"

%{
#include "limesdrmod/sinkmod.h"
#include "limesdrmod/sourcemod.h"
#include "limesdrmod/bitshifter.h"
#include "limesdrmod/variable_source.h"
#include "limesdrmod/correlate_and_sync_bb.h"
#include "limesdrmod/checksum_crc_append_bb.h"
#include "limesdrmod/tagged_mux_bb.h"
#include "limesdrmod/tagged_demux_bb.h"
#include "limesdrmod/checksum_crc_header_checker_bb.h"
#include "limesdrmod/data_rate_bb.h"
#include "limesdrmod/counter_source.h"
%}


%include "limesdrmod/sinkmod.h"
GR_SWIG_BLOCK_MAGIC2(limesdrmod, sinkmod);
%include "limesdrmod/sourcemod.h"
GR_SWIG_BLOCK_MAGIC2(limesdrmod, sourcemod);
%include "limesdrmod/bitshifter.h"
GR_SWIG_BLOCK_MAGIC2(limesdrmod, bitshifter);
%include "limesdrmod/variable_source.h"
GR_SWIG_BLOCK_MAGIC2(limesdrmod, variable_source);

%include "limesdrmod/correlate_and_sync_bb.h"
GR_SWIG_BLOCK_MAGIC2(limesdrmod, correlate_and_sync_bb);
%include "limesdrmod/checksum_crc_append_bb.h"
GR_SWIG_BLOCK_MAGIC2(limesdrmod, checksum_crc_append_bb);
%include "limesdrmod/tagged_mux_bb.h"
GR_SWIG_BLOCK_MAGIC2(limesdrmod, tagged_mux_bb);
%include "limesdrmod/tagged_demux_bb.h"
GR_SWIG_BLOCK_MAGIC2(limesdrmod, tagged_demux_bb);
%include "limesdrmod/checksum_crc_header_checker_bb.h"
GR_SWIG_BLOCK_MAGIC2(limesdrmod, checksum_crc_header_checker_bb);
%include "limesdrmod/data_rate_bb.h"
GR_SWIG_BLOCK_MAGIC2(limesdrmod, data_rate_bb);
%include "limesdrmod/counter_source.h"
GR_SWIG_BLOCK_MAGIC2(limesdrmod, counter_source);
