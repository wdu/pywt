// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#ifndef COMPLEXCONVERSION_H
#define COMPLEXCONVERSION_H

#include <ostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <boost/any.hpp>
#include "ConversionBase.h"
#include "Arg.h"
#include "CPPClassConversion.h"
#include "PyException.h"
#include "Typedefs.h"


//////////////////////////// Pair ////////////////////////////////

template <class K, class V>
class Python2C<const std::pair<K, V> &>{
	public: 
		static std::pair<K, V> arg(PyObject* v, std::pair<K, V>** r) {
			if (!PySequence_Check(v)) {
				PyErr_Format(PyExc_TypeError, 
				"Got a %s while expecting a sequence of size 2", v->ob_type->tp_name);
				throw PyException();
			}
			if (!PySequence_Size(v)) {
				PyErr_Format(PyExc_TypeError, 
				"Got a seqence of size %d while expecting a one of size 2", (int)PySequence_Size(v));
				throw PyException();
			}
			*r = new std::pair<K, V>(Arg<const K&>(PySequence_ITEM(v, 0)).v, Arg<const V&>(PySequence_ITEM(v, 1)).v);
			return **r;
		}
		typedef std::pair<K, V>  S;
};

//////////////////////////// Vector ////////////////////////////////
template <class T>
PyObject* vector2python_arg(const std::vector<T>& v) {
	int l = v.size();
	PyObject* r = PyList_New(l);
	typename std::vector<T>::const_iterator it;
	int index = 0;
	for (it = v.begin(); it != v.end(); ++it, ++index) {
		PyList_SET_ITEM(r, index, C2Python<T>::arg(*it));
	}
	return r;
}

template <class T>
PyObject* vector2python_ret(const std::vector<T>& v) {
	int l = v.size();
	PyObject* r = PyList_New(l);
	typename std::vector<T>::const_iterator it;
	int index = 0;
	for (it = v.begin(); it != v.end(); ++it, ++index) {
		PyList_SET_ITEM(r, index, C2Python<T>::ret(*it));
	}
	return r;
}


template <class T>
void python2vector_arg(PyObject* v, std::vector<T>& r) {
	if (!PySequence_Check(v)) {
		PyErr_Format(PyExc_TypeError, 
				"Got a %s while expecting a sequence", v->ob_type->tp_name);
		throw PyException();
	}
	int l = PySequence_Size(v);
	r.clear();
	r.reserve(l);
	for (int i = 0; i < l; i++) {
		PyObject* item = (PySequence_GetItem(v, i));
		r.push_back(Arg<T>(item).v);
		Py_DECREF(item);
	}
}


template <class T>
void python2vector_ret(PyObject* v, std::vector<T>& r) {
	if (!PySequence_Check(v)) {
		PyErr_Format(PyExc_TypeError, 
				"Got a %s while expecting a sequence", v->ob_type->tp_name);
		throw PyException();
	}
	int l = PySequence_Size(v);
	r.clear();
	r.reserve(l);
	for (int i = 0; i < l; i++) {
		PyObject* item = (PySequence_GetItem(v, i));
		r.push_back(Python2C<T>::ret(item));
		Py_DECREF(item);
	}
}


template <class T>
class Python2C<const std::vector< T >&> {
	public: 
		static const std::vector<T>& arg(PyObject* o, std::vector< T >** v) {
			*v = new std::vector<T>;
			python2vector_arg<T>(o, **v);
			return **v;
		}
		static bool acceptarg(PyObject* o) {
			return PySequence_Check(o) != 0;
		}
		typedef const std::vector<T>& S;
};

template <class T>
class Python2C<std::vector< T > > {
	public: 
		static std::vector<T> ret(PyObject* o) {
			std::vector<T> r;
			python2vector_ret<T>(o, r);
			return r;
		}

};


template <class T> 
class C2Python<std::vector<T> > {
	public: 
		static PyObject* arg(const std::vector<T>& v) {
			return vector2python_arg(v);
		}
		static PyObject* ret(const std::vector<T>& v) {
			return vector2python_ret(v);
		}
};


template <class T> 
class C2Python<const std::vector<T> > {
	public: 
		static PyObject* arg(const std::vector<T>& v) {
			return vector2python_arg(v);
		}
		static PyObject* ret(const std::vector<T>& v) {
			return vector2python_ret(v);
		}
};


template <class T> 
class C2Python<const std::vector<T>&> {
	public: 
		static PyObject* arg(const std::vector<T>& v) {
			return vector2python_arg(v);
		}
		static PyObject* ret(const std::vector<T>& v) {
			return vector2python_ret(v);
		}
};

/////////////////////////////// Map ///////////////////////////////////

template <class K, class V>
PyObject* map2python_arg(const std::map<K, V>& v) {
	PyObject* r = PyDict_New();
	for (typename std::map<K, V>::const_iterator it = v.begin(); it != v.end(); ++it) {
		PyObject* k = C2Python<const K&>::arg(it->first);
		PyObject* v = C2Python<const V&>::arg(it->second);
		PyDict_SetItem(r, k, v);
		Py_DECREF(k);
		Py_DECREF(v);
	}
	return r;
}


