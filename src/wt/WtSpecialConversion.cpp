// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#include "WtSpecialConversion.h"
#include "ImplicitConversion.h"
#include "PyInstanceFactory.h"
#include "PyCPPClassType.h"
#include "PyCPPClassInstance.h"
#include "CPPClassConversion.inc"
#include <Wt/WGlobal>
#include <Wt/WDialog>
#include <Wt/WGoogleMap>
#include <Wt/WValidator>
#include <string>
namespace Wt {
	class WModelIndex;
	class WMouseEvent;
	class WScrollEvent;
	class WKeyEvent;
	class WDate;
	class WStandardButton;
	class WRadioButton;
	class WStandardItem;
};



template class C2Python_ref< Wt::Signal<Wt::Orientation, int, int> >;
template class C2Python_ref< Wt::Signal<Wt::WModelIndex > >;
template class C2Python_ref< Wt::Signal<int, Wt::WFormWidget* > >;
template class C2Python_ref< Wt::Signal<Wt::WModelIndex, int, int > >;
template class C2Python_ref< Wt::Signal<Wt::WModelIndex, Wt::WMouseEvent > >;
template class C2Python_ref< Wt::Signal<Wt::WModelIndex, Wt::WModelIndex > >;
template class C2Python_ref< Wt::Signal<Wt::StandardButton > >;
template class C2Python_ref< Wt::Signal<Wt::WDate> >;
template class C2Python_ref< Wt::Signal<> >;
template class C2Python_ref< Wt::Signal< Wt::WDialog::DialogCode > >;
template class C2Python_ref< Wt::Signal< Wt::WString > >;
template class C2Python_ref< Wt::Signal< Wt::WRadioButton* > >;
template class C2Python_ref< Wt::Signal< Wt::WStandardItem* > >;
template class C2Python_ref< Wt::Signal< Wt::WMenuItem* > >;
template class C2Python_ref< Wt::Signal< Wt::WValidator::Result > >;
template class C2Python_ref< Wt::Signal< int, Wt::WLength > >;
template class C2Python_ref< Wt::Signal< std::string > >;
template class C2Python_ref< Wt::Signal< bool > >;
template class C2Python_ref< Wt::Signal< int > >;
//template class C2Python_ref< Wt::Signal< long long > >;
template class C2Python_ref< Wt::Signal< double > >;
template class C2Python_ref< Wt::Signal< int, int > >;
template class C2Python_ref< Wt::Signal< int64_t > >;
template class C2Python_ref< Wt::Signal< int64_t, int64_t > >;
template class C2Python_ref< Wt::Signal< uint64_t, uint64_t > >;
#ifndef _LP64
template class C2Python_ref< Wt::Signal< unsigned long, unsigned long > >;
#endif

template class C2Python_ref< Wt::Signal< Wt::WWidget*, bool > >;
template class C2Python_cref< Wt::Signal< Wt::WWidget*, bool > >;

template class C2Python_ref<Wt::JSignal<Wt::WGoogleMap::Coordinate > >;
template class C2Python_ref<Wt::JSignal<int > >;
template class C2Python_ref<Wt::JSignal< > >;


template class C2Python_ptr< Wt::EventSignal< Wt::WMouseEvent > >;
template class C2Python_ptr< Wt::EventSignal< Wt::WScrollEvent > >;
template class C2Python_ptr< Wt::EventSignal< Wt::NoClass  > >;
template class C2Python_ptr< Wt::EventSignal< Wt::WKeyEvent > >;
template class C2Python_ref< Wt::EventSignal< Wt::WMouseEvent > >;
template class C2Python_ref< Wt::EventSignal< Wt::WScrollEvent > >;
template class C2Python_ref< Wt::EventSignal< Wt::NoClass  > >;
template class C2Python_ref< Wt::EventSignal< Wt::WKeyEvent > >;





#include "PyException.h"
#include "PythonApplication.h"
#include "gwt_Conversion.h"

#include "Wt/WApplication"


PyObject* C2Python<Wt::WApplication*>::ret(Wt::WApplication* a) {
	return C2Python<PythonApplication*>::ret((PythonApplication*)a);
}

bool Python2C<Wt::WApplication*>::acceptarg(PyObject* o) {
	return Python2C<PythonApplication*>::acceptarg(o);
}

Wt::WApplication* Python2C<Wt::WApplication*>::arg(PyObject* o) {
	return Python2C<PythonApplication*>::arg(o);
}


