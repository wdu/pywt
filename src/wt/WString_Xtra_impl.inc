// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */




#include "Wt/WString"
#include <Python.h>


PyObject* WString_str(PyCPPClassInstance* o) {
	Wt::WString* s = PyCPPClassType<Wt::WString>::getCore(o);
	std::string str = s->narrow();
	return PyString_FromString(str.c_str());
}

PyObject* WString_concat(PyCPPClassInstance* o1, PyCPPClassInstance* o2) {
	Wt::WString* r = 0;

	// str + WString
	if (PyString_Check(o1)) {
		r = new Wt::WString(PyString_AsString(o1) + 
				*PyCPPClassType<Wt::WString>::getCore(o2));
	}
	// WString + str
	if (PyString_Check(o2)) {
		r = new Wt::WString(*PyCPPClassType<Wt::WString>::getCore(o1) +
				PyString_AsString(o2));
	}
	// WString + WString
	if (PyObject_IsInstance(o1, (PyObject*)PyCPPClassType<Wt::WString>::Type) &&
	    PyObject_IsInstance(o2, (PyObject*)PyCPPClassType<Wt::WString>::Type)) {
		Wt::WString* s1 =  PyCPPClassType<Wt::WString>::getCore(o1);
		Wt::WString* s2 =  PyCPPClassType<Wt::WString>::getCore(o2);
		r = new Wt::WString(*s1+ *s2);
	}
	if (r) {
		PyCPPClassInstance* robj = PyCPPClassType<Wt::WString>::create(r);
		robj->setPythonOwnership();
		return robj;
	}
	return Py_NotImplemented;
}


static PyNumberMethods WString_AsNumber = {
	(binaryfunc)WString_concat,      // nb_add
	0,      // nb_subtract
	0,      // nb_multiply
	0,      // nb_divide
	0,      // nb_remainder
	0,      // nb_divmod
	0,      // nb_power
	0,      // nb_negative
	0,      // nb_positive
	0,      // nb_absolute
	0,      // nb_nonzero
	0,       // nb_invert
	0,      // nb_lshift
	0,      // nb_rshift
	0,       // nb_and
	0,      // nb_xor
	0,        // nb_or
	0,        // nb_coerce
	0,     // nb_int
	0,      // nb_long
	0,      // nb_float
	0,      // nb_oct
	0,      // nb_hex
	/* Added in release 2.0 */
	0,      // nb_inplace_add
	0,      // nb_inplace_subtract
	0,      // nb_inplace_multiply
	0,      // nb_inplace_divide
	0,      // nb_inplace_remainder
	0,      // nb_inplace_power
	0,      // nb_inplace_lshift
	0,      // nb_inplace_rshift
	0,      // nb_inplace_and
	0,      // nb_inplace_xor
	0,      // nb_inplace_or
	/* Added in release 2.2 */
	/* The following require the Py_TPFLAGS_HAVE_CLASS flag */
	0,      // nb_floor_divide
	0,      // nb_true_divide
	0,      // nb_inplace_floor_divide
	0,      // nb_inplace_true_divide
	/* Added in release 2.5 */
	0,      // nb_index
};


static PySequenceMethods WString_AsSequence = {
		0, //sq_length;
		0, //sq_concat; // used from number because for sequence left type must be WString
		0, //sq_repeat;
		0, //sq_item;
		0, //sq_slice;
		0, //sq_ass_item;
		0, //sq_ass_slice;
		0, //sq_contains;
		/* Added in release 2.0 */
		0, //sq_inplace_concat;
		0, //sq_inplace_repeat;
};



void WString_addExtras(PyTypeObject* t) {
	t->tp_as_number = &WString_AsNumber;
	t->tp_as_sequence = &WString_AsSequence;
	t->tp_flags |= (Py_TPFLAGS_HAVE_INPLACEOPS | Py_TPFLAGS_CHECKTYPES);
	t->tp_str = (unaryfunc)&WString_str;
}

