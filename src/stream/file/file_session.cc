//--------------------------------------------------------------------------
// Copyright (C) 2015-2016 Cisco and/or its affiliates. All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License Version 2 as published
// by the Free Software Foundation.  You may not use, modify or distribute
// this program under any other version of the GNU General Public License.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//--------------------------------------------------------------------------
// file_session.cc author Russ Combs <rucombs@cisco.com>

#include "file_session.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "detection/detection_engine.h"
#include "file_api/file_api.h"
#include "file_api/file_flows.h"
#include "profiler/profiler.h"
#include "packet_io/sfdaq.h"
#include "perf_monitor/perf_monitor.h"
#include "target_based/snort_protocols.h"

#include "stream_file.h"
#include "file_module.h"

#define DECODE_PDU (DECODE_SOF | DECODE_EOF)

THREAD_LOCAL ProfileStats file_ssn_stats;

//-------------------------------------------------------------------------
// FileSession methods
//-------------------------------------------------------------------------

FileSession::FileSession(Flow* flow) : Session(flow) { }

FileSession::~FileSession() { }

bool FileSession::setup(Packet*)
{
    // FIXIT-H file context is null here
    //const char* s = DAQ_GetInterfaceSpec();
    //file_api->set_file_name(p->flow, (uint8_t*)s, strlen(s));
    return true;
}

void FileSession::clear() { }

static FilePosition position(Packet* p)
{
    if ( (p->ptrs.decode_flags & DECODE_PDU) == DECODE_PDU )
        return SNORT_FILE_FULL;

    if ( p->ptrs.decode_flags & DECODE_SOF )
        return SNORT_FILE_START;

    if ( p->ptrs.decode_flags & DECODE_EOF )
        return SNORT_FILE_END;

    return SNORT_FILE_MIDDLE;
}

int FileSession::process(Packet* p)
{
    Profile profile(file_ssn_stats);

    p->flow->ssn_state.application_protocol = SNORT_PROTO_USER;
    StreamFileConfig* c = get_file_cfg(p->flow->ssn_server);

    FileFlows* file_flows = FileFlows::get_file_flows(p->flow);

    if (file_flows &&
        file_flows->file_process((uint8_t*)p->data, p->dsize, position(p), c->upload))
    {
        const char* file_name = SFDAQ::get_interface_spec();
        if (file_name)
            file_flows->set_file_name((uint8_t*)file_name, strlen(file_name));
    }
    set_file_data(p->data, p->dsize);

    return 0;
}

