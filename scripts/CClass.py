# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.

import string
import TypeMap
import DefaultMap
import sys


class Enum:
	def __init__(self, name, *values):
		self.name = name
		self.values = values
		self.namespace = ""
		self.fullname = name
		self.doclist = None

	def setDocumentation(self, doc):
		self.doc = doc

	def setNamespace(self, sp):
		self.namespace = sp + "::"
		self.fullname = sp + "::" + self.name

	def pyNamespace(self):
		import re
		pyspace = re.sub('::', '.', self.namespace)
		if pyspace[0] == '.': return pyspace[1:]
		return pyspace

	def setDocumentation(self, doclist):
		self.doclist = doclist

	def getDocumentation(self):
		if self.doclist == None: return [""] * (len(self.values)+1)
		return self.doclist



class ArgList:
	def __init__(self):
		self.args = []
	
	def add(self, arg):
		self.args.append(arg)

	def copy(self):
		r = ArgList()
		r.args = self.args[:]
		r.ndefaults = self.ndefaults
		return r

	def complete(self):
		for i in range(len(self.args)):
			arg = self.args[i]
			if (arg.__class__.__name__ == 'str'):
				type = arg
				name = "a%d" % (i+1,)
				attr = {}
				# hack to remove class
				#if type[0:5] == "class": type = type[6:]
				self.args[i] = (TypeMap.getTypename(type), name, attr)
			elif (arg.__class__.__name__ == 'tuple'):
				if (len(arg) > 3): print "Error"
				if (len(arg) == 0): print "Error"
				if (len(arg) == 1):
					type = arg[0]
					name = "a%d" % (i+1,)
					attr = {}
					# hack to remove class
					#if type[0:5] == "class": type = type[6:]
					self.args[i] = (TypeMap.getTypename(type), name, attr)
				elif len(arg) == 2:
					type = arg[0]
					if arg[1].__class__.__name__ == 'str':
						name = arg[1]
						attr = {}
					elif arg[1].__class__.__name__ == 'dict':
						name = "a%d" % (i+1,)
						attr =arg[1] 
					# hack to remove class
					#if type[0:5] == "class": type = type[6:]
					self.args[i] = (TypeMap.getTypename(type), name, attr)
				else: # len  = 3
					self.args[i] = (TypeMap.getTypename(self.args[i][0]), self.args[i][1], self.args[i][2])

		self.ndefaults = 0
		for i in range(len(self.args)):
			arg = self.args[i]
			if arg[2].has_key('default'):
				self.ndefaults = self.ndefaults+1
			elif self.ndefaults != 0:  # arg wo default following args with default
				print "Error: argument ", i+1, " of ", self.name, "must have a default value"
				import sys
				sys.exit(1)

	def __eq__(self, other):
		if len(self.args) != len(other.args): return False;
		for i in range(len(self.args)):
			# check type only, not name or default
			if self.args[i][0] != other.args[i][0]: return False
		return True


	def __ne__(self, other): return not self.__eq__(other)



	def asDecl(self, prefix=False):
		r = ""
		if prefix and len(self.args): r = " ,"
		for i in range(len(self.args)):
			arg = self.args[i]
			r = r+ "%s %s" % (arg[0], arg[1])
			if (i < len(self.args)-1): r = r + ", "
		return r

	def asSignature(self):
		r = ""
		for i in range(len(self.args)):
			arg = self.args[i]
			if arg[2].has_key('default'):
				r += "%s %s = %s" % (arg[0], arg[1], arg[2]['default'])
			else:
				r += "%s %s" % (arg[0], arg[1])
			if (i < len(self.args)-1): r = r + ", "
		return r;



	def asObjDecl(self):
		r = ""
		for i in range(len(self.args)):
			arg = self.args[i]
			r = r+ "Obj<%s>& %s" % (arg[0], arg[1])
			if (i < len(self.args)-1): r = r + ", "
		return r

	def asArgs(self):
		r = ""
		for i in range(len(self.args)):
			arg = self.args[i]
			r = r+ "%s" % (arg[1])
			if (i < len(self.args)-1): r = r + ", "
		return r

	def asCArgs(self, prefix = False):
		r = ""
		if prefix and len(self.args): r = " ,"
		for i in range(len(self.args)):
			arg = self.args[i]
			r = r+ "a%dc.v" % (i+1)
			if (i < len(self.args)-1): r = r + ", "
		return r

	def asTypeList(self, prefix = 1):
		r = ""
		pref = ""
		if (prefix): pref = ", "

		for i in range(len(self.args)):
			arg = self.args[i]
			r = r+ "%s%s" % (pref, arg[0])
			pref = ", "
			#if (i < len(self.args)-1): r = r + ", "
		return r

	def asObjArgs(self):
		r = ""
		for i in range(len(self.args)):
			arg = self.args[i]
			r = r+ "%s.a" % (arg[1])
			if (i < len(self.args)-1): r = r + ", "
		return r

	def asPyFormat(self):
		r = "O" * (len(self.args) - self.ndefaults)
		if self.ndefaults != 0: r = r + "|"
		r = r + "O" * self.ndefaults
		return r

	def asDefaultList(self):
		r = ""
		for a in self.args:
			if a[2].has_key('default'):
				r = r + ", %s" % DefaultMap.getDefault(a[2]['default'])
		return r

	def getNDefaults(self): return self.ndefaults


