// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#include "Threading.h"

#if THREADMODE == THREADMODE_LOCKONNOTIFY
#include <Python.h>
PyThreadState* globalThreadState;
#endif

#if THREADMODE == THREADMODE_GIL
#include <Python.h>
PyThreadState* globalThreadState;
#endif

