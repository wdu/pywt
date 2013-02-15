// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */




#include "Compare.h"
#include "PyCPPClassInstance.h"

PyObject* compare_ptr(PyObject *a, PyObject *b, int op) {
	PyObject* r = 0;
	if (!PyObject_IsSubclass((PyObject*)(a->ob_type), (PyObject*)(b->ob_type)) &&
	    !PyObject_IsSubclass((PyObject*)(b->ob_type), (PyObject*)(a->ob_type))) {
		PyErr_Format(PyExc_TypeError, "Cannot compare '%s' and '%s'", a->ob_type->tp_name, b->ob_type->tp_name);
		return 0;
	}
	PyCPPClassInstance* ia = static_cast<PyCPPClassInstance*>(a);
	PyCPPClassInstance* ib = static_cast<PyCPPClassInstance*>(b);
	void* corea = (ia->proxy)?ia->proxy->getCoreAsVoid():0;
	void* coreb = (ib->proxy)?ib->proxy->getCoreAsVoid():0;
	switch (op) {
		case Py_LT:
			r = (corea <  coreb)?Py_True:Py_False;
			break;
		case Py_LE:
			r = (corea <= coreb)?Py_True:Py_False;
			break;
		case Py_EQ:
			r = (corea == coreb)?Py_True:Py_False;
			break;
		case Py_NE:
			r = (corea != coreb)?Py_True:Py_False;
			break;
		case Py_GT:
			r = (corea >  coreb)?Py_True:Py_False;
			break;
		case Py_GE:
			r = (corea >= coreb)?Py_True:Py_False;
			break;
	}
	Py_INCREF(r);
	return r;
}


