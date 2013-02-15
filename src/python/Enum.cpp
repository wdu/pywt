// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#include "Enum.h"
#include <Python.h>
#include "structmember.h"
#include <string>
#include <map>

long Enum_Hash(PyObject* o) {
	PyObject* asint = Enum_ToInt(o);
	long v = PyObject_Hash(asint);
	Py_DECREF(asint);
	return v;
}

/*************** Python base Type for all Enums ***********/
// No Objects of this type will exist. It is only use for
// type checking, and as an initialization struct for the real
// Enum<T> types
PyNumberMethods Enum_AsNumber = {
	0,		// nb_add
	0,		// nb_subtract
	0,		// nb_multiply
	0,		// nb_divide
	0,		// nb_remainder
	0,		// nb_divmod
	0,		// nb_power
	0,		// nb_negative
	0,		// nb_positive
	0,		// nb_absolute
	0,		// nb_nonzero
	Enum_Not,		// nb_invert
	0,		// nb_lshift
	0,		// nb_rshift
	Enum_And,		// nb_and
	0,		// nb_xor
	Enum_Or,		// nb_or
	0,		// nb_coerce
	Enum_ToInt,		// nb_int
	0,		// nb_long
	0,		// nb_float
	0,		// nb_oct
	0,		// nb_hex
	0,		// nb_inplace_add
	0,		// nb_inplace_subtract
	0,		// nb_inplace_multiply
	0,		// nb_inplace_divide
	0,		// nb_inplace_remainder
	0,		// nb_inplace_power
	0,		// nb_inplace_lshift
	0,		// nb_inplace_rshift
	0,		// nb_inplace_and
	0,		// nb_inplace_xor
	0,		// nb_inplace_or
	0,		// nb_floor_divide
	0,		// nb_true_divide
	0,		// nb_inplace_floor_divide
	0,		// nb_inplace_true_divide
	Enum_ToInt,		// nb_index
};

PyTypeObject EnumTypeBase = {
	PyObject_HEAD_INIT(NULL)
	0,                         /*ob_size*/
	"Enum",             /*tp_name*/
	sizeof(PyObject),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	0, /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	0,                         /*tp_repr*/
	0,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	0,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT, /*tp_flags*/
	0,           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	0,             /* tp_methods */
	0,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	0,      /* tp_init */
	0,                         /* tp_alloc */
	0,                 /* tp_new */
};


PyTypeObject EnumInit = {
	PyObject_HEAD_INIT(NULL)
	0,                         /*ob_size*/
	0,             /*tp_name*/
	0,             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	0, /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	0,                         /*tp_repr*/
	0,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	Enum_Hash,                         /*tp_hash */
	0,                         /*tp_call*/
	0,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	0, /*tp_flags*/
	0,           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	Enum_Compare,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	0,             /* tp_methods */
	0,             /* tp_members */
	0,                         /* tp_getset */
	&EnumTypeBase,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	0,      /* tp_init */
	0,                         /* tp_alloc */
	0,                 /* tp_new */
};


/*************** Check that a python object is an enum ***********/
inline Enum* createEnumOfSameType(PyObject* o, unsigned int value) {
	PyTypeObject* obtype = o->ob_type;
	Enum* self = (Enum*)obtype->tp_alloc(obtype, 0);
	self->valuestr = 0;
	self->value = value;
	return self;
}

bool Enum_Check(PyObject* o) {
	return 0 != (PyObject_IsInstance(o, (PyObject*)&EnumTypeBase));
}

#define compareTypes(o1, o2) \
	if (o1->ob_type != o2->ob_type) {\
		PyErr_SetString(PyExc_ValueError, "Operation not permitted for these 2 values"); \
		return 0; \
	}\

static PyObject* Enum_Or(PyObject* o1, PyObject* o2) {
	compareTypes(o1, o2);
	return createEnumOfSameType(o1, Enum_GetIntValue(o1) | Enum_GetIntValue(o2));
}

static PyObject* Enum_And(PyObject* o1, PyObject* o2) {
	compareTypes(o1, o2);
	return createEnumOfSameType(o1, Enum_GetIntValue(o1) & Enum_GetIntValue(o2));
}

static PyObject* Enum_Not(PyObject* o1) {
	return createEnumOfSameType(o1, ~Enum_GetIntValue(o1));
}

static PyObject* Enum_ToInt(PyObject* o1) {
	return PyInt_FromLong(Enum_GetIntValue(o1));
}


// only called when a or b is an enum
// supported comparisons are enum and int or 2 enums of the same type
static PyObject* Enum_Compare(PyObject *a, PyObject *b, int op) {
	PyObject* r = 0;
	int a_val;
	int b_val;

	if (PyInt_Check(a)) {
		a_val = PyInt_AS_LONG(a);
		b_val = Enum_GetIntValue(b);
	}
	else if (PyInt_Check(b)) {
		a_val = Enum_GetIntValue(a);
		b_val = PyInt_AS_LONG(b);
	}
	else if (a->ob_type == b->ob_type) {
		a_val = Enum_GetIntValue(a);
		b_val = Enum_GetIntValue(b);
	}
	else {
		PyErr_Format(PyExc_TypeError, "Cannot compare %s and %s", a->ob_type->tp_name, b->ob_type->tp_name);
		return 0;
	}

	switch (op) {
		case Py_LT:
			r = (a_val <  b_val)?Py_True:Py_False;
			break;
		case Py_LE:
			r = (a_val <=  b_val)?Py_True:Py_False;
			break;
		case Py_EQ:
			r = (a_val ==  b_val)?Py_True:Py_False;
			break;
		case Py_NE:
			r = (a_val !=  b_val)?Py_True:Py_False;
			break;
		case Py_GT:
			r = (a_val >  b_val)?Py_True:Py_False;
			break;
		case Py_GE:
			r = (a_val >=  b_val)?Py_True:Py_False;
			break;
	}
	Py_INCREF(r);
	return r;
}



