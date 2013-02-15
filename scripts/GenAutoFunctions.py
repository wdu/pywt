# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.

n_methodargs = 9
n_methodwrapperargs = 9

import SmartFile
open = SmartFile.open


def argformatstring(tot, default):
	nondefault = tot - default
	r = "O" * nondefault
	if (default > 0): r = r + "|"
	r = r + "O" * default
	r = r + ":"
	return r


def template_list(n, prefixon = 1):
	r = ""
	prefix = ""
	if prefixon: prefix = ", "
	for j in range(n):
		 r = r+ "%sclass A%d" % (prefix, j+1)
		 prefix = ", "
	return r


def arg_list(n):
	r = ""
	prefix = ""
	for j in range(n):
		 r = r+ "%sA%d a%d" % (prefix, (j+1), (j+1))
		 prefix = ", "
	return r


def argref_list(n):
	r = ""
	prefix = ""
	for j in range(n):
		 r = r+ "%sA%d& a%d" % (prefix, (j+1), (j+1))
		 prefix = ", "
	return r


def argsign_list(n, prefixon = 0, template = "A%d"):
	r = ""
	prefix = ""
	if prefixon: prefix = ", "
	t = "%s" + template
	for j in range(n):
		 r = r+ t % (prefix, (j+1))
		 prefix = ", "
	return r

def argdefaultlist(n, nd):
	r = ""
	for i in range(n-nd, n):
		r = r + ", a%dd" % (i+1)
	return r




def write_runmethod(fname):
	f = open(fname, "w")
	indent = ""
	for i in range(n_methodwrapperargs+1):
		for d in range(i+1):
			f.write("#define ARGS_%d_%dD_CHECK(fname, aname%s) \\\n" % (i, d, argsign_list(i, 1, "t%d")))
			for j in range(i):
				f.write("\tPyObject* a%dp = 0;\\\n" % (j+1,))
			f.write("\tif (!PyArg_ParseTupleAndKeywords(args, kwargs, fname, aname%s)) {\\\n" %(argsign_list(i, 1, "&a%dp")))
			f.write("\t\tPyErr_Clear();\\\n")
			f.write("\t\targsok = false;\\\n")
			f.write("\t} \\\n")
			if i > 0:
				f.write("\tif ((argsok) && (\\\n")
				for j in range(i):
					if (j >= i-d):
						f.write("\t\t(a%dp && !Python2C<t%d>::acceptarg(a%dp)) ||\\\n" % (j+1, j+1, j+1))
					else:
						f.write("\t\t!Python2C<t%d>::acceptarg(a%dp) || \\\n" % (j+1, j+1))
				f.write("\t\tfalse)) {\\\n")
				f.write("\t\targsok = false;\\\n")
				f.write("\t} \\\n")
			f.write("\n\n")


			f.write("#define ARGS_%d_%dD_CONV(fname, aname%s%s) \\\n" % (i, d, argsign_list(i, 1, "t%d"), argdefaultlist(i, d)))
			for j in range(i):
				if (j >= i-d):
					#f.write("\tt%d a%dc = a%dp?Python2C<t%d>::arg(a%dp):(t%d)a%dd;\\\n" % (j+1, j+1, j+1, j+1, j+1, j+1, j+1))
					f.write("\tArg< t%d > a%dc(a%dp, a%dd);\\\n" % (j+1, j+1, j+1, j+1))
				else:
					#f.write("\tt%d a%dc = Python2C<t%d>::arg(a%dp);\\\n" % (j+1, j+1, j+1, j+1))
					f.write("\tArg< t%d > a%dc(a%dp);\\\n" % (j+1, j+1, j+1))
			f.write("\tif (PyErr_Occurred()) throw PyException();\n")
			f.write("\n\n")

			f.write("#define ARGS_%d_%dD(fname, aname%s%s) \\\n" % (i, d, argsign_list(i, 1, "t%d"), argdefaultlist(i, d)))
			for j in range(i):
				f.write("\tPyObject* a%dp = 0;\\\n" % (j+1,))
			f.write("\tif (!PyArg_ParseTupleAndKeywords(args, kwargs, fname, aname%s)) {\\\n" %(argsign_list(i, 1, "&a%dp")))
			f.write("\t\tthrow PyException();\\\n")
			f.write("\t} \\\n")
			for j in range(i):
				if (j >= i-d):
					#f.write("\tt%d a%dc = a%dp?Python2C<t%d>::arg(a%dp):(t%d)a%dd;\\\n" % (j+1, j+1, j+1, j+1, j+1, j+1, j+1))
					f.write("\tArg< t%d > a%dc(a%dp, a%dd);\\\n" % (j+1, j+1, j+1, j+1))
				else:
					#f.write("\tt%d a%dc = Python2C<t%d>::arg(a%dp);\\\n" % (j+1, j+1, j+1, j+1))
					f.write("\tArg< t%d > a%dc(a%dp);\\\n" % (j+1, j+1, j+1))
			f.write("\tif (PyErr_Occurred()) throw PyException();\n")
			f.write("\n\n")

	f.close()


