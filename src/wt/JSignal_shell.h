// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef JSIGNAL_SHELL
#define JSIGNAL_SHELL


#include <Wt/WObject>
#include <Wt/WJavaScript>
#include <Wt/WJavaScriptSlot>
#include <string>
#include <Python.h>
#include "PyCPPMethodWrapper.h" // for CATCHALL

template <class A1, class A2, class A3, class A4, class A5, class A6>
class JSignal_shell: public Wt::JSignal<A1, A2, A3, A4, A5, A6> {
	protected:
		PyTypeObject** signature;
	public:
	  JSignal_shell(Wt::WObject *object, const std::string& name,
				        bool collectSlotJavaScript = false, PyTypeObject** sig = 0): Wt::JSignal<A1, A2, A3, A4, A5, A6>(object, name, collectSlotJavaScript), signature(sig) {
	  }
		void processDynamic(const Wt::JavaScriptEvent& jse);

};

namespace Wt {
	template <>
		struct SignalArgTraits<PyObject*>
		{
			static PyObject* unMarshal(const JavaScriptEvent& jse, int argi) {
				if ((unsigned)argi >= jse.userEventArgs.size())
					throw Wt::WException("Missing JavaScript argument");

				return PyString_FromString(jse.userEventArgs[argi].c_str());
			}
		};
}



template <class T>
inline T PyUnMarshal(const Wt::JavaScriptEvent&, int, PyTypeObject*) {
	return T();
}

template <>
inline Wt::NoClass PyUnMarshal<Wt::NoClass>(const Wt::JavaScriptEvent&, int, PyTypeObject*) {
	return Wt::NoClass::none;
}

template <>
inline PyObject* PyUnMarshal<PyObject*>(const Wt::JavaScriptEvent& jse, int argi, PyTypeObject* reqtype) {
	if ((unsigned)argi >= jse.userEventArgs.size()) {
		PyErr_SetString(PyExc_RuntimeError, "Missing Javascript argument");
		return 0;
	}
	if (reqtype == 0) {
		PyErr_SetString(PyExc_RuntimeError, "Missing Javascript argument");
		return 0;
		//return UntypedJSArgument_New(jse.userEventArgs[argi].c_str());
	}
	if (PyType_IsSubtype(reqtype, &PyInt_Type)) {
		return  PyInt_FromString((char*)jse.userEventArgs[argi].c_str(), 0, 0);
	}
	else if (PyType_IsSubtype(reqtype, &PyString_Type)) {
		return  PyString_FromString(jse.userEventArgs[argi].c_str());
	}
	else if (PyType_IsSubtype(reqtype, &PyFloat_Type)) {
		PyObject* r = 0;
		PyObject* flstr = PyString_FromString(jse.userEventArgs[argi].c_str());
		r = PyFloat_FromString(flstr, 0);
		Py_DECREF(flstr);
		return r;
	}
	else {
		if (jse.userEventArgs[argi] == "0") Py_RETURN_FALSE;
		Py_RETURN_TRUE;
	}
}

template <class T>
inline void DiscardEmitArg(T) {}

template <>
inline void DiscardEmitArg<PyObject*>(PyObject* obj) {Py_XDECREF(obj);}



template <typename A1, typename A2, typename A3,
	typename A4, typename A5, typename A6>
void JSignal_shell<A1, A2, A3, A4, A5, A6>::processDynamic(const Wt::JavaScriptEvent& jse)
{
	A1 a1 = PyUnMarshal<A1>(jse, 0, signature[0]);
	A2 a2 = PyUnMarshal<A2>(jse, 1, signature[1]);
	A3 a3 = PyUnMarshal<A3>(jse, 2, signature[2]);
	A4 a4 = PyUnMarshal<A4>(jse, 3, signature[3]);
	A5 a5 = PyUnMarshal<A5>(jse, 4, signature[4]);
	A6 a6 = PyUnMarshal<A6>(jse, 5, signature[5]);
	
	if (!PyErr_Occurred()) {
		try {
			this->emit(a1, a2, a3, a4, a5, a6);
		}
		CATCHALL();
	}

	DiscardEmitArg<A1>(a1);
	DiscardEmitArg<A2>(a2);
	DiscardEmitArg<A3>(a3);
	DiscardEmitArg<A4>(a4);
	DiscardEmitArg<A5>(a5);
	DiscardEmitArg<A6>(a6);

	if (PyErr_Occurred()) {
		throw PyException();
	}
}
















#endif
