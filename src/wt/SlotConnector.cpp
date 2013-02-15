// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */



#include "SlotConnector.h"
#include "PyCPPClassInstance.h"
#include "PyInstanceFactory.h"

inline int PyCFunction_GET_NARGS(PyObject* f) {
	const char* name = ((PyCFunctionObject *)f) -> m_ml -> ml_name;
	int pos = strlen(name)+1;
	return (int)(name[pos]);
}



SlotConnectorBase::SlotConnectorBase(PyObject* slot, int n_sig_args) {
	function = 0;
	klass = 0;
	PyObject* instance = 0;
	cfunction = 0;
	if (PyMethod_Check(slot)) {
		instance = PyMethod_GET_SELF(slot);

		klass =    PyMethod_GET_CLASS(slot);
		function = PyMethod_GET_FUNCTION(slot);
		Py_INCREF(klass);
		Py_INCREF(function);
		PyObject* func_code = PyObject_GetAttrString(function, "func_code");
		PyObject* co_argcount = PyObject_GetAttrString(func_code, "co_argcount");
		n_slot_args = PyInt_AsLong(co_argcount)-1;
		if (n_slot_args == -1) {
			PyErr_SetString(PyExc_TypeError, "Slot method should at least have a 'self' argument");
			return;
		}
		core = ((Wt::WObject*)(((PyCPPClassInstance*)instance)->proxy->getCoreAsVoid()));
		Py_DECREF(func_code);
		Py_DECREF(co_argcount);
	}
	else if PyCFunction_Check(slot) {
		instance = PyCFunction_GET_SELF(slot);
		cfunction = PyCFunction_GET_FUNCTION(slot);
		flags = PyCFunction_GET_FLAGS(slot);
		n_slot_args = PyCFunction_GET_NARGS(slot);
		core = ((Wt::WObject*)(((PyCPPClassInstance*)instance)->proxy->getCoreAsVoid()));
	}
	else {
		PyErr_SetString(PyExc_TypeError, "Slot must be a bound method");
		return;
	}
	if (n_slot_args > n_sig_args) {
		PyErr_SetString(PyExc_TypeError, "Slot has more arguments than signal");
	}
}

void SlotConnectorBase::doCall(PyObject* args) {
	PyObject* r = 0;


	PyObject* method = 0;
	PyObject* instance = PyCPPClassType<Wt::WObject>::Type->Factory->create(core);
	if (cfunction) {
		if (flags & METH_KEYWORDS) {
			PyCFunctionWithKeywords f = (PyCFunctionWithKeywords)cfunction;
			r = f(instance, args, 0);
		}
		else {
			PyErr_SetString(PyExc_TypeError, "Slot only supports METH_KEYWORDS methods");
			throw PyException();
		}
	}
	else if (function) {
		method = PyMethod_New(function, instance,  klass);
		try {
		r = PyObject_Call(method, args, 0);
		}
		catch (std::exception& e) {
			PyErr_SetString(PyExc_RuntimeError, e.what());
			throw PyException();
		}
		catch (...) {
			PyErr_SetString(PyExc_RuntimeError, "Unknown error from slot call");
			throw PyException();
		}
	}
	if (r) {
		if (r != Py_None) {
			PyErr_SetString(PyExc_TypeError, "Slot should return 'None'");
			throw PyException();
		}
	}
	if (PyErr_Occurred()) {
		throw PyException();
	}


	Py_DECREF(args);
	Py_XDECREF(method);
	Py_DECREF(instance);
	Py_XDECREF(r);

}

