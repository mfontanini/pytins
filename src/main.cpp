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
#include <tins/hw_address.h>
#include <tins/tins.h>
#include <string>
#include "pysniffer.h"
#include "pypdu.h" // MUST GO HERE
#include <boost/python.hpp>
#include <boost/python/def.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/implicit.hpp>
#include <boost/function.hpp>
#include "pyethernetII.h"
#include "pyip.h"
#include "pytcp.h"
#include "pyudp.h"
#include "pyicmp.h"
#include "pyrawpdu.h"
#include "pysender.h"

using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SnifferSniffLoopOverloads, sniff_loop, 1, 2)

BOOST_PYTHON_MODULE(tins)
{
    using namespace Tins;
    
    PyPDU::python_register();
    PyEthernetII::python_register();
    
    // ****************************************************************
    // ***************************** PDUs *****************************
    // ****************************************************************
    /*class_<PyEthernetII, bases<PyPDU> >("EthernetII", no_init)
        PYTINS_MAKE_ATTR(uint16_t, EthernetII, payload_type)
        PYTINS_MAKE_ATTR2(EthernetII::address_type, const EthernetII::address_type&, EthernetII, src_addr)
        PYTINS_MAKE_ATTR2(EthernetII::address_type, const EthernetII::address_type&, EthernetII, dst_addr)
    ;*/
    
    class_<PyIP, bases<PyPDU> >("IP", no_init)
        PYTINS_MAKE_ATTR(IP::address_type, IP, src_addr)
        PYTINS_MAKE_ATTR(IP::address_type, IP, dst_addr)
        PYTINS_MAKE_ATTR(small_uint<4>, IP, head_len)
        PYTINS_MAKE_ATTR(small_uint<4>, IP, version)
        PYTINS_MAKE_ATTR(uint8_t, IP, tos)
        PYTINS_MAKE_ATTR(uint16_t, IP, tot_len)
        PYTINS_MAKE_ATTR(uint16_t, IP, id)
        PYTINS_MAKE_ATTR(uint16_t, IP, frag_off)
        PYTINS_MAKE_ATTR(uint8_t, IP, ttl)
        PYTINS_MAKE_ATTR(uint8_t, IP, protocol)
        PYTINS_MAKE_ATTR(uint16_t, IP, check)
    ;
    
    class_<PyTCP, bases<PyPDU> >("TCP", no_init)
        PYTINS_MAKE_ATTR(uint16_t, TCP, sport)
        PYTINS_MAKE_ATTR(uint16_t, TCP, dport)
        PYTINS_MAKE_ATTR(uint32_t, TCP, seq)
        PYTINS_MAKE_ATTR(uint32_t, TCP, ack_seq)
        PYTINS_MAKE_ATTR(uint16_t, TCP, window)
        PYTINS_MAKE_ATTR(uint16_t, TCP, check)
        PYTINS_MAKE_ATTR(uint16_t, TCP, urg_ptr)
    ;
    
    class_<PyUDP, bases<PyPDU> >("UDP", no_init)
        PYTINS_MAKE_ATTR(uint16_t, UDP, sport)
        PYTINS_MAKE_ATTR(uint16_t, UDP, dport)
        PYTINS_MAKE_ATTR(uint16_t, UDP, length)
    ;
    
    class_<PyICMP, bases<PyPDU> >("ICMP", no_init)
        PYTINS_MAKE_ATTR(uint8_t, ICMP, code)
        PYTINS_MAKE_ATTR(uint16_t, ICMP, check)
        PYTINS_MAKE_ATTR(uint16_t, ICMP, id)
        PYTINS_MAKE_ATTR(uint16_t, ICMP, mtu)
        PYTINS_MAKE_ATTR(uint8_t, ICMP, pointer)
        PYTINS_MAKE_ATTR(uint16_t, ICMP, check)
        PYTINS_MAKE_ATTR(uint16_t, ICMP, sequence)
        PYTINS_MAKE_ATTR(uint32_t, ICMP, gateway)
        PYTINS_MAKE_ATTR(ICMP::Flags, ICMP, type)
    ;
    
    enum_<ICMP::Flags>("ICMP")
        .value("ECHO_REPLY", ICMP::ECHO_REPLY)
        .value("DEST_UNREACHABLE", ICMP::DEST_UNREACHABLE)
        .value("SOURCE_QUENCH", ICMP::SOURCE_QUENCH)
        .value("REDIRECT", ICMP::REDIRECT)
        .value("ECHO_REQUEST", ICMP::ECHO_REQUEST)
        .value("TIME_EXCEEDED", ICMP::TIME_EXCEEDED)
        .value("PARAM_PROBLEM", ICMP::PARAM_PROBLEM)
        .value("INFO_REQUEST", ICMP::INFO_REQUEST)
        .value("INFO_REPLY", ICMP::INFO_REPLY)
    ;
    
    class_<PyRawPDU, bases<PyPDU> >("RawPDU", init<const std::string&>())
        .def("__iter__", iterator<PyRawPDU>())
        .def("__str__", &PyRawPDU::to_string)
        .def("__repr__", &PyRawPDU::repr)
    ;
    
    // ****************************************************************
    // ************************* Other stuff **************************
    // ****************************************************************
    
    class_<PyPacketSender>("PacketSender", init<>())
        .def("send", &PyPacketSender::send)
    ;
    
    class_<PyPacket>("Packet", no_init)
        .def("pdu", &PyPacket::pdu, return_internal_reference<1>())
    ;
    
    class_<HWAddress<6> >("HWAddress6", init<const std::string&>())
        .def("__str__", &HWAddress<6>::to_string)
        .def("__repr__", &HWAddress<6>::to_string)
    ;
    
    class_<IPv4Address>("IPv4Address", init<const std::string&>())
        .def("__str__", &IPv4Address::to_string)
        .def("__repr__", &IPv4Address::to_string)
    ;
    
    class_<Tins::PDU::serialization_type>("serialization_type")
        .def(vector_indexing_suite<Tins::PDU::serialization_type>())
    ;
    
    class_<PySniffer, boost::noncopyable>("Sniffer", init<std::string, unsigned, optional<bool, std::string> >())
        .def("next_packet", &PySniffer::next_packet)
        .def("sniff_loop", &PySniffer::sniff_loop, SnifferSniffLoopOverloads())
    ;
    
    class_<PyFileSniffer, boost::noncopyable>("FileSniffer", init<std::string, optional<std::string> >())
        .def("next_packet", &PyFileSniffer::next_packet)
        .def("sniff_loop", &PyFileSniffer::sniff_loop, SnifferSniffLoopOverloads())
    ;
    
    implicitly_convertible<std::string, HWAddress<6> >();
    implicitly_convertible<std::string, IPv4Address >();
    implicitly_convertible<std::string, PyRawPDU >();
}