template <class K, class V>
PyObject* map2python_ret(const std::map<K, V>& v) {
	PyObject* r = PyDict_New();
	for (typename std::map<K, V>::const_iterator it = v.begin(); it != v.end(); ++it) {
		PyObject* k = C2Python<const K&>::ret(it->first);
		PyObject* v = C2Python<const V&>::ret(it->second);
		PyDict_SetItem(r, k, v);
		Py_DECREF(k);
		Py_DECREF(v);
	}
	return r;
}
template <class K, class V>
void python2dict_arg(PyObject* v, std::map<K, V>& r) {
	if (!PyDict_Check(v)) {
		PyErr_Format(PyExc_TypeError, 
				"Got a %s while expecting a dictionary", v->ob_type->tp_name);
		throw PyException();
	}
	r.clear();
	PyObject *key, *value;
	Py_ssize_t pos = 0;

	while (PyDict_Next(v, &pos, &key, &value)) {
		r.insert(std::pair<K, V>(Arg<const K&>(key).v, Arg<const V&>(value).v));
	}

}


template <class K, class V>
void python2dict_ret(PyObject* v, std::map<K, V>& r) {
	if (!PyDict_Check(v)) {
		PyErr_Format(PyExc_TypeError, 
				"Got a %s while expecting a dictionary", v->ob_type->tp_name);
		throw PyException();
	}
	r.clear();
	PyObject *key, *value;
	Py_ssize_t pos = 0;

	while (PyDict_Next(v, &pos, &key, &value)) {
		r.insert(std::pair<K, V>(Python2C<K>::ret(key), Python2C<V>::ret(value)));
	}

}
template <class K, class T>
class Python2C<const std::map< K, T >&> {
	public: 
		static std::map<K, T > arg(PyObject* o, std::map<K, T >** r) {
			*r = new std::map<K, T>;
			python2dict_arg<K, T>(o, **r);
			return **r;
		}
	
		static bool acceptarg(PyObject* o) {
			return PyDict_Check(o);
		}
		typedef std::map<K, T> S;

};


template <class K, class T, class C, class A>
class Python2C<std::map< K, T, C, A > > {
	public: static std::map<K, T, C, A > ret(PyObject* o) {
		std::map<K, T> r;
		python2dict_ret<K, T>(o, r);
		return r;
	}

};

/////////////////////////////// Set ///////////////////////////////////
template <class K>
class Python2C<const std::set< K >&> {
	public: 
		static const std::set<K>& arg(PyObject* o, std::set< K >** r) {
			if (!PyAnySet_Check(o)) {
				PyErr_Format(PyExc_TypeError, "Expected a set while getting a %s", o->ob_type->tp_name);
				throw PyException();
			}
			*r = new std::set<K>();
			PyObject* iter = PyObject_GetIter(o);
			PyObject* item;
			while (item = PyIter_Next(iter)) {
				(*r)->insert(Arg<K>(item).v);
				Py_DECREF(item);
			}
			Py_DECREF(iter);
			return **r;
		}
		static bool acceptarg(PyObject* o) {
			return PyAnySet_Check(o);
		}
		typedef std::set<K> S;

};


template <class K> 
class C2Python<const std::set<K>& > {
	public: 
		static PyObject* arg(const std::set<K>& v) {
			PyObject* l = PySet_New(0);
			for (typename std::set<K>::const_iterator it = v.begin(); it != v.end(); ++it) {
				PyObject* key = C2Python<K>::arg(*it);
				PySet_Add(l, key);
				Py_DECREF(key);
			}
			return l;
		}
		static PyObject* ret(const std::set<K>& v) {
			PyObject* l = PySet_New(0);
			for (typename std::set<K>::const_iterator it = v.begin(); it != v.end(); ++it) {
				PyObject* key = C2Python<K>::ret(*it);
				PySet_Add(l, key);
				Py_DECREF(key);
			}
			return l;
		}
};


template <class K> 
class C2Python<std::set<K> > {
	public: 
		static PyObject* ret(const std::set<K>& v) {
			return C2Python<const std::set<K>&>::ret(v);
		}
};


template <class K> 
class C2Python<const std::set<K> > {
	public: 
		static PyObject* ret(const std::set<K>& v) {
			return C2Python<const std::set<K>&>::ret(v);
		}
};



///////////////////////////////////////////////////////////////////////////////////
template <class K, class T, class C, class A> 
class C2Python<const std::map<K, T, C, A>& > {
	public: static PyObject* arg(const std::map<K, T, C, A>& v) {
			return map2python_arg<K, T>(v);
	}
	public: static PyObject* ret(const std::map<K, T, C, A>& v) {
			return map2python_ret<K, T>(v);
			}
};


template <class K, class T, class C, class A> 
class C2Python<std::map<K, T, C, A> > {

	public: static PyObject* ret(const std::map<K, T, C, A>& v) {
			return map2python_ret<K, T>(v);
			}
};

///////////////////////////////////////////////////////////////////////////////////


template <class K, class T, class C, class A> 
class C2Python<const std::multimap<K, T, C, A>& > {
	public: static PyObject* ret(const std::multimap<K, T, C, A>& v) {
			return 0;
			}
};

template <class K, class T>
class Python2C<const std::multimap< K, T >&> {
	public: 
		static std::multimap<K, T > arg(PyObject* o, std::multimap<K, T >** r) {
			*r = new std::multimap<K, T>;
//todo
			return **r;
		}
	
		static bool acceptarg(PyObject* o) {
			return false;
		}
		typedef std::multimap<K, T> S;

};


#endif
