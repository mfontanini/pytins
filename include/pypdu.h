#ifndef PYTINS_PYPDU_H
#define PYTINS_PYPDU_H

#include <boost/mpl/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <tins/pdu.h>
#include <tins/small_uint.h>
#include <tins/packet.h>

class PyPDU { 
public:
    static PyPDU *from_pdu(Tins::PDU *pdu);

    PyPDU(Tins::PDU *pdu_ptr) : pdu_(pdu_ptr) { }
    
    virtual ~PyPDU() {}

    uint32_t header_size() const {
        return pdu()->header_size();
    }
    
    Tins::PDU* clone() const {
        return pdu()->clone();
    }
    
    Tins::PDU::PDUType pdu_type() const {
        return pdu()->pdu_type();
    }
    
    Tins::PDU::serialization_type serialize() {
        return pdu()->serialize();
    }
    
    const Tins::PDU *pdu() const {
        return pdu_;
    }
    
    Tins::PDU *pdu() {
        return pdu_;
    }
    
    PyPDU *inner_pdu() {
        return PyPDU::from_pdu(pdu_->inner_pdu());
    }
    
private:
    Tins::PDU *pdu_;
};

template<typename T>
class generic_pdu : public PyPDU {
public:
    generic_pdu(Tins::PDU *pdu) 
    : PyPDU(pdu) { }
};

/*
 *************************** PyPDU subclasses **************************
 */

#include "pyethernetII.h"
#include "pyip.h"
#include "pytcp.h"
#include "pyudp.h"
#include "pyicmp.h"
#include "pyrawpdu.h"

/*
 ***********************************************************************
 */

PyPDU *PyPDU::from_pdu(Tins::PDU *pdu) {
    switch(pdu->pdu_type()) {
        case Tins::PDU::ETHERNET_II:
            return new PyEthernetII(pdu);
        case Tins::PDU::IP:
            return new PyIP(pdu);
        case Tins::PDU::TCP:
            return new PyTCP(pdu);
        case Tins::PDU::UDP:
            return new PyUDP(pdu);
        case Tins::PDU::ICMP:
            return new PyICMP(pdu);
        default:
            return 0;
    };
}

template<typename T>
struct type_filter {
    typedef T type;
};

template<size_t n>
struct type_filter<Tins::small_uint<n> > {
    typedef typename Tins::small_uint<n>::repr_type type;
};


template<typename Result, typename Class>
struct getter_wrapper {
    typedef typename type_filter<Result>::type result_type;
    typedef Result (Class::*function_type)() const;
    
    getter_wrapper(function_type function)
    : function(function) { }

    result_type operator()(const PyPDU *pdu) const {
        const Class *cl = static_cast<const Class*>(pdu->pdu());
        return (cl->*function)();
    }

    function_type function;
};

template<typename ArgType, typename Class>
struct setter_wrapper {
    typedef typename type_filter<ArgType>::type arg_type;
    typedef void (Class::*function_type)(ArgType);
    
    setter_wrapper(function_type function)
    : function(function) { }

    void operator()(PyPDU *pdu, arg_type arg) const {
        Class *cl = static_cast<Class*>(pdu->pdu());
        (cl->*function)(arg);
    }

    function_type function;
};

namespace boost { namespace python { namespace detail {

    template <class Result, typename Class>
    inline boost::mpl::vector<typename getter_wrapper<Result, Class>::result_type, const PyPDU*>
    get_signature(getter_wrapper<Result, Class>, const PyPDU* = 0)
    {
        return boost::mpl::vector<
            typename getter_wrapper<Result, Class>::result_type, 
            const PyPDU*
        >();
    }
    
    template <class ArgType, typename Class>
    inline boost::mpl::vector<void, PyPDU*, typename setter_wrapper<ArgType, Class>::arg_type>
    get_signature(setter_wrapper<ArgType, Class>, PyPDU* = 0)
    {
        return boost::mpl::vector<
            void, 
            PyPDU*, 
            typename setter_wrapper<ArgType, Class>::arg_type
        >();
    }

} } }

template<typename Result, typename Class>
getter_wrapper<Result, Class> 
make_getter_wrapper(Result (Class::*fun)() const) {
    return getter_wrapper<Result, Class>(fun);
}

template<typename ArgType, typename Class>
setter_wrapper<ArgType, Class> 
make_setter_wrapper(void (Class::*fun)(ArgType)) {
    return setter_wrapper<ArgType, Class>(fun);
}

class PyPacket {
public:
    PyPacket(const Tins::PtrPacket &pck) 
    : pdu_(PyPDU::from_pdu(const_cast<Tins::PDU*>(pck.pdu()))) { 
    }
    
    PyPacket(const Tins::PDU &pck) 
    : pdu_(PyPDU::from_pdu(pck.clone())) { 
    }
    
    PyPDU& pdu() {
        return *pdu_;
    }
private:
    boost::shared_ptr<PyPDU> pdu_;
};

#include <boost/python.hpp>
#include <boost/python/def.hpp>

#define PYTINS_GETTER_FUN(RETURN, CLASS, NAME) \
    make_getter_wrapper((RETURN (CLASS::*)() const)(&CLASS::NAME))

#define PYTINS_SETTER_FUN(ARGTYPE, CLASS, NAME) \
    make_setter_wrapper((void (CLASS::*)(ARGTYPE))(&CLASS::NAME))

#define PYTINS_MAKE_ATTR(ATTRTYPE, CLASS, NAME) \
    .add_property(#NAME, PYTINS_GETTER_FUN(ATTRTYPE, CLASS, NAME), PYTINS_SETTER_FUN(ATTRTYPE, CLASS, NAME))

#define PYTINS_MAKE_ATTR2(GETTER_TYPE, SETTER_TYPE, CLASS, NAME) \
    .add_property(#NAME, PYTINS_GETTER_FUN(GETTER_TYPE, CLASS, NAME), PYTINS_SETTER_FUN(SETTER_TYPE, CLASS, NAME))

inline void export_pypdu()
{
    using namespace boost::python;
    
    class_<PyPDU>("PDU", no_init)
        .def("serialize", &PyPDU::serialize)
        .def("inner_pdu", &PyPDU::inner_pdu, return_value_policy<manage_new_object>())
    ;
}

#endif // PYTINS_PYPDU_H
