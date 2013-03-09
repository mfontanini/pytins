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

#include <string>
#include <vector>
#include <list>
#include <boost/python.hpp>
#include <boost/python/def.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <tins/hw_address.h>
#include <tins/ip_address.h>
#include <tins/ipv6_address.h>
#include <tins/network_interface.h>
#include "pyhelpers.h"

using namespace boost::python;
using namespace Tins;

namespace PyHelpers {
    void python_register() {
        class_<HWAddress<6> >("HWAddress6", init<const std::string&>())
            .def(init<>())
            .def("__str__", &HWAddress<6>::to_string)
            .def("__repr__", &HWAddress<6>::to_string)
            .def(self == HWAddress<6>())
            .def(self != HWAddress<6>())
            .def(self < HWAddress<6>())
        ;
        
        class_<IPv4Address>("IPv4Address", init<const std::string&>())
            .def(init<>())
            .def("__str__", &IPv4Address::to_string)
            .def("__repr__", &IPv4Address::to_string)
            .def(self == IPv4Address())
            .def(self != IPv4Address())
            .def(self < IPv4Address())
        ;
        
        class_<IPv6Address>("IPv6Address", init<const std::string&>())
            .def(init<>())
            .def("__str__", &IPv6Address::to_string)
            .def("__repr__", &IPv6Address::to_string)
            .def(self == IPv6Address())
            .def(self != IPv6Address())
            .def(self < IPv6Address())
        ;

        class_<NetworkInterface::Info>("InterfaceInfo", no_init)
            .add_property("ip_addr", &NetworkInterface::Info::ip_addr)
            .add_property("netmask", &NetworkInterface::Info::netmask)
            .add_property("bcast_addr", &NetworkInterface::Info::bcast_addr)
            .add_property("hw_addr", &NetworkInterface::Info::hw_addr)
        ;
        
        class_<NetworkInterface>("NetworkInterface", init<>())
            .def(init<const std::string&>())
            .add_property("__str__", &NetworkInterface::name)
            .add_property("addresses", &NetworkInterface::addresses)
            .add_property("id", &NetworkInterface::id)
        ;
     
        // Containers
        class_<std::vector<uint8_t>>("vector_8")
            .def(vector_indexing_suite<std::vector<uint8_t>>())
        ;
        
        class_<std::vector<uint16_t>>("vector_16")
            .def(vector_indexing_suite<std::vector<uint16_t>>())
        ;
        
        class_<std::vector<uint32_t>>("vector_32")
            .def(vector_indexing_suite<std::vector<uint32_t>>())
        ;
        
        class_<std::vector<IPv4Address>>("vector_ipv4")
            .def(vector_indexing_suite<std::vector<IPv4Address>>())
        ;
        
        class_<std::vector<IPv6Address>>("vector_ipv6")
            .def(vector_indexing_suite<std::vector<IPv6Address>>())
        ;
        
        implicitly_convertible<std::string, HWAddress<6> >();
        implicitly_convertible<std::string, IPv4Address>();
        implicitly_convertible<std::string, NetworkInterface>();
    }
}
