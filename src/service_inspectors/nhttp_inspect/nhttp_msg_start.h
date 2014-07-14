/****************************************************************************
 *
** Copyright (C) 2014 Cisco and/or its affiliates. All rights reserved.
 * Copyright (C) 2003-2013 Sourcefire, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 2 as
 * published by the Free Software Foundation.  You may not use, modify or
 * distribute this program under any other version of the GNU General
 * Public License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 ****************************************************************************/

//
//  @author     Tom Peters <thopeter@cisco.com>
//
//  @brief      NHttpMsgStart class declaration
//

#ifndef NHTTP_MSG_START_H
#define NHTTP_MSG_START_H

#include "nhttp_msg_section.h"

//-------------------------------------------------------------------------
// NHttpMsgStart class
//-------------------------------------------------------------------------

class NHttpMsgStart: public NHttpMsgSection {
public:
    void analyze();
    void genEvents();

protected:
    NHttpMsgStart(const uint8_t *buffer, const uint16_t bufSize, NHttpFlowData *sessionData_, NHttpEnums::SourceId sourceId_) :
       NHttpMsgSection(buffer, bufSize, sessionData_, sourceId_) {};
    virtual void parseStartLine() = 0;
    void deriveVersionId();

    field startLine;
    field version;
};

#endif