class Overload:
	def __init__(self, retval, args= [], flags = None):
		self.retval = TypeMap.getTypename(retval)
		self.args = ArgList()
		for a in args: self.args.add(a)
		self.args.complete()
		self.ownresult = 0

		self.protected = 0
		self.private = 0
		self.virtual = 0
		self.const = 0
		self.static = 0
		self.abstract = 0
		self.isimpl = 0
		self.doc = None
		if flags:
			if 'c' in flags: self.const = 1
			if 'p' in flags: self.protected = 1
			if 'P' in flags: self.private = 1
			if 'v' in flags: self.virtual = 1
			if 's' in flags: self.static = 1
			if 'a' in flags: self.abstract = 1
			if 'i' in flags: self.isimpl = 1
		self.implementation = ""

	def __eq__(self, other):
		if type(self) != type(other): return False
		if self.retval != other.retval: return False
		if self.args != other.args: return False
		if self.const != other.const: return False
		return True


	def __ne__(self, other): return not self.__eq__(other)

	def copy(self):
		r = Overload(self.retval)
		r.args = self.args.copy()
		r.protected = self.protected
		r.private = self.private 
		r.virtual = self.virtual
		r.const = self.const
		r.static = self.static
		r.abstract = self.abstract
		r.isimpl = self.isimpl
		r.ownresult = self.ownresult
		r.doc = self.doc
		return r

	def setDocumentation(self, doc):
		self.doc = doc




	def asPySignature(self):
		r = "(%s)" % self.args.asSignature()
		if self.retval not in ['', 'void']: r += " -> %s" % self.retval
		return r


	def createOverloadSets(ols, name = None):
		#each set contains a list of overloads having the same 
		#number of args. 
		#consider following overloads:
		# A: (x, y=0, z=0)
		# B: (a, b, c, d=0)
		# this will lead to 3 sets:
		# 1:  (1-2): A
		# 2:  (3-3): A, B
		# 3:  (4-4): B
		#which leads to the following c code:
		# if (nargs == 1 || nargs == 2) {
		#     try A
		# }
		# if (nargs == 3) {
		# try A and B
		# }
		# if (nargs == 4) {
		#     try B
		# }

		anums = {}
		totmax = 0
		for ol in ols:
			minargs = len(ol.args.args)-ol.args.ndefaults
			maxargs = len(ol.args.args)
			if totmax < maxargs: totmax = maxargs
			for i in range(minargs, maxargs+1):
				if anums.has_key(i): anums[i].append(ol)
				else: anums[i] = [ol]
		sets = []
		currentmin = -1
		currentmax = -1
		for i in range(totmax+1):
			if anums.has_key(i):
				# start a new set
				if currentmin == -1:
					currentmin = i
					currentmax = i
					currentlist = anums[i]
				#continue an existing set
				elif currentlist == anums[i]:
					currentmax = i
				# close current set an start a new one
				else: 
					sets.append((currentmin, currentmax, currentlist))
					currentmin = i
					currentmax = i
					currentlist = anums[i]
			else:
				# close current set
				if currentmin != -1:
					sets.append((currentmin, currentmax, currentlist))
					currentmin = -1
		if currentmin != -1:
			sets.append((currentmin, currentmax, currentlist))
		return sets
	createOverloadSets = staticmethod(createOverloadSets)

