#ifndef PYTINS_PYPDU_H
#define PYTINS_PYPDU_H

#include <boost/mpl/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <tins/pdu.h>
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

/*
 *************************** PyPDU subclasses **************************
 */

#include "pyethernetII.h"
#include "pyip.h"

/*
 ***********************************************************************
 */

PyPDU *PyPDU::from_pdu(Tins::PDU *pdu) {
    switch(pdu->pdu_type()) {
        case Tins::PDU::ETHERNET_II:
            return new PyEthernetII(pdu);
        case Tins::PDU::IP:
            return new PyIP(pdu);
        default:
            return 0;
    };
}

template<typename Result, typename Class>
struct getter_wrapper {
    typedef Result (Class::*function_type)() const;
    
    getter_wrapper(function_type function)
    : function(function) { }

    Result operator()(const PyPDU *pdu) const {
        const Class *cl = dynamic_cast<const Class*>(pdu->pdu());
        if(!cl)
            // fixme
            throw std::runtime_error("invalid cast");
        return (cl->*function)();
    }

    function_type function;
};

template<typename ArgType, typename Class>
struct setter_wrapper {
    typedef void (Class::*function_type)(ArgType);
    
    setter_wrapper(function_type function)
    : function(function) { }

    void operator()(PyPDU *pdu, ArgType arg) const {
        Class *cl = dynamic_cast<Class*>(pdu->pdu());
        if(!cl)
            // fixme
            throw std::runtime_error("invalid cast");
        (cl->*function)(arg);
    }

    function_type function;
};

namespace boost { namespace python { namespace detail {

    template <class Result, typename Class>
    inline boost::mpl::vector<Result, const PyPDU*>
    get_signature(getter_wrapper<Result, Class>, const PyPDU* = 0)
    {
        return boost::mpl::vector<Result, const PyPDU*>();
    }
    
    template <class ArgType, typename Class>
    inline boost::mpl::vector<void, PyPDU*, ArgType>
    get_signature(setter_wrapper<ArgType, Class>, PyPDU* = 0)
    {
        return boost::mpl::vector<void, PyPDU*, ArgType>();
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
