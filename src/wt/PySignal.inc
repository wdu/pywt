// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#include "Proxy.h"
#include "Threading.h"

template <class A1, class A2, class A3, class A4, class A5, class A6>
void PySignal<A1, A2, A3, A4, A5, A6>::instanceConstructor(PyCPPClassInstance* inst, PyObject* args, PyObject* kwargs, int) {
	try {
		const char* argnames[] = {"sender", 0};
		ARGS_1_1D("|O:constructor", (char**)argnames, Wt::WObject*, 0);
		Wt::Signal<A1, A2, A3, A4, A5, A6>* rv = new Wt::Signal<A1, A2, A3, A4, A5, A6>(a1c.v);
		inst->proxy = Proxy<Wt::Signal<A1, A2, A3, A4, A5, A6 > >::create(inst, rv, AbstractProxy::OWNED_BY_PYTHON);

		//inst->core = rv;
		//inst->setup(rv, PyCPPClassInstance::DEFAULT_INST_FROMPYTHON);
		if (NONZERO(a1c.v)) inst->setCOwnership();
	}
	CATCHALL()
}



template <class A1, class A2, class A3, class A4, class A5, class A6>
PyObject* PySignal<A1, A2, A3, A4, A5, A6>::s_connect(PyCPPClassInstance* self, PyObject* args, PyObject* kwargs) {
	Wt::Signal<A1, A2, A3, A4, A5, A6>* me = PyCPPClassType<Wt::Signal<A1, A2, A3, A4, A5, A6> >::getCore(self);
	const char* anames[] = {"a1", 0};
	ARGS_1_0D("O:connect", (char**)anames, PyObject*);
		SlotConnectorBase* conn = SlotConnectorFactory<Wt::Signal<A1, A2, A3, A4, A5, A6>, A1, A2, A3, A4, A5, A6 >::createConnector(me, a1c.v);
		if (PyErr_Occurred()) {
			delete conn;
			return 0;
		}
		conn->connect();
	Py_RETURN_NONE;
}


template <class A1, class A2, class A3, class A4, class A5, class A6>
PyObject* PySignal<A1, A2, A3, A4, A5, A6>::s_emit(PyCPPClassInstance* self, PyObject* args, PyObject* kwargs) {
	int nargs = SC_traits<A1, A2, A3, A4, A5, A6>::n_signal_args;

	Wt::Signal<A1, A2, A3, A4, A5, A6>* me = PyCPPClassType<Wt::Signal<A1, A2, A3, A4, A5, A6> >::getCore(self);

	if (kwargs && PyDict_Size(kwargs)) {
		PyErr_SetString(PyExc_RuntimeError, "Keyword arguments are not supported for emit");
		return 0;
	}
	PyObject* a1p = 0;
	PyObject* a2p = 0;
	PyObject* a3p = 0;
	PyObject* a4p = 0;
	PyObject* a5p = 0;
	PyObject* a6p = 0;
	if (!PyArg_UnpackTuple(args, "emit", nargs, nargs, &a1p, &a2p, &a3p, &a4p, &a5p, &a6p)) {
		return 0;
	}
	A1 a1c = Python2C<A1>::arg(a1p);
	A2 a2c = Python2C<A2>::arg(a2p);
	A3 a3c = Python2C<A3>::arg(a3p);
	A4 a4c = Python2C<A4>::arg(a4p);
	A5 a5c = Python2C<A5>::arg(a5p);
	A6 a6c = Python2C<A6>::arg(a6p);

	START_CPPCALL
	{ me->emit(a1c, a2c, a3c, a4c, a5c, a6c);}
	END_CPPCALL

	Py_RETURN_NONE;
}

