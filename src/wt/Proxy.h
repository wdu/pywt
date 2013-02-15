// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef PROXY_H
#define PROXY_H

#include "AbstractProxy.h"
class PyCPPClassInstance;

template <class CORE, bool deletable>
class Delete {
	public: static void del(CORE* c) {delete c;}
};

template <class CORE> 
class Delete<CORE, false> {
	public: static void del(CORE* c) {}
};

template <class CORE, bool deletable = true> 
class ProxyBase: public AbstractProxy {

	protected:
		CORE* core;
		PyCPPClassInstance* pyobj;
		char flags;

	public:
		ProxyBase(PyCPPClassInstance* inst, const CORE* c, int flags):
			core(const_cast<CORE*>(c)), pyobj(inst), flags(flags) {}

		static AbstractProxy* create(PyCPPClassInstance* inst, const CORE* c, int flags) {
			return new ProxyBase<CORE, deletable>(inst, c, flags);
		}
		void deleteCore() {
			Delete<CORE, deletable>::del(core);
			core = 0;
		}

		void* getCoreAsVoid() const {
			return core;
		}

		bool isOwnedByPython() const {
			return (flags & OWNED_BY_PYTHON) != 0;
		}

		bool isDead() const {
			return (flags & DEAD) != 0;
		}

		bool isSubClassInstance() const {
			return (flags & SUBCLASS_INSTANCE) != 0;
		}

		void setPythonOwnership(bool v) {
			if (v) flags |= OWNED_BY_PYTHON;
			else flags &= ~OWNED_BY_PYTHON;
		}

		void deletedByC() {
			core = 0;
			flags |= DEAD;
			setPythonOwnership(true);
		}

		inline PyCPPClassInstance* getPyObj() const {
			return pyobj;
		}
		inline CORE* getCore() const {
			return core;
		}
};

#include <boost/signal.hpp>
#include <Wt/WSignal>
template <class CORE, bool deletable = true> 
class ProxyWObject: public ProxyBase<CORE, deletable> {
	protected:
	    boost::signals::connection conn;

	public:
		using ProxyBase<CORE, deletable>::getPyObj;
		using ProxyBase<CORE, deletable>::getCore;
		using ProxyBase<CORE, deletable>::deletedByC;
		using ProxyBase<CORE, deletable>::isDead;

		ProxyWObject(PyCPPClassInstance* inst, const CORE* c, int flags):
			ProxyBase<CORE, deletable>(inst, c, flags) {
			if (static_cast<Wt::WObject*>(ProxyBase<CORE, deletable>::core)->hasParent()) {
				flags &= ~AbstractProxy::OWNED_BY_PYTHON;
				Py_INCREF(inst);
			}
			else {
				flags |= AbstractProxy::OWNED_BY_PYTHON; 
			}
			conn = getCore()->Wt::WObject::destroyed().connect(this, &ProxyWObject<CORE, deletable>::destroyed);
		}

		virtual ~ProxyWObject() {
			conn.disconnect();
		}

		static AbstractProxy* create(PyCPPClassInstance* inst, const CORE* c, int flags) {
			return new ProxyWObject<CORE, deletable>(inst, c, flags);
		}
		virtual bool isOwnedByPython() const {
			if (isDead()) return false;
			return !(static_cast<Wt::WObject*>(ProxyBase<CORE, deletable>::core)->hasParent());
		}

		void setPythonOwnership(bool v) {
			if (this->isSubClassInstance()) {
				if (v && !(ProxyBase<CORE, deletable>::flags & AbstractProxy::OWNED_BY_PYTHON)) {
					Py_DECREF(getPyObj());
				}
				else if ((!v) && (ProxyBase<CORE, deletable>::flags & AbstractProxy::OWNED_BY_PYTHON)) {
					Py_INCREF(getPyObj());
				}
			}
			ProxyBase<CORE, deletable>::setPythonOwnership(v);
		}


		void destroyed(Wt::WObject* obj) {
			assert(obj == getCore());
			deletedByC();
		}
	

};



#include <Wt/WStandardItem>
template <class CORE, bool deletable = true> 
class ProxyWStandardItem: public ProxyBase<CORE, deletable> {

	public:
		using ProxyBase<CORE, deletable>::getPyObj;
		using ProxyBase<CORE, deletable>::getCore;
		using ProxyBase<CORE, deletable>::deletedByC;
		using ProxyBase<CORE, deletable>::isDead;

		ProxyWStandardItem(PyCPPClassInstance* inst, const CORE* c, int flags):
			ProxyBase<CORE, deletable>(inst, c, flags) {
			if (static_cast<Wt::WStandardItem*>(ProxyBase<CORE, deletable>::core)->model()) {
				flags &= ~AbstractProxy::OWNED_BY_PYTHON;
				Py_INCREF(inst);
			}
			else {
				flags |= AbstractProxy::OWNED_BY_PYTHON; 
			}
		}

		static AbstractProxy* create(PyCPPClassInstance* inst, const CORE* c, int flags) {
			return new ProxyWStandardItem<CORE, deletable>(inst, c, flags);
		}
		virtual bool isOwnedByPython() const {
			if (isDead()) return false;
			return !(static_cast<Wt::WStandardItem*>(ProxyBase<CORE, deletable>::core)->model());
		}

		void setPythonOwnership(bool v) {
			if (this->isSubClassInstance()) {
				if (v && !(ProxyBase<CORE, deletable>::flags & AbstractProxy::OWNED_BY_PYTHON)) {
					Py_DECREF(getPyObj());
				}
				else if ((!v) && (ProxyBase<CORE, deletable>::flags & AbstractProxy::OWNED_BY_PYTHON)) {
					Py_INCREF(getPyObj());
				}
			}
			ProxyBase<CORE, deletable>::setPythonOwnership(v);
		}



};





#include "PythonApplication.h"

class ProxyWApplication: public ProxyWObject<PythonApplication, true> {
	public:
		ProxyWApplication(PyCPPClassInstance* inst, const PythonApplication* c, int flags):
			ProxyWObject<PythonApplication, true>(inst, c, flags) {}

		static AbstractProxy* create(PyCPPClassInstance* inst, const PythonApplication* c, int flags) {
			return new ProxyWApplication(inst, c, flags);
		}

		bool isOwnedByPython() const {
			return false;
		}
};

template <class CORE>
class Proxy {
};

#include <istream>
#include <ostream>
template <>
class Proxy<std::ostream>: public ProxyBase<std::ostream> {};

template <>
class Proxy<std::istream>: public ProxyBase<std::istream> {};



#endif
