// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */



extern PyObject* wlength_repr(PyCPPClassInstance* o);

extern PyObject* wlength_mult(PyCPPClassInstance*, PyCPPClassInstance*);
extern PyObject* wlength_div(PyCPPClassInstance*, PyCPPClassInstance*);
//static PyObject* wlength_ipmult(PyObject*, PyObject*);

static PyNumberMethods WLength_AsNumber = {
	0,      // nb_add
	0,      // nb_subtract
	(binaryfunc)wlength_mult,      // nb_multiply
	(binaryfunc)wlength_div,      // nb_divide
	0,      // nb_remainder
	0,      // nb_divmod
	0,      // nb_power
	0,      // nb_negative
	0,      // nb_positive
	0,      // nb_absolute
	0,      // nb_nonzero
	0,       // nb_invert
	0,      // nb_lshift
	0,      // nb_rshift
	0,       // nb_and
	0,      // nb_xor
	0,        // nb_or
	0,        // nb_coerce
	0,     // nb_int
	0,      // nb_long
	0,      // nb_float
	0,      // nb_oct
	0,      // nb_hex
	/* Added in release 2.0 */
	0,      // nb_inplace_add
	0,      // nb_inplace_subtract
	0,      // nb_inplace_multiply
	0,      // nb_inplace_divide
	0,      // nb_inplace_remainder
	0,      // nb_inplace_power
	0,      // nb_inplace_lshift
	0,      // nb_inplace_rshift
	0,      // nb_inplace_and
	0,      // nb_inplace_xor
	0,      // nb_inplace_or
	/* Added in release 2.2 */
	/* The following require the Py_TPFLAGS_HAVE_CLASS flag */
	0,      // nb_floor_divide
	0,      // nb_true_divide
	0,      // nb_inplace_floor_divide
	0,      // nb_inplace_true_divide
	/* Added in release 2.5 */
	0,      // nb_index
};


/*
template <>
class Python2C<const Wt::WLength&> {
	public: static Wt::WLength arg(PyObject* e) {
				if (e == Py_None) return Wt::WLength();
				if (PyNumber_Check(e)) {
					PyObject* floatobj = PyNumber_Float(e);
					if (!floatobj) return Wt::WLength();
					Wt::WLength r(PyFloat_AS_DOUBLE(floatobj));
					Py_DECREF(floatobj);
					return r;
				}
				if (!PyObject_IsInstance(e, (PyObject*)(PyCPPClassType<Wt::WLength>::Type))) {
					PyErr_Format(PyExc_TypeError, "Wrong argument type: got %s while expecting %s",  e->ob_type->tp_name,  PyCPPClassType<Wt::WLength>::Type->tp_name);
					return Wt::WLength();
				}
				PyCPPClassInstance* n = (PyCPPClassInstance*)e;
				return *(PyCPPClassType<Wt::WLength>::getCore(n));
			}
	public: static Wt::WLength ret(PyObject* e) {
				return arg(e);
			}
};


template <>
class Python2C<Wt::WLength> {
	public: static Wt::WLength arg(PyObject* e) {
				return Python2C<const Wt::WLength&>::arg(e);
			}
	public: static Wt::WLength ret(PyObject* e) {
				return arg(e);
			}
};


*/



