// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#include <stdlib.h>
#include "PyCPPClassInstance.h"


PyObject* wlength_repr(PyCPPClassInstance* o) {

	Wt::WLength* l = PyCPPClassType<Wt::WLength>::getCore(o);
	return PyString_FromString(l->cssText().c_str());
}

PyObject* wlength_mult(PyCPPClassInstance* o1, PyCPPClassInstance* o2) {
	if (PyObject_IsInstance(o1, (PyObject*)PyCPPClassType<Wt::WLength>::Type)&& PyNumber_Check(o2)) {
		PyObject* floatobj = PyNumber_Float(o2);
		if (!floatobj) return 0;
		double v = PyFloat_AS_DOUBLE(floatobj);
		Py_DECREF(floatobj);

		Wt::WLength* l = PyCPPClassType<Wt::WLength>::getCore(o1);
		if (!l) return 0;

		Wt::WLength* r = new Wt::WLength;
		*r = *l * v;

		PyCPPClassInstance* robj = PyCPPClassType<Wt::WLength>::create(r);
		robj->setPythonOwnership();
		return robj;
	}

	else if (PyObject_IsInstance(o2, (PyObject*)PyCPPClassType<Wt::WLength>::Type)&& PyNumber_Check(o1)) {
		PyObject* floatobj = PyNumber_Float(o1);
		if (!floatobj) return 0;
		double v = PyFloat_AS_DOUBLE(floatobj);
		Py_DECREF(floatobj);

		Wt::WLength* l = PyCPPClassType<Wt::WLength>::getCore(o2);
		if (!l) return 0;

		Wt::WLength* r = new Wt::WLength;
		*r = v * *l;

		PyCPPClassInstance* robj = PyCPPClassType<Wt::WLength>::create(r);
		robj->setPythonOwnership();
		return robj;
	}
	else {
		PyErr_Format(PyExc_TypeError, "Cannot multiply '%s' and '%s'", o1->ob_type->tp_name, o2->ob_type->tp_name);
		return 0;
	}
}


PyObject* wlength_div(PyCPPClassInstance* o1, PyCPPClassInstance* o2) {
	if (PyObject_IsInstance(o1, (PyObject*)PyCPPClassType<Wt::WLength>::Type) && PyNumber_Check(o2)) {
		PyObject* floatobj = PyNumber_Float(o2);
		if (!floatobj) return 0;
		double v = PyFloat_AS_DOUBLE(floatobj);
		Py_DECREF(floatobj);

		Wt::WLength* l = PyCPPClassType<Wt::WLength>::getCore(o1);
		if (!l) return 0;

		//TODO: check for division by 0
		Wt::WLength* r = new Wt::WLength;
		*r = *l / v;
		PyCPPClassInstance* robj = PyCPPClassType<Wt::WLength>::create(r);
		robj->setPythonOwnership();
		return robj;
	}
	else {
		PyErr_Format(PyExc_TypeError, "Cannot divide '%s' and '%s'", o1->ob_type->tp_name, o2->ob_type->tp_name);
		return 0;
	}
}

/*
static PyObject* wlength_ipmult(PyObject* obj, PyObject* arg) {
	PyObject* floatobj = PyNumber_Float(arg);
	if (!floatobj) return 0;
	double v = PyFloat_AS_DOUBLE(floatobj);
	Py_DECREF(floatobj);

	Wt::WLength* l = PyCPPClassType<Wt::WLength>::getCore(obj);
	if (!l) return 0;

	(*l) *= v;

	Py_INCREF(obj);
	return obj;
}
*/



