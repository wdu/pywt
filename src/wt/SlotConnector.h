// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef SLOTCONNECTOR
#define SLOTCONNECTOR

#include <Python.h>
#include <Wt/WObject>
#include <Wt/WSignal>
#include <Wt/WWidget>
#include <Wt/WTableCell>

template <class A1=Wt::NoClass, class A2=Wt::NoClass, 
		  class A3=Wt::NoClass, class A4=Wt::NoClass, 
		  class A5=Wt::NoClass, class A6=Wt::NoClass>
class SC_traits {
	public: const static int n_signal_args = 6;
};

template <class A1, class A2, class A3, class A4, class A5>
class SC_traits<A1, A2, A3, A4, A5, Wt::NoClass> {
	public: const static int n_signal_args = 5;
};


template <class A1, class A2, class A3, class A4>
class SC_traits<A1, A2, A3, A4, Wt::NoClass, Wt::NoClass> {
	public: const static int n_signal_args = 4;
};

template <class A1, class A2, class A3>
class SC_traits<A1, A2, A3, Wt::NoClass, Wt::NoClass, Wt::NoClass> {
	public: const static int n_signal_args = 3;
};

template <class A1, class A2>
class SC_traits<A1, A2, Wt::NoClass, Wt::NoClass, Wt::NoClass, Wt::NoClass> {
	public: const static int n_signal_args = 2;
};


template <class A1>
class SC_traits<A1, Wt::NoClass, Wt::NoClass, Wt::NoClass, Wt::NoClass, Wt::NoClass> {
	public: const static int n_signal_args = 1;
};

template <>
class SC_traits<Wt::NoClass, Wt::NoClass, Wt::NoClass, Wt::NoClass, Wt::NoClass, Wt::NoClass> {
	public: const static int n_signal_args = 0;
};


template <>
class SC_traits<void, Wt::NoClass, Wt::NoClass, Wt::NoClass, Wt::NoClass, Wt::NoClass> {
	public: const static int n_signal_args = 0;
};



class SlotConnectorBase: public Wt::WObject {
	protected:
		PyObject* function;
		PyObject* klass;
		PyCFunction cfunction;
		long flags;
		Wt::WObject* core;
		int n_slot_args;

	public:
		SlotConnectorBase(PyObject* slot, int n_sig_args); 

		void doCall(PyObject* args);

		virtual ~SlotConnectorBase() {
			Py_XDECREF(klass);
			Py_XDECREF(function);
		}

		virtual void connect() {
			if (core) {
				core->destroyed().connect(SLOT(this, SlotConnectorBase::instanceDestroyed));
			}
		}

		void instanceDestroyed() {
			delete this;
		}

};

#include "ConversionBase.h"

template <>
class C2Python<Wt::NoClass> {
	public: static PyObject* arg(Wt::NoClass) {return 0;}
};

template <>
class Python2C<Wt::NoClass> {
	public: static Wt::NoClass arg(PyObject*) {return Wt::NoClass::none;}
};

template <class SIGNAL,
		  class A1=Wt::NoClass, class A2=Wt::NoClass, 
		  class A3=Wt::NoClass, class A4=Wt::NoClass, 
		  class A5=Wt::NoClass, class A6=Wt::NoClass> 
class SlotConnector: public SlotConnectorBase {
	protected:
		SIGNAL* _signal;
	public:
		SlotConnector(SIGNAL* s, PyObject* slot): SlotConnectorBase(slot,
				SC_traits<A1, A2, A3, A4, A5, A6>::n_signal_args),
		_signal(s) {

		};

		void call(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {
			PyObject* args = PyTuple_New(n_slot_args);
			if (n_slot_args >= 1) PyTuple_SetItem(args, 0, C2Python<A1>::arg(a1));
			if (n_slot_args >= 2) PyTuple_SetItem(args, 1, C2Python<A2>::arg(a2));
			if (n_slot_args >= 3) PyTuple_SetItem(args, 2, C2Python<A3>::arg(a3));
			if (n_slot_args >= 4) PyTuple_SetItem(args, 3, C2Python<A4>::arg(a4));
			if (n_slot_args >= 5) PyTuple_SetItem(args, 4, C2Python<A5>::arg(a5));
			if (n_slot_args >= 6) PyTuple_SetItem(args, 5, C2Python<A6>::arg(a6));
			doCall(args);
		}

		void connect() {
			typedef SlotConnector<SIGNAL, A1, A2, A3, A4, A5, A6> thisType;
			_signal->connect(SLOT(this, thisType::call));
			SlotConnectorBase::connect();
		}
};


/* Specialization for EventSignal, which expects a call with only one parameter */

template <class SIGNAL, class A1>
class SlotConnector<SIGNAL, A1>: public SlotConnectorBase {
	protected:
		SIGNAL* _signal;
	public:
		SlotConnector(SIGNAL* s, PyObject* slot): SlotConnectorBase(slot,
				SC_traits<A1>::n_signal_args),
												_signal(s) {
		};

		void call(A1 a1) {
			PyObject* args = PyTuple_New(n_slot_args);
			if (n_slot_args >= 1) PyTuple_SetItem(args, 0, C2Python<A1>::arg(a1));
			doCall(args);
		}

		void connect() {
			typedef SlotConnector<SIGNAL, A1> thisType;
			_signal->connect(SLOT(this, thisType::call));
			SlotConnectorBase::connect();
		}
};

template <class SIGNAL>
class SlotConnector<SIGNAL, void>: public SlotConnectorBase {
	protected:
		SIGNAL* _signal;
	public:
		SlotConnector(SIGNAL* s, PyObject* slot): SlotConnectorBase(slot,
				SC_traits<void>::n_signal_args),
												_signal(s) {
		};

		void call() {
			PyObject* args = PyTuple_New(0);
			doCall(args);
		}

		void connect() {
			typedef SlotConnector<SIGNAL, void> thisType;
			_signal->connect(SLOT(this, thisType::call));
			SlotConnectorBase::connect();
		}
};


#endif
