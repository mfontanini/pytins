#include <boost/shared_ptr.hpp>
#include <tins/hw_address.h>
#include <tins/pdu.h>
#include <tins/sniffer.h>
#include <tins/ethernetII.h>
#include <tins/ip.h>
#include <tins/ip_address.h>
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
class Sniffer : public Tins::Sniffer {
public:
    Sniffer(const std::string &device, unsigned max_packet_size,
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

class FileSniffer : public Tins::FileSniffer {
public:
    FileSniffer(const std::string &file_name, const std::string &filter = "")
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
    using Tins::EthernetII;
    using Tins::IP;
    using Tins::HWAddress;
    using Tins::IPv4Address;
    
    export_pypdu();
    
    class_<PyEthernetII, bases<PyPDU> >("EthernetII", no_init)
        PYTINS_MAKE_ATTR(uint16_t, EthernetII, payload_type)
        PYTINS_MAKE_ATTR2(EthernetII::address_type, const EthernetII::address_type&, EthernetII, src_addr)
        PYTINS_MAKE_ATTR2(EthernetII::address_type, const EthernetII::address_type&, EthernetII, dst_addr)
    ;
    
    class_<PyIP, bases<PyPDU> >("IP", no_init)
        PYTINS_MAKE_ATTR(IP::address_type, IP, src_addr)
        PYTINS_MAKE_ATTR(IP::address_type, IP, dst_addr)
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
    
    class_<Sniffer, boost::noncopyable>("Sniffer", init<std::string, unsigned, optional<bool, std::string> >())
        .def("next_packet", &Sniffer::wrapped_next_packet)
        .def("sniff_loop", &Sniffer::wrapped_sniff_loop, SnifferSniffLoopOverloads())
    ;
    
    class_<FileSniffer, boost::noncopyable>("FileSniffer", init<std::string, optional<std::string> >())
        .def("next_packet", &FileSniffer::wrapped_next_packet)
        .def("sniff_loop", &FileSniffer::wrapped_sniff_loop, SnifferSniffLoopOverloads())
    ;
    
    implicitly_convertible<std::string, HWAddress<6> >();
    implicitly_convertible<std::string, IPv4Address >();
}
