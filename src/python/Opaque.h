// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef OPAQUE_H
#define OPAQUE_H

#include <Python.h>
#include "structmember.h"

/*************** Object representing a python Enum of type T ***********/
class Opaque: public PyObject {
	public:
		void* object;
};


static PyTypeObject OpaqueInit = {
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
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	0,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	0, /*tp_flags*/
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



template <class T>
class OpaqueType: public PyTypeObject {
	protected:
		static void dealloc(Opaque* self) {
			self->ob_type->tp_free((PyObject*)self);
		}

	public:
		static OpaqueType<T>* Type;

		// Constructor
		OpaqueType(const char* name) {
			*((PyTypeObject*)this) = OpaqueInit;
			tp_basicsize = sizeof(Opaque);
			tp_dealloc = (destructor)dealloc;
			tp_name = name;
			tp_flags = Py_TPFLAGS_DEFAULT;
			PyType_Ready((PyTypeObject*)this);
			Type = this;
		}

		static PyObject* create(const T* value) {
			Opaque* self = (Opaque*)Type->tp_alloc(Type, 0);
			self->object = value;
			return (PyObject*) self;
		}

		static T* get(PyObject* o) {
			return (T*)(((Opaque)o)->object);
		}

};

template <class T>
OpaqueType<T>* OpaqueType<T>::Type = 0;


#endif
