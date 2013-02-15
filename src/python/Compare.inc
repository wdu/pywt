// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */




#include "Compare.h"
#include "PyCPPClassInstance.h"
#include "PyCPPClassType.h"

template <class T>
PyObject* compare_val_eq(PyObject *a, PyObject *b, int op) {
	PyObject* r = 0;
	if (!PyObject_IsInstance(a, (PyObject*)PyCPPClassType<T>::Type) ||
	    !PyObject_IsInstance(b, (PyObject*)PyCPPClassType<T>::Type)) {
		PyErr_Format(PyExc_TypeError, "Cannot compare %s and %s", a->ob_type->tp_name, b->ob_type->tp_name);
		return 0;
    	} 
	PyCPPClassInstance* ia = static_cast<PyCPPClassInstance*>(a);
	PyCPPClassInstance* ib = static_cast<PyCPPClassInstance*>(b);
	T* corea = PyCPPClassType<T>::getCore(ia); 
	T* coreb = PyCPPClassType<T>::getCore(ib); 
	switch (op) {
		case Py_EQ:
			r = corea->operator==(*coreb)?Py_True:Py_False;
			break;
		case Py_NE:
			r = (!corea->operator==(*coreb))?Py_True:Py_False;
			break;
		default:
			PyErr_Format(PyExc_TypeError, "No order relation defined for '%s'", a->ob_type->tp_name);
			return 0;
	}
	Py_INCREF(r);
	return r;

}


template <class T>
PyObject* compare_val_eq_lt(PyObject *a, PyObject *b, int op) {
	PyObject* r = 0;
	if (!PyObject_IsInstance(a, (PyObject*)PyCPPClassType<T>::Type) ||
	    !PyObject_IsInstance(b, (PyObject*)PyCPPClassType<T>::Type)) {
		PyErr_Format(PyExc_TypeError, "Cannot compare %s and %s", a->ob_type->tp_name, b->ob_type->tp_name);
		return 0;
    	} 
	PyCPPClassInstance* ia = static_cast<PyCPPClassInstance*>(a);
	PyCPPClassInstance* ib = static_cast<PyCPPClassInstance*>(b);
	T* corea = PyCPPClassType<T>::getCore(ia); 
	T* coreb = PyCPPClassType<T>::getCore(ib); 
	switch (op) {
		case Py_LT:
			r = (*corea <  *coreb)?Py_True:Py_False;
			break;
		case Py_LE:
			r = (!(*coreb < *corea))?Py_True:Py_False;
			break;
		case Py_EQ:
			r = (*corea == *coreb)?Py_True:Py_False;
			break;
		case Py_NE:
			r = (!(*corea == *coreb))?Py_True:Py_False;
			break;
		case Py_GT:
			r = (*coreb <  *corea)?Py_True:Py_False;
			break;
		case Py_GE:
			r = (!(*corea < *coreb))?Py_True:Py_False;
			break;
	}
	Py_INCREF(r);
	return r;
}