def write_callPyMethod(fname):
	f = open(fname, "w")
	f.write("#include \"PyException.h\"\n\n")
	indent = ""
	for i in range(n_methodargs+1):
		f.write("%s/************************* %d Arguments ******************/\n"% (indent, i))
		f.write("%stemplate <class R%s>\n" % (indent, template_list(i)))
		comma = ""
		if (i > 0): comma = ", "
		f.write("%sinline R callPyMethod%d(PyObject* instance, const char* name%s%s) {\n" % (indent, i, comma, arg_list(i)))
		f.write("%s\tPyObject* f = PyObject_GetAttrString(instance, name);\n" % indent)
		f.write("%s\tif (PyErr_Occurred()) throw PyException();\n" % indent)
		for j in range(i):
			f.write("%s\tPyObject* a%dp = C2Python<A%d>::arg(a%d);\n" % (indent, j+1, j+1, j+1))
		f.write("%s\tPyObject* args = PyTuple_New(%d);\n" % (indent, i))
		for j in range(i):
			f.write("%s\tPyTuple_SetItem(args, %d, a%dp);\n" % (indent, j, j+1))
		f.write("%s\tPyObject* pyr = PyObject_Call(f, args, 0);\n" % indent)
		f.write("%s\tif (!pyr) {throw PyException();}\n"% indent)
		f.write("%s\tR r = Python2C<R>::ret(pyr);\n" % indent)
		f.write("%s\tPy_XDECREF(pyr);\n" % indent)
		f.write("%s\tPy_XDECREF(f);\n" % indent)
		f.write("%s\tPy_XDECREF(args);\n" % indent)
		f.write("%s\treturn r;\n" % indent)
		f.write("%s}\n\n\n" % indent)

		if i > 0: f.write("%stemplate <%s>\n" % (indent, template_list(i, 0)))
		comma = ""
		if (i > 0): comma = ", "
		f.write("%sinline void callPyMethodV%d(PyObject* instance, const char* name%s%s) {\n" % (indent, i, comma, arg_list(i)))
		f.write("%s\tPyObject* f = PyObject_GetAttrString(instance, name);\n" % indent)
		for j in range(i):
			f.write("%s\tPyObject* a%dp = C2Python<A%d>::arg(a%d);\n" % (indent, j+1, j+1, j+1))
		f.write("%s\tPyObject* args = PyTuple_New(%d);\n" % (indent, i))
		for j in range(i):
			f.write("%s\tPyTuple_SetItem(args, %d, a%dp);\n" % (indent, j, j+1))
		f.write("%s\tPyObject* pyr = PyObject_Call(f, args, 0);\n" % indent)
		f.write("%s\tif (!pyr) {throw PyException();}\n"% indent)
		f.write("%s\tPy_XDECREF(pyr);\n" % indent)
		f.write("%s\tPy_XDECREF(f);\n" % indent)
		f.write("%s\tPy_XDECREF(args);\n" % indent)
		f.write("%s}\n\n\n" % indent)
	f.close()



def write(adir):
	print "GenAutoFunctions"
	write_runmethod(adir + "/g_runmethod.inc")
	write_callPyMethod(adir + "/g_callPyMethod.inc");