class Constructor:
	def __init__(self, args, flags, klass):
		self.ol = [Overload("", args, flags)]
		self.klass = klass

	def addOverload(self, args, flags):
		self.ol.append(Overload("", args, flags))

	def update(self):
		self.sets = Overload.createOverloadSets(self.ol, "constructor")

	def setDocumentation(self, doc):
		newdoc = "%s%s<BR><BR>" % (self.klass.name, self.ol[-1].asPySignature())
		newdoc += doc;
		self.ol[-1].setDocumentation(newdoc)

	def getDocumentation(self):
		doc = ""
		for ol in self.ol:
			if ol.doc:
				if doc != "": doc += "<BR><BR><BR>"
				doc += ol.doc
		return doc

	


class Method:
	def __init__(self, name, retval=None, args=None, flags=None):
		self.name = name # used for method inheritance, etc
		self.pyname = name #python method name
		self.cname = name # method to be called in C
		if retval != None: self.ol = [Overload(retval, args, flags)]
		else: self.ol = []
		self.doc = None

	def addOverload(self, retval, args, flags):
		self.ol.append(Overload(retval, args, flags))

	def setPyName(self, name):
		self.pyname = name


	def setDocumentation(self, doc):
		newdoc = "%s%s<P>" % (self.name, self.ol[-1].asPySignature())
		newdoc += doc;
		self.ol[-1].setDocumentation(newdoc)

	def getDocumentation(self):
		doc = ""
		for ol in self.ol:
			if ol.doc:
				if doc != "": doc += "<BR><BR>"
				doc += ol.doc
		return doc

	def update(self):
		self.sets = Overload.createOverloadSets(self.ol, self.name)


		"""
	def createOverloadSets(self):
		#each set contains a list of overloads having the same 
		#number of args. 
		#consider following overloads:
		# A: (x, y=0, z=0)
		# B: (a, b, c, d=0)
		# this will lead to 3 sets:
		# 1:  (1-2): A
		# 2:  (3-3): A, B
		# 3:  (4-4): B
		#which leads to the following c code:
		# if (nargs == 1 || nargs == 2) {
		#     try A
		# }
		# if (nargs == 3) {
		# try A and B
		# }
		# if (nargs == 4) {
		#     try B
		# }

		if len(self.ol) > 1:
			print "MultipleOverloads for", self.name
		anums = {}
		totmax = 0
		for ol in self.ol:
			minargs = len(ol.args.args)-ol.args.ndefaults
			maxargs = len(ol.args.args)
			if totmax < maxargs: totmax = maxargs
			for i in range(minargs, maxargs+1):
				if anums.has_key(i): anums[i].append(ol)
				else: anums[i] = [ol]
		self.sets = []
		currentmin = -1
		currentmax = -1
		for i in range(totmax+1):
			if anums.has_key(i):
				# start a new set
				if currentmin == -1:
					currentmin = i
					currentmax = i
					currentlist = anums[i]
				#continue an existing set
				elif currentlist == anums[i]:
					currentmax = i
				# close current set an start a new one
				else: 
					self.sets.append((currentmin, currentmax, currentlist))
					currentmin = i
					currentmax = i
					currentlist = anums[i]
			else:
				# close current set
				if currentmin != -1:
					self.sets.append((currentmin, currentmax, currentlist))
					currentmin = -1
		if currentmin != -1:
			self.sets.append((currentmin, currentmax, currentlist))
		if len(self.ol) > 1:
			print "     created", self.sets

			"""


class Member:
	def __init__(self, name, type):
		self.name = name
		self.type = type
		self.doc = None

	def setDocumentation(self, doc):
		self.doc = doc

	def getDocumentation(self):
		return self.doc