// returns a boost::any value, can be used by arg and ret
static boost::any PyObject2BoostAny(PyObject* obj) {
	if (obj == Py_None) return boost::any();

	if (PyInt_Check(obj)) return boost::any(PyInt_AS_LONG(obj));
	if (PyLong_Check(obj)) return boost::any(PyLong_AsLongLong(obj)); 
	if (PyFloat_Check(obj)) return boost::any(PyFloat_AS_DOUBLE(obj)); 

	if (PyString_Check(obj)) return boost::any(std::string(PyString_AS_STRING(obj))); 
	if (PyObject_IsInstance(obj, (PyObject*)(PyCPPClassType<Wt::WString>::Type))) 
	       return boost::any(* PyCPPClassType<Wt::WString>::getCore((PyCPPClassInstance*)obj));	

	if (PyObject_IsInstance(obj, (PyObject*)(PyCPPClassType<Wt::WDate>::Type))) 
	       return boost::any(* PyCPPClassType<Wt::WDate>::getCore((PyCPPClassInstance*)obj));	
	if (PyObject_IsInstance(obj, (PyObject*)(PyCPPClassType<Wt::WDateTime>::Type))) 
	       return boost::any(* PyCPPClassType<Wt::WDateTime>::getCore((PyCPPClassInstance*)obj));	
	if (PyObject_IsInstance(obj, (PyObject*)(PyCPPClassType<Wt::WTime>::Type))) 
	       return boost::any(* PyCPPClassType<Wt::WTime>::getCore((PyCPPClassInstance*)obj));	


	PyErr_Format(PyExc_TypeError, "Cannot convert '%s' to boost::any", obj->ob_type->tp_name);
	throw PyException();
	
}


// returns a new python object, which contains a new core object, can be used for arg and ret
static PyObject* BoostAny2PyObject(const boost::any& obj) {

	if (obj.empty()) Py_RETURN_NONE;
	if (obj.type() == typeid(double)) return PyFloat_FromDouble(boost::any_cast<double>(obj));
	if (obj.type() == typeid(float)) return PyFloat_FromDouble(boost::any_cast<float>(obj));

	// use the C2Python functions: these will correctly return a python int or long depending on
	// the value
	if (obj.type() == typeid(short)) return C2Python<short>::ret(boost::any_cast<short>(obj));
	if (obj.type() == typeid(unsigned short)) return C2Python<short>::ret(boost::any_cast<unsigned short>(obj));
	if (obj.type() == typeid(int)) return C2Python<int>::ret(boost::any_cast<int>(obj));
	if (obj.type() == typeid(unsigned int)) return C2Python<unsigned int>::ret(boost::any_cast<unsigned int>(obj));
	if (obj.type() == typeid(long)) return C2Python<long>::ret(boost::any_cast<long>(obj));
	if (obj.type() == typeid(unsigned long)) return C2Python<unsigned long>::ret(boost::any_cast<unsigned long>(obj));
	if (obj.type() == typeid(long long)) return C2Python<long long>::ret(boost::any_cast<long long>(obj));
	if (obj.type() == typeid(int64_t)) return C2Python<int64_t>::ret(boost::any_cast<int64_t>(obj));
	if (obj.type() == typeid(uint64_t)) return C2Python<uint64_t>::ret(boost::any_cast<uint64_t>(obj));

	if (obj.type() == typeid(Wt::WString)) return C2Python<Wt::WString>::ret(boost::any_cast<Wt::WString>(obj));
	if (obj.type() == typeid(std::string)) return C2Python<std::string>::ret(boost::any_cast<std::string>(obj));
	if (obj.type() == typeid(const char*)) return C2Python<std::string>::ret(std::string(boost::any_cast<const char*>(obj)));

	if (obj.type() == typeid(Wt::WDate)) return C2Python<Wt::WDate>::ret(boost::any_cast<Wt::WDate>(obj));
	if (obj.type() == typeid(Wt::WDateTime)) return C2Python<Wt::WDateTime>::ret(boost::any_cast<Wt::WDateTime>(obj));
	if (obj.type() == typeid(Wt::WTime)) return C2Python<Wt::WTime>::ret(boost::any_cast<Wt::WTime>(obj));
	PyErr_SetString(PyExc_TypeError, "Unsupported type in boost::any");
	return 0;
}


boost::any Python2C<boost::any>::arg(PyObject* o) {
	return PyObject2BoostAny(o);
}

boost::any Python2C<boost::any>::ret(PyObject* o) {
	return PyObject2BoostAny(o);
}


bool Python2C<const boost::any&>::acceptarg(PyObject* obj) {
	return (obj == Py_None ||
		PyInt_Check(obj) ||
		PyLong_Check(obj) ||
		PyFloat_Check(obj) ||
		PyString_Check(obj) ||
		PyObject_IsInstance(obj, (PyObject*)(PyCPPClassType<Wt::WString>::Type)) ||
		PyObject_IsInstance(obj, (PyObject*)(PyCPPClassType<Wt::WDate>::Type)) ||
			false);

}

const boost::any& Python2C<const boost::any&>::arg(PyObject* o, boost::any** r) {
	*r = new boost::any();
	**r = PyObject2BoostAny(o);
	return **r;
}


PyObject* C2Python<const boost::any&>::arg(const boost::any& o) {
	return BoostAny2PyObject(o);
}

PyObject* C2Python<const boost::any&>::ret(const boost::any& o) {
	return BoostAny2PyObject(o);
}

PyObject* C2Python<boost::any>::ret(const boost::any& o) {
	return BoostAny2PyObject(o);
	Py_RETURN_NONE;
}






