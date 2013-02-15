// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef JSIGNALTYPEPROVIDER
#define JSIGNALTYPEPROVIDER

#include <Python.h>

typedef struct {
	    PyObject_HEAD
} JSignalTypeProviderObject;

inline bool checkTemplateType(PyObject* t) {
	if (!PyType_Check(t)) {
		PyObject* repr = PyObject_Repr(t);
		PyErr_Format(PyExc_TypeError, "'%s' is not a type.", PyString_AsString(repr));
		Py_DECREF(repr);
		return false;
	}
	PyTypeObject* typ = (PyTypeObject*)t;
	if (PyType_IsSubtype(typ, &PyInt_Type) ||
		PyType_IsSubtype(typ, &PyString_Type) ||
		PyType_IsSubtype(typ, &PyFloat_Type) ||
		PyType_IsSubtype(typ, &PyBool_Type)) return true;
		PyErr_Format(PyExc_TypeError, "'%s' is not a valid template type.", typ->tp_name);
	return false;
}

inline bool signatureFromTuple(PyObject* tuple, PyTypeObject** sig) {
	int s = PyTuple_GET_SIZE(tuple);
	for (int i = 0; i<s; i++) {
		sig[i] = (PyTypeObject*)PyTuple_GET_ITEM(tuple, i);
		if (!checkTemplateType((PyObject*)sig[i])) return false;
	}
	for (int i = s; i< 7; i++) sig[i] = 0;
	return true;
}

inline bool signatureFromObject(PyObject* object, PyTypeObject** sig) {
	if (!checkTemplateType(object)) return false;
	sig[0] = (PyTypeObject*)object;
	for (int i = 1; i< 7; i++) sig[i] = 0;
	return true;
}

inline void signatureNone(PyTypeObject** sig) {
	for (int i = 0; i< 7; i++) sig[i] = 0;
}

static void JSignalTypeProvider_dealloc(PyObject* self) {
	delete self;
}

static PyTypeObject* JSignalTypeProvider_GetItem(JSignalTypeProviderObject* self, PyObject* what) {
	PyTypeObject* r = 0;
	PyTypeObject* signature[7];
	if (!PyTuple_Check(what)) {
		if (what == Py_None) {
			signatureNone(signature);
			void* buf = malloc(sizeof(PyJSignal<>));
			r = new /*(buf)*/ PyJSignal<>("PyJSignal<>", signature);
		}
		else {
			if (!signatureFromObject(what, signature)) return 0;
			void* buf = malloc(sizeof(PyJSignal<PyObject*>));
			r = new /*(buf)*/ PyJSignal<PyObject*>("PyJSignal<1>", signature);
		}
	}
	else {
		if (!signatureFromTuple(what, signature)) return 0;
		if (PyTuple_GET_SIZE(what) == 0) {
			void* buf = malloc(sizeof(PyJSignal<>));
			r = new /*(buf)*/ PyJSignal<>("PyJSignal<>", signature);
		}
		else if (PyTuple_GET_SIZE(what) == 1) {
			void* buf = malloc(sizeof(PyJSignal<PyObject*>));
			r = new /*(buf)*/ PyJSignal<PyObject*>("PyJSignal<1>", signature);
		}
		else if (PyTuple_GET_SIZE(what) == 2) {
			void* buf = malloc(sizeof(PyJSignal<PyObject*, PyObject*>));
			r = new /*(buf)*/ PyJSignal<PyObject*, PyObject*>("PyJSignal<2>", signature);
		}
		else if (PyTuple_GET_SIZE(what) == 3) {
			void* buf = malloc(sizeof(PyJSignal<PyObject*, PyObject*, PyObject*>));
			r = new /*(buf)*/ PyJSignal<PyObject*, PyObject*, PyObject*>("PyJSignal<3>", signature);
		}
		else {
			PyErr_SetString(PyExc_TypeError, "Wrong number of type specifiers");
			return 0;
		}
	}
	PyType_Ready(r);
	Py_INCREF(r); // temporary for debug: never delete
	return r;
}

static PyMappingMethods JSignalMap = {
	0,
	(binaryfunc)JSignalTypeProvider_GetItem,
	0
};

	static PyTypeObject JSignalTypeProviderType = {
		PyObject_HEAD_INIT(NULL)
			0,                         /*ob_size*/
		"wt.JSignal",             /*tp_name*/
		sizeof(JSignalTypeProviderObject),             /*tp_basicsize*/
		0,                         /*tp_itemsize*/
		JSignalTypeProvider_dealloc, /*tp_dealloc*/
		0,                         /*tp_print*/
		0,                         /*tp_getattr*/
		0,                         /*tp_setattr*/
		0,                         /*tp_compare*/
		0,                         /*tp_repr*/
		0,                         /*tp_as_number*/
		0,                         /*tp_as_sequence*/
		&JSignalMap,                         /*tp_as_mapping*/
		0,                         /*tp_hash */
		0,                         /*tp_call*/
		0,                         /*tp_str*/
		0,                         /*tp_getattro*/
		0,                         /*tp_setattro*/
		0,                         /*tp_as_buffer*/
		Py_TPFLAGS_DEFAULT, /*tp_flags*/
		"Noddy objects",           /* tp_doc */
		0,		               /* tp_traverse */
		0,		               /* tp_clear */
		0,		               /* tp_richcompare */
		0,		               /* tp_weaklistoffset */
		0,		               /* tp_iter */
		0,		               /* tp_iternext */
		0,             /* tp_methods */
		0,             /* tp_members */
		0,           /* tp_getset */
		0,                         /* tp_base */
		0,                         /* tp_dict */
		0,                         /* tp_descr_get */
		0,                         /* tp_descr_set */
		0,                         /* tp_dictoffset */
		0,      /* tp_init */
		0,                         /* tp_alloc */
		PyType_GenericNew,                 /* tp_new */
	};

#endif