class CClass:
	def __init__(self, name, base, namespace = ''):
		self.name = name
		self.corename = name
		self.base = []
		for b in base:
			self.base.append(string.split(b, '::')[-1])
		self.corefile = name
		self.instaciatable = False
		self.constructor = None
		self.methods = {}
		self.operators = {}
		self.members = {}
		self.staticmembers = {}
		self.enums = {}
		self.toC = ['ptr', 'cptr', 'cref', 'ref']
		self.fromC = ['ptr', 'cptr', 'cref', 'ref']
		self.noshell = 0
		self.nonpolymorphic = 0
		self.derived = []
		self.namespace = namespace
		while self.namespace[0] == ":": self.namespace = self.namespace[1:]
		self.include_extra_decl = ""
		self.include_extra_impl = ""
		self.extra_typedef = ""
		self.min = 0
		self.final = 0
		self.doc = None
		self.innerclasses = {}
		self.outer = None
		self.childs = []
		self.inheritedmethods = {}
		self.undeletable = False
		self.using = {}
		self.condition = None
		self.destructionmethod = None

	#note on final:
	#for classes that have no constructors, a shell is still generated
	#having only static methods that can be called from python derived classes
	#however, if the destructor is private this is not possible. In this specific
	#case it is also not required because there can't be derived classes.

	def namespacePrefix(self):
		if self.namespace: return "%s::" % self.namespace
		return ''

	def fullCoreName(self): return self.namespacePrefix() + self.corename

	def hasConstructor(self):
		return self.constructor

	def setConstructor(self, args, flags = ""):
		self.setBaseConstructor(args[:], flags)
		self.instaciatable = True

	def setBaseConstructor(self, args, flags = ""):
		if self.constructor:
			self.constructor.addOverload(args, flags)
		else:
			self.constructor = Constructor(args, flags, self)
		self.constructor.sets = Overload.createOverloadSets(self.constructor.ol, "baseconstructor")

	def addMethod(self, name, retval, args, flags = ""):
		if self.methods.has_key(name):
			self.methods[name].addOverload(retval, args, flags)
		else:
			self.methods[name] = Method(name, retval, args, flags)
		self.methods[name].baseclass = self
		self.methods[name].sets = Overload.createOverloadSets(self.methods[name].ol, name)

	def addOperator(self, name, retval, args, flags = ""):
		self.operators[name] = Method(name, retval, args, flags)
		if name == "==":
			if 'val' not in self.toC: self.toC.append('val')
			if 'val' not in self.fromC: self.fromC.append('val')
			if 'cval' not in self.fromC: self.fromC.append('cval')


	def addNonStaticMember(self, name, type):
		self.members[name] = Member(name, TypeMap.getTypename(type))

	def addStaticMember(self, name, type):
		self.staticmembers[name] = Member(name, TypeMap.getTypename(type))

	def addEnum(self, name, *values):
		self.enums[name] = Enum(name, *values)
		self.enums[name].setNamespace(self.fullCoreName())

	def addClass(self, c):
		self.innerclasses[c.name] = c
		c.name = self.name + "_" + c.name
		c.outer = self
		c.namespace = self.fullCoreName()

	def nonPolymorphic(self):
		self.nonpolymorphic = 1

	def findParentClass(self, name):
		if self.baseclass == None: return None
		for p in self.baseclass:
			print p.fullCoreName(), "<>", name
			if p.fullCoreName() == name: return p
			b = p.findParentClass(name)
			if b != None: return b

	def findMethodDef(self, mname):
		if self.methods.has_key(mname): return self.methods[mname]
		for p in self.baseclass:
			m = p.findMethodDef(mname)
			if m: return m
		return None

	def addUsing(self, classname, methodname, flags):
		if classname[0:2] == "::": classname = classname[2:]
		self.using[methodname] = (classname, methodname, flags)

	def resolveUsing(self):
		for mname, m in self.using.items():
			print "RESOLVING USING", mname, " IN ", self.name
			if (self.methods.has_key(mname)):
				method = self.methods[mname]
			else:
				method = Method(mname)
				method.baseclass = self
				self.methods[mname] = method
			originclass = self.findParentClass(m[0])
			if originclass == None:
				print "Error: no baseclass found for Using statement"
				sys.exit(1)
			# we now have X from 'using X::foo', but this does not mean
			# foo is defined in X. it can be one of the base classes of X
			origmethod = originclass.findMethodDef(mname)
			if origmethod == None:
				print "Error: no method found for Using statement"
				sys.exit(1)

			for ol in origmethod.ol:
				newol = ol.copy()
				# change public/private
				if not newol in method.ol: method.ol.append(newol)
			method.update()




	def __getitem__(self, item):
		if self.methods.has_key(item): return self.methods[item]
		if self.innerclasses.has_key(item): return self.innerclasses[item]
		if self.staticmembers.has_key(item): return self.staticmembers[item]
		if self.enums.has_key(item): return self.enums[item]
		if len(item) > 8 and item[0:8] == "operator":
			if self.operators.has_key(item[8:]): return self.operators[item[8:]]
		return self.members[item]

	def __repr__(self):
		return "<class %s>" % self.name

	def setDocumentation(self, doc):
		self.doc = doc

