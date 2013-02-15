// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#define MODPREFIX "Wt."

#include <Python.h>
#include "Typedefs.h"


#include <Wt/WObject>
#include "Enum.h"

#include "gwt_Conversion.h"

#include "WtProxy.h"

#include "gwt_includeTypes.h"
#include "includeTemplateTypes.h"
//#include "OpaqueTypes.h"
//#include "gwt_EnumConversions.h"

#include "includeTemplateWrapperImpl.h"
#include "PythonApplication.h"

#include "PyJSignalTypeProvider.h"

#include "TypeMacros.h"

static PyObject* wt_destroy(PyObject *self, PyObject *args) {	
	PyObject* obj = 0;
	if (PyArg_ParseTuple(args, "O", &obj)) {

		if (PyObject_IsInstance(obj, (PyObject*)PyCPPClassType<Wt::WWidget>::Type)) {

			// because the class is WWidget, we know that the object is a PyCPPClassInstance
			PyCPPClassInstance* instance = (PyCPPClassInstance*)obj;
			instance->proxy->deleteCore();


			Py_RETURN_NONE;
		}

		else {
			PyErr_SetString(PyExc_TypeError, "Can only destroy objects of type WWidget");
			return 0;
		}
	}
	return 0;
}

static PyMethodDef WtMethods[] = {
	    {"destroy",  wt_destroy, METH_VARARGS,
			      "destroys the underlying C++ object contained by the wrapper"},
		    {NULL, NULL, 0, NULL}        /* Sentinel */
};


PyObject* init() {
	PyObject *m;

	m = Py_InitModule("Wt", WtMethods);
	if (m == NULL) return 0;

	PythonApplication::setWtModule(m);


#include "gwt_TypeDecl.h"
#include "TemplateTypes.h"
#include "gwt_includeEnums.h"

	PyType_Ready(&JSignalTypeProviderType);
	PyObject_SetAttrString(m, "JSignal", (PyObject*)PyObject_New(JSignalTypeProviderObject, &JSignalTypeProviderType));

	return m;
}



