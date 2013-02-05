#include <boost/shared_ptr.hpp>
#include <tins/hw_address.h>
#include <tins/tins.h>
#include <string>
#include "pypdu.h" // MUST GO HERE
#include <boost/python.hpp>
#include <boost/python/def.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/implicit.hpp>
#include <boost/function.hpp>

using namespace boost::python;

struct callback_proxy {
    PyObject* obj;
    
    callback_proxy(PyObject* obj) : obj(obj) { }
    
    bool operator()(Tins::RefPacket &pck) {
        PyPacket packet(pck.pdu());
        return call<bool>(obj, packet);
    }
};
class PySniffer : public Tins::Sniffer {
public:
    PySniffer(const std::string &device, unsigned max_packet_size,
      bool promisc = false, const std::string &filter = "")
    : Tins::Sniffer(device, max_packet_size, promisc, filter) {}
    
    PyPacket wrapped_next_packet() {
        return next_packet();
    }
    
    void wrapped_sniff_loop(PyObject* callback_obj, size_t max_packets = 0)
    {
        callback_proxy handler(callback_obj);
        sniff_loop(handler, max_packets);
    }
};

class PyFileSniffer : public Tins::FileSniffer {
public:
    PyFileSniffer(const std::string &file_name, const std::string &filter = "")
    : Tins::FileSniffer(file_name, filter) {}
    
        PyPacket wrapped_next_packet() {
        return next_packet();
    }
    
    void wrapped_sniff_loop(PyObject* callback_obj, size_t max_packets = 0)
    {
        callback_proxy handler(callback_obj);
        sniff_loop(handler, max_packets);
    }
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SnifferSniffLoopOverloads, wrapped_sniff_loop, 1, 2)


BOOST_PYTHON_MODULE(tins)
{
    using namespace Tins;
    
    export_pypdu();
    
    class_<PyEthernetII, bases<PyPDU> >("EthernetII", no_init)
        PYTINS_MAKE_ATTR(uint16_t, EthernetII, payload_type)
        PYTINS_MAKE_ATTR2(EthernetII::address_type, const EthernetII::address_type&, EthernetII, src_addr)
        PYTINS_MAKE_ATTR2(EthernetII::address_type, const EthernetII::address_type&, EthernetII, dst_addr)
    ;
    
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
        .def("next_packet", &PySniffer::wrapped_next_packet)
        .def("sniff_loop", &PySniffer::wrapped_sniff_loop, SnifferSniffLoopOverloads())
    ;
    
    class_<PyFileSniffer, boost::noncopyable>("FileSniffer", init<std::string, optional<std::string> >())
        .def("next_packet", &PyFileSniffer::wrapped_next_packet)
        .def("sniff_loop", &PyFileSniffer::wrapped_sniff_loop, SnifferSniffLoopOverloads())
    ;
    
    implicitly_convertible<std::string, HWAddress<6> >();
    implicitly_convertible<std::string, IPv4Address >();
    implicitly_convertible<std::string, PyRawPDU >();
}
