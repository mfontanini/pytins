/*
 * Copyright (c) 2012, Matias Fontanini
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <boost/shared_ptr.hpp>
#include <tins/ip.h>
#include <tins/tcp.h>
#include <tins/udp.h>
#include <tins/icmp.h>
#include <string>
#include "pysniffer.h"
#include "pypdu.h" // MUST GO HERE
#include <boost/python.hpp>
#include <boost/python/def.hpp>
#include <boost/python/implicit.hpp>
#include <boost/function.hpp>
#include "pyethernetII.h"
#include "pyip.h"
#include "pyipv6.h"
#include "pytcp.h"
#include "pyudp.h"
#include "pyarp.h"
#include "pydhcp.h"
#include "pyicmp.h"
#include "pyrawpdu.h"
#include "pysender.h"
#include "pypacketwriter.h"
#include "pyhelpers.h"

using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SnifferSniffLoopOverloads, sniff_loop, 1, 2)

BOOST_PYTHON_MODULE(tins_core)
{
    using namespace Tins;
    
    PyPDU::python_register();
    PyEthernetII::python_register();
    PyHelpers::python_register();
    PyTCP::python_register();
    PyIP::python_register();
    PyIPv6::python_register();
    PyUDP::python_register();
    PyICMP::python_register();
    PyARP::python_register();
    PyRawPDU::python_register();
    PyDHCP::python_register();
    PyPacketSender::python_register();
    PyPacketWriter::python_register();
    
    
    class_<PyPacket>("Packet", no_init)
        .def("pdu", &PyPacket::pdu, return_internal_reference<1>())
    ;
    
    class_<PySniffer, boost::noncopyable>("Sniffer", init<std::string, unsigned, optional<bool, std::string> >())
        .def("next_packet", &PySniffer::next_packet)
        .def("sniff_loop", &PySniffer::sniff_loop, SnifferSniffLoopOverloads())
    ;
    
    class_<PyFileSniffer, boost::noncopyable>("FileSniffer", init<std::string, optional<std::string> >())
        .def("next_packet", &PyFileSniffer::next_packet)
        .def("sniff_loop", &PyFileSniffer::sniff_loop, SnifferSniffLoopOverloads())
    ;
    
    implicitly_convertible<std::string, PyRawPDU >();
}
