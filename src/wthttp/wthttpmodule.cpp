// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */



#include <Python.h>
#include <Wt/WServer>
#include "WtHook.h"

static PyObject *wt_wrun(PyObject *self, PyObject *pyargs)
{

	// Import Wt, if not yet done
	PyObject* wtmodule = PyImport_ImportModule("Wt");
	if (!wtmodule) return 0;

	// get the __hook__ object from the Wt module
	PyObject* pyhook = PyDict_GetItemString(PyModule_GetDict(wtmodule), "__hook__");
	if (!pyhook) return 0;

	// extract the underlying C struct
	WtHook hook = *(WtHook*)PyCObject_AsVoidPtr(pyhook);

	PyObject* creator = 0;
	PyObject* args = 0;

	// Extract the 2 arguments, which must be a callable and a list of strings
	// (the command line arguments)
	// creator and args are borrowed references
	if (!PyArg_ParseTuple(pyargs, "OO", &args, &creator)) {
		PyErr_SetString(PyExc_RuntimeError, "2 arguments expected");
		return 0;
	}


	// check that the second argument is a list
	if (!PyList_Check(args)) {
		PyErr_SetString(PyExc_TypeError, "second argument must be a list of strings");
		return 0;
	}

	// check that every element in the list is a string
	// and copy the char* pointer into pyargv. Note that
	// the pointer are valid as long as the string objects
	// exist
	int pyargc = PyList_Size(args);
	char** pyargv = new char*[pyargc];

	for (int i = 0; i<pyargc; ++i) {
		PyObject* item = PyList_GetItem(args, i);
		if (!PyString_Check(item)) {
			PyErr_SetString(PyExc_TypeError, "second argument must be a list of strings");
			delete [] pyargv;
			return 0;
		}
		pyargv[i] = PyString_AsString(item);
	}

	// keep a reference to the first argument
	(*hook.setAppCreatorFunc)(creator);


	// Initialize thread system, and release the current 
	// thread state, so that other thread may start
	(*hook.initThreading)();

	// Start the server
	int r = WRun(pyargc, pyargv, hook.createApplication);

	// delete allocated memory
	delete [] pyargv;

	// re-acquire the main thread state
	(*hook.finalizeThreading)();

	return Py_BuildValue("i", r);
}

static PyMethodDef WtHttpdMethods[] = {
	{"WRun",  wt_wrun, METH_VARARGS,
      "Start the HTTP server"},
	{NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initWtHttp(void)
{
	Py_InitModule("WtHttp", WtHttpdMethods);
}



