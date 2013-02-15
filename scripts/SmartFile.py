# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.

import tempfile
import filecmp
import os


ropen = open

def copyfile(src, dest):
	f = ropen(src)
	lines = f.readlines()
	f.close()
	f = ropen(dest, 'w')
	for l in lines: f.write(l)
	f.close()

class File:
	def __init__(self, name):
		self.name = name
		self.tmpname = tempfile.mktemp()
		self.f = ropen(self.tmpname, 'w')
		self.f.write("""// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2013 Emweb bvba, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
// This is an automatically generated file. Do not edit.

""");

	def write(self, content): self.f.write(content)

	def close(self):
		if self.f == None: return
		self.f.close()
		equal = False
		try:
			equal = filecmp.cmp(self.tmpname, self.name)
		except:
			pass # self.name does not yet exist ->equal = False
		if not equal:
			copyfile(self.tmpname, self.name)
		os.unlink(self.f.name)
		self.f = None


	def __del__(self): self.close()


def open(name, mode='r'):
	if mode != 'w': return __builtin__.open(name, mode)
	return File(name)
