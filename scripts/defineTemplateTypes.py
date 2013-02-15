# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.

import re

def getPyClass(name):
	name = name[4:] # remove Wt::
	return "Py%s" % name

def defineTemplateTypes(classlist):

	# collect all overloads
	ol = []
	for c in classlist:
		if c.constructor: ol += c.constructor.ol
		for m in c.methods.values():
			ol += m.ol


	# collect all types
	alltypes = []
	for o in ol:
		if o.retval != "": alltypes.append(o.retval.strip())
		for a in o.args.args:
			alltypes.append(a[0].strip())


	#remove const, * and &
	for i in range(len(alltypes)):
		t = alltypes[i]
		if t[0:5] == 'const': t = t[5:].strip()
		if t[-5:] == 'const': t = t[:-5].strip()
		if t[-1] == '&': t = t[:-1].strip()
		if t[-1] == '*': t = t[:-1].strip()
		alltypes[i] = t

	alltypes = set(alltypes)

	# types to keep
	ttypes = []
	for t in alltypes:
		if t.find('<') == -1: continue

		if t.find('Signal') == -1: continue

		t = re.sub(', struct Wt::NoClass', '', t)

		typedict = {
			'cname' : t,                  # Wt::Signal<bool>
			'pyclass' : getPyClass(t)     # PySignal<bool>
		}

		ttypes.append(typedict)


	return ttypes

		


