// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */





#include "PythonApplication.h"
#include <Wt/WText>
#include <Wt/WString>
#include <Wt/WEnvironment>
#include <Wt/WContainerWidget>

#include "PyCPPClassInstance.h"
#include "PyException.h"
#include "Threading.h"

using namespace Wt;

#include "Proxy.h"
template <>
class Proxy<Wt::WEnvironment>: public ProxyBase <Wt::WEnvironment, false> {};


PyObject* PythonApplication::_appCreatorFunc = 0;
PyObject* PythonApplication::_wtModule = 0;
WtHook PythonApplication::hook;


void PythonApplication::setAppCreatorFunc(PyObject* creator) {
	if (!PyCallable_Check(creator)) {
		PyErr_SetString(PyExc_RuntimeError, "Creator function must be callable");
		return;
	}
	Py_INCREF(creator);
	_appCreatorFunc = creator;
}


void PythonApplication::createErrorPage(Wt::WApplication* app, const std::string& message) {
	// clear the page. This is done by hiding the widgets, rather than deleting them,
	// as deleting may cause new python events, causing new errors
	app->root()->setLayout(0);
	for (int i = 0; i < app->root()->count(); ++i) {
		app->root()->widget(i)->hide();
	}

	app->setTitle("Python Error");
	new WText("<H1>Python Error</H1>", app->root());
	std::string msg2 = std::string("<pre>") + message + "</pre>";
	new WText(msg2, app->root());
}

Wt::WApplication* PythonApplication::handleError(Wt::WApplication* app, const Wt::WEnvironment* env, const std::string& message, bool show) {
	if (show) {
		if (app == 0) {
			app = new Wt::WApplication(*env);
		}	
		std::cout << "Python error:\n" << message << std::endl;
		createErrorPage(app, message);
		app->quit();
		return app;
	}
	else {
		throw std::runtime_error(message);
	}
}



#include <iostream>

WApplication* PythonApplication::createApplication(const WEnvironment& env) {
	PyThreadState* ts = 0;
std::cout << "Before OnCreateAppStart" << std::endl;
	OnCreateAppStart(ts);
std::cout << "After OnCreateAppStart" << std::endl;
	PythonApplication* pythonApp = 0;
	try {


		// create a wrapper for WEnvironment
		PyTypeObject* env_type = (PyTypeObject*)PyObject_GetAttrString(_wtModule, "WEnvironment");
		if (PyErr_Occurred()) throw PyException();
		PyCPPClassInstance* arg1 = (PyCPPClassInstance*)(env_type->tp_alloc(env_type, 0));
		Py_XDECREF(env_type);
		if (PyErr_Occurred()) throw PyException();
		arg1->proxy = Proxy<Wt::WEnvironment>::create(arg1, &env, AbstractProxy::OWNED_BY_C);


		// create an argumentlist
		PyObject* arglist = Py_BuildValue("(N)", arg1);

		// do the call
		PyObject* result = PyObject_CallObject(_appCreatorFunc, arglist);
		Py_DECREF(arglist);
		if (PyErr_Occurred()) throw PyException();


		// if there was no error, result should at least contain something
		assert(result);

		// check that the returned value is a WApplication
		PyObject* app_type = PyObject_GetAttrString(_wtModule, "WApplication");
		if (PyErr_Occurred()) throw PyException();
		if (!PyObject_IsInstance(result, app_type)) {
			PyErr_SetString(PyExc_RuntimeError,
				"Object returned by createApplication was not a WApplication");
			Py_XDECREF(app_type);
			throw PyException();
		}
		Py_XDECREF(app_type);

		// get the C++ PythonApplication object
		PyCPPClassInstance* app_inst = (PyCPPClassInstance*)result;
		pythonApp = (PythonApplication*)(app_inst->proxy->getCoreAsVoid());

		// let the application object hold the newly created threadstate
		pythonApp->setThreadState(ts);

		// ownership is set to C 
		app_inst->setCOwnership();

		// now we can release the python wrapper
		Py_DECREF(app_inst);
		if (PyErr_Occurred()) throw PyException();

		OnCreateAppEnd(ts);

		return pythonApp;
	}
	catch (PyException&) {
		assert(PyErr_Occurred());
		PyObject *exception = NULL, *value = NULL, *tb = NULL;
		PyErr_Fetch(&exception, &value, &tb);
		std::string msg = getExceptionMessage(exception, value, tb);
		OnCreateAppEnd(ts);
		return handleError(pythonApp, &env, msg, true);

	}

}

void PythonApplication::notify(const Wt::WEvent& event) {
	OnNotifyStart(_threadstate);
	try {
		Wt::WApplication::notify(event);
		OnNotifyEnd(_threadstate);
	}
	catch (PyException&) {
		std::cout << "Error occured during event handling" << std::endl;
		assert(PyErr_Occurred());
		PyObject *exception = NULL, *value = NULL, *tb = NULL;
		PyErr_Fetch(&exception, &value, &tb);
		std::string msg = getExceptionMessage(exception, value, tb);
		OnNotifyEnd(_threadstate);
		handleError(this, 0, msg, true);
	}
}




PythonApplication::PythonApplication(const Wt::WEnvironment& env): WApplication(env), _threadstate(0) {
}


PythonApplication::~PythonApplication() {
	OnDestructorStart(_threadstate);
	OnDestructorEnd(_threadstate);
}
	

void PythonApplication::setWtModule(PyObject* m) {
	_wtModule = m;
}




