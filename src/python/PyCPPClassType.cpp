// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#include <Python.h>

#include "PyCPPClassType.h"
#include "PyException.h"
#include "PyCPPMethodWrapper.h" // for CATCHALL

#define B_OFFSETOF(field) ( (unsigned) (& ((PyCPPClassInstance*)1024)->field)-1024 )

static PyObject* baseArray2Tuple(const PyTypeObject** bases) {
	int count = 0;
	while (bases[count] != 0) count++; 
	PyObject* r = PyTuple_New(count);
	for (int i = 0; i < count; i++) {
		Py_INCREF(bases[i]);
		PyTuple_SET_ITEM(r, i, (PyObject*)(bases[i]));
	}
	return r;
}

static PyTypeObject PyCPPClassInstanceType = {
		PyObject_HEAD_INIT(0)
			0,                         /*ob_size*/
		"wt.ClassWrapper",             /*tp_name*/
		sizeof(PyCPPClassInstance),             /*tp_basicsize*/
		0,                         /*tp_itemsize*/
		(destructor)0, /*tp_dealloc*/
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
		Py_TPFLAGS_DEFAULT /* | Py_TPFLAGS_HAVE_GC*/, /*tp_flags*/
		"Class Wrapper object",           /* tp_doc */
		(traverseproc)0,   /* tp_traverse */
		(inquiry)0,           /* tp_clear */
		0,                     /* tp_richcompare */
		//B_OFFSETOF(weakreflist),                     /* tp_weaklistoffset */
		offsetof(PyCPPClassInstance, weakreflist),                     /* tp_weaklistoffset */
		0,                     /* tp_iter */
		0,                     /* tp_iternext */
		0,             /* tp_methods */
		0,             /* tp_members */
		0,                         /* tp_getset */
		0,                         /* tp_base */
		0,                         /* tp_dict */
		0,                         /* tp_descr_get */
		0,                         /* tp_descr_set */
		0,                         /* tp_dictoffset */
		(initproc)0,      /* tp_init */
		0,                         /* tp_alloc */
		0,                 /* tp_new */
	};


PyCPPClassTypeBase::PyCPPClassTypeBase(
		const char *name, const PyTypeObject** base): 
	baseconstructor(0),
	Factory(0),
	instanciatable(false) {
		memcpy (this, &PyCPPClassInstanceType, sizeof (PyTypeObject));
		tp_name = name;
		tp_dealloc = (destructor)s_dealloc;
		//tp_new = s_new;
		tp_hash = s_hash;

		tp_dict = PyDict_New();
		if (base) {
			tp_bases = baseArray2Tuple(base);
		}

}


void PyCPPClassTypeBase::checkCoreValid(PyCPPClassInstance* obj) {
	if (obj->proxy == 0) {
		PyErr_SetString(PyExc_RuntimeError, "Object is not initialized (did you forget to call __init__()?)");
		throw PyException();
	}
	if (obj->isDead()) {
		PyErr_SetString(PyExc_RuntimeError, "Object is dead");
		throw PyException();
	}
}




PyObject* PyCPPClassTypeBase::s_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
	PyCPPClassInstance *self;
	self = (PyCPPClassInstance *)type->tp_alloc(type, 0);
	return self;
}

long PyCPPClassTypeBase::s_hash(PyObject *obj) {
	return (long)obj;
}

int PyCPPClassTypeBase::s_init(PyCPPClassInstance *self, PyObject* args, PyObject* kwds, PyCPPClassTypeBase *mytype) {
	// instance of this type
	if (self->ob_type == mytype) {
		if (mytype->instanciatable) {
			try {
				mytype->baseconstructor(self, args, kwds, AbstractProxy::OWNED_BY_PYTHON);
				return 0;
			}
			CATCHALL();
			return -1;
		}
		else {
			PyErr_Format(PyExc_TypeError, "cannot create '%s' instances", mytype->tp_name);
		}
	}


	else {
		try {
			mytype->baseconstructor(self, args, kwds, AbstractProxy::OWNED_BY_PYTHON | AbstractProxy::SUBCLASS_INSTANCE);
			return 0;
		}
		CATCHALL();

		return -1;
	}
}

void PyCPPClassTypeBase::s_dealloc (PyCPPClassInstance * self) {

	if (self->proxy) { // if there was an exception in the constructor there is no proxy

		if (self->isOwnedByPython()) { // core may be 0 after exception in constructor
			self->proxy->deleteCore();
		}
		delete self->proxy;
	}
	if (self->weakreflist != NULL) PyObject_ClearWeakRefs(self);

	self->ob_type->tp_free (self);
}





