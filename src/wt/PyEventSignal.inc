// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#include <Wt/WJavaScriptSlot>
#include "Arg.h"
#include "g_runmethod.inc"

template <class T>

		PyObject* PyEventSignal<T>::s_connect(PyCPPClassInstance* self, PyObject* args, PyObject* kwargs) {
			Wt::EventSignal<T>* me = PyCPPClassType< Wt::EventSignal<T> >::getCore(self);

			const char* anames[] = {"a1", 0};
			ARGS_1_0D("O:connect", (char**)anames, PyObject*);

			// TODO: did not compile for some reason
			if (a1c.v->ob_type == PyCPPClassType<Wt::JSlot>::Type) {
				Wt::JSlot* jslot = PyCPPClassType<Wt::JSlot>::getCore((PyCPPClassInstance*)a1c.v);
				me->connect(*jslot);
			}
			else {
				SlotConnectorBase* conn = SlotConnectorFactory<Wt::EventSignal<T>, T>::createConnector(me, a1c.v);
				if (PyErr_Occurred()) {
					delete conn;
					return 0;
				}
				conn->connect();
			}
			Py_RETURN_NONE;
		}
