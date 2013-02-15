// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */





#include "PyException.h"
#include <assert.h>
#include <cStringIO.h>
#include <string>

	static int
parse_syntax_error(PyObject *err, PyObject **message, const char **filename,
		int *lineno, int *offset, const char **text)
{
	long hold;
	PyObject *v;

	/* old style errors */
	if (PyTuple_Check(err))
		return PyArg_ParseTuple(err, "O(ziiz)", message, filename,
				lineno, offset, text);

	/* new style errors.  `err' is an instance */

	if (! (v = PyObject_GetAttrString(err, "msg")))
		goto finally;
	*message = v;

	if (!(v = PyObject_GetAttrString(err, "filename")))
		goto finally;
	if (v == Py_None)
		*filename = NULL;
	else if (! (*filename = PyString_AsString(v)))
		goto finally;

	Py_DECREF(v);
	if (!(v = PyObject_GetAttrString(err, "lineno")))
		goto finally;
	hold = PyInt_AsLong(v);
	Py_DECREF(v);
	v = NULL;
	if (hold < 0 && PyErr_Occurred())
		goto finally;
	*lineno = (int)hold;

	if (!(v = PyObject_GetAttrString(err, "offset")))
		goto finally;
	if (v == Py_None) {
		*offset = -1;
		Py_DECREF(v);
		v = NULL;
	} else {
		hold = PyInt_AsLong(v);
		Py_DECREF(v);
		v = NULL;
		if (hold < 0 && PyErr_Occurred())
			goto finally;
		*offset = (int)hold;
	}

	if (!(v = PyObject_GetAttrString(err, "text")))
		goto finally;
	if (v == Py_None)
		*text = NULL;
	else if (! (*text = PyString_AsString(v)))
		goto finally;
	Py_DECREF(v);
	return 1;

finally:
	Py_XDECREF(v);
	return 0;
}


	static void
print_error_text(PyObject *f, int offset, const char *text)
{
	const char *nl;
	if (offset >= 0) {
		if (offset > 0 && offset == (int)strlen(text))
			offset--;
		for (;;) {
			nl = strchr(text, '\n');
			if (nl == NULL || nl-text >= offset)
				break;
			offset -= (int)(nl+1-text);
			text = nl+1;
		}
		while (*text == ' ' || *text == '\t') {
			text++;
			offset--;
		}
	}
	PyFile_WriteString("    ", f);
	PyFile_WriteString(text, f);
	if (*text == '\0' || text[strlen(text)-1] != '\n')
		PyFile_WriteString("\n", f);
	if (offset == -1)
		return;
	PyFile_WriteString("    ", f);
	offset--;
	while (offset > 0) {
		PyFile_WriteString(" ", f);
		offset--;
	}
	PyFile_WriteString("^\n", f);
}



std::string getExceptionMessage(PyObject* _type, PyObject* _value, PyObject* _trace) {
	PycString_IMPORT;
	PyObject* f = PycStringIO->NewOutput(10000);
	
	PyErr_NormalizeException(&_type, &_value, &_trace);
	std::string  msg = "";
	int err = 0;
	PyObject* value = _value;
	Py_XINCREF(value);

	assert(_type);

	if (_trace && _trace != Py_None) {
		err = PyTraceBack_Print(_trace, f);
	}

	if (err == 0 && PyObject_HasAttrString(_value, "print_file_and_line")) {
		PyObject *message;
		const char *filename, *text;
		int lineno, offset;
		if (!parse_syntax_error(_value, &message, &filename,
					&lineno, &offset, &text))
			PyErr_Clear();
		else {
			char buf[10];
			PyFile_WriteString("  File \"", f);
			if (filename == NULL) PyFile_WriteString("<string>", f);
			else PyFile_WriteString(filename, f);
			PyFile_WriteString("\", line ", f);
			PyOS_snprintf(buf, sizeof(buf), "%d", lineno);
			PyFile_WriteString(buf, f);
			PyFile_WriteString("\n", f);
			if (text != NULL) print_error_text(f, offset, text);
			Py_DECREF(_value);
			value = message;
			/* Can't be bothered to check all those
			 *                    PyFile_WriteString() calls */
			if (PyErr_Occurred()) err = -1;
		}
	}
	if (err) {
		/* Don't do anything else */
	}

	else if (PyExceptionClass_Check(_type)) {
		PyObject* moduleName;
		char* className = PyExceptionClass_Name(_type);
		if (className != NULL) {
			char *dot = strrchr(className, '.');
			if (dot != NULL)
				className = dot+1;
		}

		moduleName = PyObject_GetAttrString(_type, "__module__");
		if (moduleName == NULL)
			err = PyFile_WriteString("<unknown>", f);
		else {
			char* modstr = PyString_AsString(moduleName);
			if (modstr && strcmp(modstr, "exceptions"))
			{
				err = PyFile_WriteString(modstr, f);
				err += PyFile_WriteString(".", f);
			}
			Py_DECREF(moduleName);
		}
		if (err == 0) {
			if (className == NULL)
				err = PyFile_WriteString("<unknown>", f);
			else
				err = PyFile_WriteString(className, f);
		}
	}
	else err = PyFile_WriteObject(_type, f, Py_PRINT_RAW);
	if (err == 0 && (value != Py_None)) {
		PyObject *s = PyObject_Str(value);
		/* only print colon if the str() of the
		 *                object is not the empty string
		 *                            */
		if (s == NULL)
			err = -1;
		else if (!PyString_Check(s) ||
				PyString_GET_SIZE(s) != 0)
			err = PyFile_WriteString(": ", f);
		if (err == 0)
			err = PyFile_WriteObject(s, f, Py_PRINT_RAW);
		Py_XDECREF(s);
	}
	if (err == 0) err = PyFile_WriteString("\n", f);
	Py_DECREF(value);
	/* If an error happened here, don't show it.
	 *        XXX This is wrong, but too many callers rely on this behavior. */
	if (err != 0) PyErr_Clear();
	PyObject* msgo = PycStringIO->cgetvalue(f);
	msg = PyString_AsString(msgo);
	Py_DECREF(msgo);
	Py_DECREF(f);

	return msg;
}


