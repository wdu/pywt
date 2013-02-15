// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */





#ifndef PYTHONAPPLICATION
#define PYTHONAPPLICATION


#include <Python.h>
#include <Wt/WApplication>
#include "WtHook.h"

namespace Wt {
	class WEvent;
	class WEnvironment;
};


class PythonApplication: public Wt::WApplication {

	protected:
		PyThreadState* _threadstate;


	public:
		static PyObject* _appCreatorFunc;
		static PyObject* _wtModule;
		static WtHook hook;
		PyObject* _pyapp;
		static PythonApplication* instance() {
			return static_cast<PythonApplication*>(Wt::WApplication::instance());
		}


		PythonApplication(const Wt::WEnvironment& env);


		virtual ~PythonApplication();

		void setThreadState(PyThreadState* t) {_threadstate = t;}

		void notify(const Wt::WEvent& event);

		static void createErrorPage(Wt::WApplication* app, const std::string& message);
		static Wt::WApplication* handleError(Wt::WApplication* app, const Wt::WEnvironment* env, const std::string& message, bool show);

		static void setAppCreatorFunc(PyObject* creator);
		static void setWtModule(PyObject* m);
		//static void loadWtModule();

		static WApplication* createApplication(const Wt::WEnvironment& env);




};















#endif
