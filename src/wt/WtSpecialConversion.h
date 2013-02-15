// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef WTSPECIALCONVERSION_H
#define WTSPECIALCONVERSION_H

#include <Python.h>
#include <ostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <Wt/WSignal>
#include <Wt/WJavaScript>
#include <boost/any.hpp>
#include "ConversionBase.h"
#include "CPPClassConversion.h"
#include "PyException.h"
#include "PythonApplication.h"

namespace Wt {
	class WEvent;
}

template <class T> class Opaque;

/******************************************/

template <>
class C2Python<const Wt::WEvent&> {
	public: static PyObject* arg(const Wt::WEvent&);
};


/******************************************/





template<class A1, class A2, class A3, class A4, class A5, class A6>
class C2Python<  Wt::Signal<A1, A2, A3, A4, A5, A6>&>: public C2Python_ref< Wt::Signal<A1, A2, A3, A4, A5, A6> > {};


template<class A1, class A2, class A3, class A4, class A5, class A6>
class C2Python<const  Wt::Signal<A1, A2, A3, A4, A5, A6>&>: public C2Python_cref< Wt::Signal<A1, A2, A3, A4, A5, A6> > {};

template<class A1, class A2, class A3, class A4, class A5, class A6>
class C2Python<  Wt::JSignal<A1, A2, A3, A4, A5, A6>&>: public C2Python_ref< Wt::JSignal<A1, A2, A3, A4, A5, A6> > {};

template<class A1>
class C2Python<  Wt::EventSignal<A1>&>: public C2Python_ref< Wt::EventSignal<A1> > {};

template<class A1>
class C2Python<  Wt::EventSignal<A1>*>: public C2Python_ptr< Wt::EventSignal<A1> > {};

template<class T>
class Python2C<Opaque< T >& > {
};



namespace Wt {class WApplication;}
template <>
class C2Python<Wt::WApplication*> {
	public:
		static PyObject* ret(Wt::WApplication* a);
};

template<>
class Python2C<Wt::WApplication*> {
	public:
		static bool acceptarg(PyObject* o);
		static Wt::WApplication* arg(PyObject* o);
		typedef Wt::WApplication* S;
};



template <>
class Python2C<boost::any> {
	public: static boost::any arg(PyObject* o);
	public: static boost::any ret(PyObject* o);
};


template <>
class Python2C<const boost::any&> {
	public:
		static bool acceptarg(PyObject* o);
		static const boost::any& arg(PyObject* o, boost::any**);
		typedef boost::any S;
};

// boost:any
template <>
class C2Python<const boost::any&> {
	public:
		static PyObject* arg(const boost::any& o);
		static PyObject* ret(const boost::any& o);
};

// boost:any
template <>
class C2Python<boost::any> {
	public:
		static PyObject* ret(const boost::any& o);
};




#endif
