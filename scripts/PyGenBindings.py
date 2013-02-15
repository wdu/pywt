# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.

import string
import os
import re
import CClass
import sys
import GenAutoFunctions
import SmartFile

open = SmartFile.open

global root
global moduledir
global globlist
globlist = []

only = ""
#only = "wtchart_"
minonly = 0
#minonly = 1

def HtmlToPlain(html):
	return html
	import os
	(stdin, stdout) = os.popen2("html2text -style pretty -")
	stdin.write(html)
	stdin.close()
	plain = stdout.read()
	stdout.close()
	return plain


def writeWithNamespace(f, namespace, name):
	nlist = namespace.split("::")
	cnt = 0
	for e in nlist:
		if e == "": continue
		f.write("namespace %s { " % e)
		cnt = cnt+1
	f.write("class %s;" % name)
	for i in range(cnt): f.write("}")
	f.write("\n")
	


def getEnumFiles(adir):
	f = os.popen("grep -l enums %s/*.cl" % adir)
	files = f.readlines()
	f.close()
	for i in range(len(files)):
		files[i] = string.strip(files[i])
	return files

def getClassFiles(adir):
	f = os.popen("grep -L enums %s/*.cl" % adir)
	files = f.readlines()
	f.close()
	for i in range(len(files)):
		files[i] = string.strip(files[i])
	return files


def hexchar(val):
	v = hex(val)
	v = v[2:]  # remove 0x
	if len(v) == 1: v = "0" + v
	return "x"+v

def getdependencies(aclass, classlist):
	r = aclass.innerclasses.values()
	r = r + aclass.baseclass
	result = []
	for c in r:
		if c in classlist: result.append(aclass)
	return result


def sortClasslist(classlist):
	result = []
	seeds = []
	for c in classlist:
		deps = getdependencies(c, classlist)
		if len(deps) == 0: seeds.append(c);

	for c in seeds:
		result.append(c)
		classlist.remove(c)

	#print "SEEDS: ", seeds

	while len(classlist) > 0:
		seeds = []
		for c in classlist:
			ok = True
			allbases = getdependencies(c, classlist)
			for base in allbases:
				if base not in result:
					ok = False
					break;
			if ok:
				seeds.append(c)

		for c in seeds:
			result.append(c)
			classlist.remove(c)

	return result




	


def shellFile(c):
	return "g%s_shell.h" % c

def shellCFile(c):
	return "g%s_shell.inc" % c


def pythonFile(c):
	return "gPy%s.h" % c

def pythonCFile(c):
	return "gPy%s.inc" % c

def argformatstring(tot, default):
	nondefault = tot - default
	r = "O" * nondefault
	if (default > 0): r = r + "|"
	r = r + "O" * default
	r = r + ":"
	return r

def argnamelist(args):
	r = "{"
	for a in args:
		r = r + "\"%s\"" % a[1]
		r = r + ", "
	r = r + "0}"
	return r
	


nunits = 5




def generateIncludes(l, prefix):
	unit = len(l)/nunits
	if unit*nunits < len(l): unit = unit +1
	if (len(l) > 12):
		index = 1
		cnt = 0;
		f = open(moduledir+ "/g%sincludeTypes%d.h" % (prefix, index), 'w')
		f.write("#include <Wt/WConfig.h>\n")
		for i in l:
			if i.min or not minonly:
				if i.condition: f.write("%s\n" % i.condition)
				f.write("#include \"%s\"\n" % pythonFile(i.name))
				if i.condition: f.write("#endif\n")
			cnt = cnt+1
			if (cnt == unit):
				f.close()
				cnt = 0
				index = index +1
				f = open(moduledir+ "/g%sincludeTypes%d.h" % (prefix, index), 'w')
				f.write("#include <Wt/WConfig.h>\n")
		f.close()
	else:
		f = open(moduledir+ "/g%sincludeTypes.h" % prefix, 'w')
		f.write("#include <Wt/WConfig.h>\n")
		for i in l:
			if minonly and not i.min:continue
			if i.condition: f.write("%s\n" % i.condition)
			f.write("#include \"%s\"\n" % pythonFile(i.name))
			if i.condition: f.write("#endif\n")
		f.close()

	f = open(moduledir+ "/g%sincludeTypes.h" % prefix, 'w')
	f.write("#include <Wt/WConfig.h>\n")
	for i in l:
		if minonly and not i.min:continue
		if i.condition: f.write("%s\n" % i.condition)
		f.write("#include \"%s\"\n" % pythonFile(i.name))
		if i.condition: f.write("#endif\n")
	f.close()

	f = open(moduledir+ "/g%sincludeShellImpl.h" % prefix, 'w')
	f.write("#include <Wt/WConfig.h>\n")
	for i in l:
		if minonly and not i.min:continue
		if i.condition: f.write("%s\n" % i.condition)
		f.write("#include \"%s\"\n" % shellCFile(i.name))
		if i.condition: f.write("#endif\n")
	f.close()

	f = open(moduledir+ "/g%sincludeShell.h" % prefix, 'w')
	f.write("#include <Wt/WConfig.h>\n")
	for i in l:
		if minonly and not i.min:continue
		if i.condition: f.write("%s\n" % i.condition)
		f.write("#include \"%s\"\n" % shellFile(i.name))
		if i.condition: f.write("#endif\n")
	f.close()

	if (len(l) > 12):
		index = 1
		cnt = 0;
		f = open(moduledir+ "/g%sincludeWrapperImpl%d.h" % (prefix, index), 'w')
		f.write("#include <Wt/WConfig.h>\n")
		f.write("#include \"Compare.h\"\n")
		f.write("#include \"PyInstanceFactory.h\"\n")
		f.write("#include \"Arg.h\"\n")
		f.write("#include \"Res.h\"\n")
		f.write("#include \"Threading.h\"\n")
		f.write("#include \"Enum.h\"\n")
		f.write("#include \"g_runmethod.inc\"\n")
		for i in l:
			if i.min or not minonly:
				if i.condition: f.write("%s\n" % i.condition)
				f.write("#include \"%s\"\n" % pythonCFile(i.name))
				if i.condition: f.write("#endif\n")
			cnt = cnt+1
			if (cnt == unit):
				f.close()
				cnt = 0
				index = index +1
				f = open(moduledir+ "/g%sincludeWrapperImpl%d.h" % (prefix, index), 'w')
				f.write("#include <Wt/WConfig.h>\n")
				f.write("#include \"Compare.h\"\n")
				f.write("#include \"PyInstanceFactory.h\"\n")
				f.write("#include \"Arg.h\"\n")
				f.write("#include \"Res.h\"\n")
				f.write("#include \"Threading.h\"\n")
				f.write("#include \"Enum.h\"\n")
				f.write("#include \"g_runmethod.inc\"\n")
		f.close()
	else:
		f = open(moduledir+ "/g%sincludeWrapperImpl.h" % prefix, 'w')
		f.write("#include <Wt/WConfig.h>\n")
		f.write("#include \"Compare.h\"\n")
		f.write("#include \"PyInstanceFactory.h\"\n")
		f.write("#include \"Arg.h\"\n")
		f.write("#include \"Res.h\"\n")
		f.write("#include \"Threading.h\"\n")
		f.write("#include \"Enum.h\"\n")
		f.write("#include \"g_runmethod.inc\"\n")
		for i in l:
			if minonly and not i.min:continue
			if i.condition: f.write("%s\n" % i.condition)
			f.write("#include \"%s\"\n" % pythonCFile(i.name))
			if i.condition: f.write("#endif\n")
		f.close()

def escapeName(name):
	name = re.sub(':', '_', name)
	return name

def escapeString(s):
	s = re.sub(r"\\", r"\\\\", s)
	s = re.sub('\n', r'\\n', s)
	s = re.sub('"', r'\"', s)
	return s



def generateStrings(l, enumlist, prefix):
	print "Converting documentation strings to plain text"
	f = open(moduledir+ "/g%sStrings.cpp" % prefix, 'w')
	for c in l:
		if c.doc:
			doc = c.doc
			constrdoc = ""
			if c.constructor: constrdoc = c.constructor.getDocumentation()
			if constrdoc != "":
				doc = doc + "<BR>Constructors:<BR><BR>\n"
				doc = doc + constrdoc
			f.write("const char* doc_class_%s = \"%s\";\n\n" % 
					(escapeName(c.fullCoreName()), escapeString(HtmlToPlain(doc))))
		for m in c.methods.values():
			if m.getDocumentation():
				f.write("const char* doc_method_%s_%s = \"%s\";\n\n" % 
					(escapeName(c.fullCoreName()), escapeName(m.name), escapeString(HtmlToPlain(m.getDocumentation()))))
		for m in c.members.values():
				f.write("const char* doc_member_%s_%s = \"%s\";\n\n" % 
					(escapeName(c.fullCoreName()), escapeName(m.name), escapeString(HtmlToPlain(m.getDocumentation()))))
		for m in c.staticmembers.values():
				f.write("const char* doc_staticmember_%s_%s = \"%s\";\n\n" % 
					(escapeName(c.fullCoreName()), escapeName(m.name), escapeString(HtmlToPlain(m.getDocumentation()))))
		for m in c.enums.values():
			doclist = m.getDocumentation()
			f.write("const char* doc_enum_%s_%s_type = \"%s\";\n\n" %
				(escapeName(c.fullCoreName()), escapeName(m.name), escapeString(HtmlToPlain(doclist[0]))))
			for i in range(len(m.values)):
				f.write("const char* doc_enum_%s_%s_value_%s = \"%s\";\n\n" %
				(escapeName(c.fullCoreName()), escapeName(m.name), escapeName(m.values[i]), escapeString(HtmlToPlain(doclist[i+1]))))
	for e in enumlist:
		doclist = e.getDocumentation()
		f.write("const char* doc_enum_global_%s_type = \"%s\";\n\n" %
			(escapeName(e.name), escapeString(HtmlToPlain(doclist[0]))))
		for i in range(len(e.values)):
			f.write("const char* doc_enum_global_%s_value_%s = \"%s\";\n\n" %
			(escapeName(e.name), escapeName(e.values[i]), escapeString(HtmlToPlain(doclist[i+1]))))

	f.close()


def generateConversions(l, enumlist, prefix):
	f = open(moduledir+ "/g%sConversion.h" % prefix, 'w')
	corefiles = []
	for e in enumlist:
		if hasattr(e, 'corefile') and e.corefile not in corefiles:
			corefiles.append(e.corefile)
	f.write("#ifndef %sCONVERSION_H\n"% prefix)
	f.write("#define %sCONVERSION_H\n"% prefix)
	f.write("#include \"CPPClassConversion.h\"\n\n")
	f.write("#include \"SpecialConversion.h\"\n\n")
	f.write("#include <Wt/WConfig.h>\n")
	if prefix == "wt_":
		f.write("#include \"WtSpecialConversion.h\"\n\n")
	if prefix == "wthttp_":
		f.write("#include \"HttpSpecialConversion.h\"\n\n")
	if prefix == "wtchart":
		f.write("#include \"ChartSpecialConversion.h\"\n\n")
	for c in l:
		if c.condition: f.write("%s\n" % c.condition)
		needinclude = 0
		if ('val' in c.toC or 'cval' in c.toC or 'val' in c.fromC or 'cval' in c.fromC):
			needinclude = 1
		if len(c.enums): needinclude = 1
		if c.outer != None: needinclude = 1

		if needinclude:
			f.write("#include \"%s\"\n" % c.corefile)
		else:
			if c.namespace:
				writeWithNamespace(f, c.namespace, c.corename)
			else:
				f.write("class %s;\n" % c.corename)
		arg =  ("template<> class Python2C", c.fullCoreName(), c.fullCoreName())
		arg2 =  ("template<> class C2Python", c.fullCoreName(), c.fullCoreName())
		for i in c.toC:
			if i == 'ptr':  
				f.write("%s< %s* >: public Python2C_ptr< %s > {};\n" % arg)
			if i == 'cptr': 
				f.write("%s< const %s* >: public Python2C_cptr< %s > {};\n" % arg)
			if i == 'cref': 
				f.write("%s< const %s& >: public Python2C_cref< %s > {};\n" % arg)
			if i == 'ref': 
				f.write("%s< %s& >: public Python2C_ref< %s > {};\n" % arg)
			if i == 'val': 
				f.write("%s< %s >: public Python2C_val< %s > {};\n" % arg)
			if i == 'cval': 
				f.write("%s< const %s >: public Python2C_cval< %s > {};\n" % arg)
		for i in c.fromC:
			if i == 'ptr':  
				f.write("%s< %s* >: public C2Python_ptr< %s > {};\n" % arg2)
			if i == 'cptr':  
				f.write("%s< const %s* >: public C2Python_cptr< %s > {};\n" % arg2)
			if i == 'cref':  
				f.write("%s< const %s& >: public C2Python_cref< %s > {};\n" % arg2)
			if i == 'ref':  
				f.write("%s< %s& >: public C2Python_ref< %s > {};\n" % arg2)
			if i == 'val':  
				f.write("%s< %s >: public C2Python_val< %s > {};\n" % arg2)
			if i == 'cval':  
				f.write("%s< const %s >: public C2Python_cval< %s > {};\n" % arg2)
		for e in c.enums.values():
			arg = (c.fullCoreName(), e.name, c.fullCoreName(), e.name)
			f.write("template<> class Python2C< %s::%s >: public Python2C_enum< %s::%s > {};\n" % arg)
			f.write("template<> class Python2C<Wt::WFlags< %s::%s > >: public Python2C_flags< %s::%s > {};\n" % arg)
			f.write("template<> class Python2C<const Wt::WFlags< %s::%s >& >: public Python2C_flags< %s::%s > {};\n" % arg)
			f.write("template<> class C2Python< %s::%s >: public C2Python_enum< %s::%s > {};\n" % arg)
			f.write("template<> class C2Python< const %s::%s >: public C2Python_enum< %s::%s > {};\n" % arg)
			f.write("template<> class C2Python<Wt::WFlags< %s::%s > >: public C2Python_flags< %s::%s > {};\n" % arg)
			f.write("template<> class C2Python<const Wt::WFlags< %s::%s > >: public C2Python_flags< %s::%s > {};\n" % arg)
		if c.condition: f.write("#endif\n")
		f.write("\n\n\n")
	for cf in corefiles:
		f.write("#include <%s>\n" % cf)
	for e in enumlist:
		arg = (e.fullname, e.fullname)
		f.write("template<> class Python2C< %s >: public Python2C_enum< %s > {};\n" % arg)
		f.write("template<> class Python2C<Wt::WFlags< %s > >: public Python2C_flags< %s > {};\n" % arg)
		f.write("template<> class C2Python< %s >: public C2Python_enum< %s > {};\n" % arg)
		f.write("template<> class C2Python<Wt::WFlags< %s > >: public C2Python_flags< %s > {};\n" % arg)
	#f.write("#include \"ComplexConversion.h\"\n\n")


	f.write("#endif\n")
	f.close()
	f = open(moduledir+ "/g%sConversionImpl.inc" % prefix, 'w')
	f.write("#include \"g%sConversion.h\"\n\n" %prefix)
	f.write("#include \"CPPClassConversion.inc\"\n\n")
	for c in l:
		if c.condition: f.write("%s\n" % c.condition)
		f.write("#include \"%s\"\n" % c.corefile)
		arg = c.fullCoreName()
		for i in c.toC:
			if i == 'ptr':  f.write("template class Python2C_ptr< %s >;\n" % arg)
			if i == 'cptr': f.write("template class Python2C_cptr< %s >;\n" % arg)
			if i == 'cref': f.write("template class Python2C_cref< %s >;\n" % arg)
			if i == 'ref':  f.write("template class Python2C_ref< %s >;\n" % arg)
			if i == 'val':  f.write("template class Python2C_val< %s >;\n" % arg)
			if i == 'cval': f.write("template class Python2C_cval< %s >;\n" % arg)
		for i in c.fromC:
			if i == 'ptr':  f.write("template class C2Python_ptr< %s >;\n" % arg)
			if i == 'cptr': f.write("template class C2Python_cptr< %s >;\n" % arg)
			if i == 'cref': f.write("template class C2Python_cref< %s >;\n" % arg)
			if i == 'ref': f.write("template class C2Python_ref< %s >;\n" % arg)
			if i == 'val':  f.write("template class C2Python_val< %s >;\n" % arg)
			if i == 'cval': f.write("template class C2Python_cval< %s >;\n" % arg)
		for e in c.enums.values():
			f.write("template class Python2C_enum< %s::%s >;\n" % (c.fullCoreName(), e.name))
			f.write("template class Python2C_flags< %s::%s >;\n" % (c.fullCoreName(), e.name))
			f.write("template class C2Python_enum< %s::%s >;\n" % (c.fullCoreName(), e.name))
			f.write("template class C2Python_flags< %s::%s >;\n" % (c.fullCoreName(), e.name))
		f.write("\n\n\n")
		if c.condition: f.write("#endif\n")
	f.write("#include \"Enum.h\"\n")
	for e in enumlist:
		f.write("template class Python2C_enum< %s >;\n" % (e.fullname))
		f.write("template class Python2C_flags< %s >;\n" % (e.fullname))
		f.write("template class C2Python_enum< %s >;\n" % (e.fullname))
		f.write("template class C2Python_flags< %s >;\n" % (e.fullname))

	f.close()

def generateGlobalEnumDef(e, f):
	f.write("\tstatic EnumType< %s > EnumType_%s(\"%s%s\");\n" % (e.fullname, e.name, e.pyNamespace(), e.name))
	for v in e.values:
		f.write("\tPyModule_AddObject(m, \"%s\", EnumType< %s >::create(\"%s%s\", %s%s));\n" % (v, e.fullname, e.pyNamespace(), v, e.namespace, v))

def generateClassEnumDef(e, f):
	f.write("\t\t\tstatic EnumType< %s > EnumType_%s(\"%s%s\");\n" % (e.fullname, e.name, e.pyNamespace(), e.name))
	for v in e.values:
		f.write("\t\t\taddStaticMember(\"%s\", EnumType_%s.create(\n\t\t\t\t\"%s%s\", %s%s));\n" % (v, e.name, e.pyNamespace(), v, e.namespace, v))
	f.write("\n")




def generateGlobalEnumDecl(enumlist, prefix):
	corefiles = []
	for e in enumlist:
		if hasattr(e, 'corefile') and e.corefile not in corefiles:
			corefiles.append(e.corefile)
	f = open(moduledir + "/g%sincludeEnums.h" % prefix, 'w')
	for e in enumlist:
		generateGlobalEnumDef(e, f)
	f.close();
	f = open(moduledir + "/g%sEnumConversions.h" % prefix, 'w')
	f.close();

def generateTypeDeclarations(l, prefix):
	f = open(moduledir + "/g%sTypeDecl.h" % prefix, 'w')
	f.write("#include <Wt/WConfig.h>\n")
	for i in l:
		if minonly and not i.min:continue
		if i.outer != None: continue
		if i.condition: f.write("%s\n" % i.condition)
		f.write("ADD_TYPE(m, %s, %s)\n" % (i.corename, i.name))
		if i.condition: f.write("#endif\n")
	f.close()

def generateProxyTypes(l, prefix):
	f = open(moduledir + "/g%sProxyTypes.h" % prefix, 'w')
	f.write("#include \"Proxy.h\"\n")
	f.write("#include <Wt/WConfig.h>\n")
	for c in l:
		if c.condition: f.write("%s\n" % c.condition)
		f.write("#include \"%s\"\n" % c.corefile)
		f.write("template <> class Proxy<%s>: public %s {};\n" % (c.fullCoreName(), c.proxy))
		if c.condition: f.write("#endif\n")
	f.close()

def generateCompareFuncs(l, prefix):
	f = open(moduledir + "/g%sCompare.cpp" % prefix, 'w')
	f.write("#include \"Compare.inc\"\n")
	f.write("#include <Wt/WConfig.h>\n")
	for c in l:
		if c.condition: f.write("%s\n" % c.condition)
		if '<' in c.comparefunc: # it's a template
			f.write("#include <%s>\n" % c.corefile)
			f.write("template PyObject* %s(PyObject*, PyObject*, int);\n" % c.comparefunc)
		if c.condition: f.write("#endif\n")
	f.close()

def generateTemplateTypeDefs(ttypes, prefix):
	f = open(moduledir + "/g%sTemplateTypeDefs.inc" % prefix, 'w')
	for t in ttypes:
		f.write("template<> PyCPPClassType< %s >* PyCPPClassType< %s >::Type = 0;\n" % (t['cname'], t['cname']))
	f.close()
	f = open(moduledir + "/g%sTemplateTypeInst.inc" % prefix, 'w')
	index = 1
	for t in ttypes:
		f.write("\tstatic %s tt%d(\"%s\");\n" % (t['pyclass'], index, t['cname']))
		f.write("\tPyType_Ready((PyTypeObject*)&tt%d);\n" % index)
		f.write("\tPy_INCREF(&tt%d);\n" % index)
		f.write("\tPyModule_AddObject(m, \"%s\", (PyObject *)&tt%d);\n" % (t['cname'], index))
		index = index +1
	f.close()


def write_own_release_args(f, args, indent):
	for a in args:
		if a[2].has_key('ownargif'):
			f.write("%sPyCPPClassInstance* inst_%s = (PyCPPClassInstance*)(%sp);\n" %(indent, a[1], a[1]))
			f.write("%sif (%s(%sc)) inst_%s->setCOwnership();\n" % (indent, a[2]['ownargif'], a[1], a[1]))



def writeMultiOverloadMethodCall(f, indent, s, m, c):
	public = []
	protected = []
	for ol in s:
		if ol.protected or ol.private: protected.append(ol)
		else: public.append(ol)
	f.write("\t\t\tbool argsok = false;\n")
	indent2 = indent + "\t"
	for ol in public + protected:
		f.write("\t\t\tif (!argsok) {\n")
		f.write("\t\t\t\tconst char* argnames[] = %s;\n" % argnamelist(ol.args.args))
		f.write("\t\t\t\targsok = true;\n")
		f.write("\t\t\t\tARGS_%d_%dD_CHECK(\"%s:%s\", (char**)argnames%s);\n" % (
			len(ol.args.args),
			ol.args.getNDefaults(), 
			ol.args.asPyFormat(), 
			m.name,
			ol.args.asTypeList(1)))
		f.write("\t\t\t\tif (argsok) {\n")
		if ol.abstract:
			f.write("%sPyErr_Format(PyExc_RuntimeError, \"\'%s(%s)::%s\' is abstract\");\n" % (indent, m.baseclass.name, c.name, m.name))
			f.write("%sreturn 0;\n" % indent)
		if ol.protected and not ol.static:
			f.write("\t\t\t\t\tif (!self->isSubClassInstance()) {\n")
			f.write("\t\t\t\t\t\tPyErr_Format(PyExc_RuntimeError, \"\'%s(%s)::%s\' is protected within this context (%%s)\", self->ob_type->tp_name);\n" % ( m.baseclass.name, c.name, m.name))
			f.write("\t\t\t\t\t\treturn 0;\n")
			f.write("\t\t\t\t}\n")
		f.write("\t\t\t\t\tARGS_%d_%dD_CONV(\"%s:%s\", (char**)argnames%s%s);\n" % (
			len(ol.args.args),
			ol.args.getNDefaults(), 
			ol.args.asPyFormat(), 
			m.name,
			ol.args.asTypeList(1),
			ol.args.asDefaultList()))
		if hasattr(ol, "beforecall"): f.write("\t\t\t\t\t%s\n" % ol.beforecall)
		if (ol.retval == 'void'): 
			f.write("\t\t\t\t\tSTART_CPPCALL\n")
			f.write("\t\t\t\t\t{ ")
		else: 
			f.write("\t\t\t\t\tRes< %s > rv;\n" % (ol.retval))
			f.write("\t\t\t\t\tSTART_CPPCALL\n")
			f.write("\t\t\t\t\t{ rv = ")
		if ol.static and ol.protected:
			f.write("%s_shell::base_%s(%s); }\n" % (c.name, m.cname, ol.args.asCArgs()))
		elif ol.static: #static public
			f.write("%s::%s(%s);\n }" % (c.fullCoreName(), m.cname, ol.args.asCArgs()))
		elif ol.protected: #protected, not static
			f.write("%s_shell::base_%s(me%s); }\n" % (m.baseclass.name, m.cname, ol.args.asCArgs(True)))
		else: #public not static
			#explicitely call the base class method, not the
			#shells dispatcher when virtual
			f.write("me->%s::%s(%s); }\n" % (c.fullCoreName(), m.cname, ol.args.asCArgs()))
		f.write("\t\t\t\t\tEND_CPPCALL\n")
		write_own_release_args(f, ol.args.args, "\t\t\t\t\t");
		if (ol.retval != 'void'): 
			f.write("\t\t\t\t\treturn C2Python< %s >::ret(rv);\n" % (ol.retval))
		else: f.write("\t\t\t\t\tPy_RETURN_NONE;\n")
		f.write("\t\t\t\t}\n")
		f.write("\t\t\t}\n")
	f.write("\t\t\tif (!argsok) {\n")
	f.write("\t\t\t\tPyErr_SetString(PyExc_TypeError, \"Parameters not matching\");\n")
	f.write("\t\t\t\treturn 0;\n")
	f.write("\t\t\t}\n")



def writeSingleOverloadMethodCall(f, indent, ol, m, c):
	if ol.abstract:
		f.write("%sPyErr_Format(PyExc_RuntimeError, \"\'%s(%s)::%s\' is abstract\");\n" % (indent, m.baseclass.name, c.name, m.name))
		f.write("%sreturn 0;\n" % indent)
		return
	if ol.protected and not ol.static:
		f.write("%sif (!self->isSubClassInstance()) {\n" % indent)
		f.write("%s\tPyErr_Format(PyExc_RuntimeError, \"\'%s(%s)::%s\' is protected within this context (%%s)\", self->ob_type->tp_name);\n" % (indent, m.baseclass.name, c.name, m.name))
		f.write("%s\treturn 0;\n" % indent)

		f.write("%s}\n" % indent)
	indent2 = indent + "\t"
	f.write("%sconst char* argnames[] = %s;\n" % (indent2, argnamelist(ol.args.args)))
	f.write("%sARGS_%d_%dD(\"%s:%s\", (char**)argnames%s%s);\n" % (indent2, len(ol.args.args), ol.args.getNDefaults(), ol.args.asPyFormat(), m.name, ol.args.asTypeList(1),  ol.args.asDefaultList()))
	if hasattr(ol, "beforecall"): f.write("%s%s\n" % (indent2, ol.beforecall))
	if (ol.retval == 'void'): 
		f.write("%sSTART_CPPCALL\n"% indent2)
		f.write("%s{ " % indent2)
	else: 
		f.write("%sRes< %s > rv;\n" % (indent2, ol.retval))
		f.write("%sSTART_CPPCALL\n"% indent2)
		f.write("%s{ rv = " % indent2)
	if ol.static and ol.protected:
		f.write("%s_shell::base_%s(%s); }\n" % (c.name, m.cname, ol.args.asCArgs()))
	elif ol.static:
		f.write("%s::%s(%s); }\n" % (c.fullCoreName(), m.cname, ol.args.asCArgs()))
	elif ol.protected: #protected, not static
		f.write("%s_shell::base_%s(me%s); }\n" % (m.baseclass.name, m.cname, ol.args.asCArgs(True)))
	else: #public not static
			#explicitely call the base class method, not the
			#shells dispatcher when virtual
		f.write("me->%s::%s(%s); }\n" % (c.fullCoreName(), m.cname, ol.args.asCArgs()))
	f.write("%sEND_CPPCALL\n"% indent2)
	write_own_release_args(f, ol.args.args, indent2);
	if (ol.retval != 'void'): f.write("%sreturn C2Python< %s >::ret(rv);\n" % (indent2, ol.retval))
	else: f.write("%sPy_RETURN_NONE;\n" % indent2)



def writeProtectedError(f, indent, ol, m, c):
	f.write("%sPyErr_Format(PyExc_RuntimeError, \"\'%s(%s)::%s\' is protected within this context (%%s)\", self->ob_type->tp_name);\n" % (indent, m.baseclass.name, c.name, m.name))


def writeConstructorCallSimple(f, c, cname, constr):
	if len(constr.sets) > 1:
		f.write("\tint nargs = PyTuple_Size(args) + (kwargs?PyDict_Size(kwargs):0);\n")
	for s in constr.sets:
		if len(constr.sets) > 1:
			if s[0] == s[1]:
				f.write("\tif (nargs == %d) {\n" % (s[0]))
			else:
				f.write("\tif (nargs >= %d && nargs <= %d) {\n" % (s[0], s[1]))
		if len(s[2]) == 1: 
			for ol in s[2]:
				f.write("\t\tconst char* argnames[] = %s;\n" % argnamelist(ol.args.args))
				f.write("\t\tARGS_%d_%dD(\"%s:constructor\", (char**)argnames%s%s);\n" % (
					len(ol.args.args),
					ol.args.getNDefaults(), 
					ol.args.asPyFormat(), 
					ol.args.asTypeList(1),
					ol.args.asDefaultList()))
				f.write("\t\trv = new %s(%s);\n" % (cname, ol.args.asCArgs()))
		else: #multiple calls with same amount of args
			f.write("\t\tbool argsok = false;\n")
			for ol in s[2]:
				f.write("\t\tif (!argsok) {\n")
				f.write("\t\t\tconst char* argnames[] = %s;\n" % argnamelist(ol.args.args))
				f.write("\t\t\targsok = true;\n")
				f.write("\t\t\tARGS_%d_%dD_CHECK(\"%s:constructor\", (char**)argnames%s);\n" % (
					len(ol.args.args),
					ol.args.getNDefaults(), 
					ol.args.asPyFormat(), 
					ol.args.asTypeList(1)))
				f.write("\t\t\tif (argsok) {\n")
				f.write("\t\t\t\tARGS_%d_%dD_CONV(\"%s:constructor\", (char**)argnames%s%s);\n" % (
					len(ol.args.args),
					ol.args.getNDefaults(), 
					ol.args.asPyFormat(), 
					ol.args.asTypeList(1),
					ol.args.asDefaultList()))
				f.write("\t\t\t\trv = new %s(%s);\n" % (cname, ol.args.asCArgs()))
				f.write("\t\t\t}\n")
				f.write("\t\t}\n")
			f.write("\t\tif (!argsok) {\n")
			f.write("\t\t\tPyErr_SetString(PyExc_TypeError, \"Parameters not matching\");\n")
			f.write("\t\t\tthrow PyException();\n")
			f.write("\t\t}\n")
		if len(constr.sets) > 1:
			f.write("\t} else ")
	if len(constr.sets) > 1:
		f.write("{\n") 
		f.write("\t\tPyErr_SetString(PyExc_TypeError, \"Wrong Arg number\");\n")
		f.write("\t\tthrow PyException();\n")
		f.write("\t\treturn;\n")
		f.write("\t}\n")



def writeConstructorCallDouble(f, c, cname_base, cname_sub, constr):
	if len(constr.sets) > 1:
		f.write("\tint nargs = PyTuple_Size(args) + (kwargs?PyDict_Size(kwargs):0);\n")
	for s in constr.sets:
		if len(constr.sets) > 1:
			if s[0] == s[1]:
				f.write("\tif (nargs == %d) {\n" % (s[0]))
			else:
				f.write("\tif (nargs >= %d && nargs <= %d) {\n" % (s[0], s[1]))
		if len(s[2]) == 1: 
			for ol in s[2]:
				f.write("\t\tconst char* argnames[] = %s;\n" % argnamelist(ol.args.args))
				f.write("\t\tARGS_%d_%dD(\"%s:constructor\", (char**)argnames%s%s);\n" % (
					len(ol.args.args),
					ol.args.getNDefaults(), 
					ol.args.asPyFormat(), 
					ol.args.asTypeList(1),
					ol.args.asDefaultList()))
				f.write("\t\tif (subclass) {rvs = new %s(%s);} \n" % (cname_sub, ol.args.asCArgs()))
				f.write("\t\telse {\n")
				if ol.protected:
					f.write("\t\t\t\tPyErr_Format(PyExc_RuntimeError, \"\'%s constructor\' is protected\");\n" % ( cname_base))
					f.write("\t\t\tthrow PyException();\n")
				else:
					f.write("\t\t\trv = new %s(%s);\n" % (cname_base, ol.args.asCArgs()))
				f.write("\t\t}\n")
#f.write("\t\trv = new %s(%s);\n" % (cname, ol.args.asCArgs()))
		else: #multiple calls with same amount of args
			f.write("\t\tbool argsok = false;\n")
			for ol in s[2]:
				f.write("\t\tif (!argsok) {\n")
				f.write("\t\t\tconst char* argnames[] = %s;\n" % argnamelist(ol.args.args))
				f.write("\t\t\targsok = true;\n")
				f.write("\t\t\tARGS_%d_%dD_CHECK(\"%s:constructor\", (char**)argnames%s);\n" % (
					len(ol.args.args),
					ol.args.getNDefaults(), 
					ol.args.asPyFormat(), 
					ol.args.asTypeList(1)))
				f.write("\t\t\tif (argsok) {\n")
				f.write("\t\t\t\tARGS_%d_%dD_CONV(\"%s:constructor\", (char**)argnames%s%s);\n" % (
					len(ol.args.args),
					ol.args.getNDefaults(), 
					ol.args.asPyFormat(), 
					ol.args.asTypeList(1),
					ol.args.asDefaultList()))
#f.write("\t\t\t\trv = new %s(%s);\n" % (cname, ol.args.asCArgs()))
				f.write("\t\t\t\tif (subclass) {rvs = new %s(%s);} \n" % (cname_sub, ol.args.asCArgs()))
				f.write("\t\t\t\telse {\n")
				if ol.protected:
					f.write("\t\t\t\t\t\tPyErr_Format(PyExc_RuntimeError, \"\'%s constructor\' is protected\");\n" % ( cname_base))
					f.write("\t\t\t\t\tthrow PyException();\n")
				else:
					f.write("\t\t\t\t\trv = new %s(%s);\n" % (cname_base, ol.args.asCArgs()))
				f.write("\t\t\t\t}\n")
				f.write("\t\t\t}\n")
				f.write("\t\t}\n")
			f.write("\t\tif (!argsok) {\n")
			f.write("\t\t\tPyErr_SetString(PyExc_TypeError, \"Parameters not matching\");\n")
			f.write("\t\t\tthrow PyException();\n")
			f.write("\t\t}\n")
		if len(constr.sets) > 1:
			f.write("\t} else ")
	if len(constr.sets) > 1:
		f.write("{\n") 
		f.write("\t\tPyErr_SetString(PyExc_TypeError, \"Wrong Arg number\");\n")
		f.write("\t\tthrow PyException();\n")
		f.write("\t\treturn;\n")
		f.write("\t}\n")



def generatePyObject(c):
	fname = moduledir + "/" + pythonFile(c.name)
	f = open(fname, 'w')
	f.write("#ifndef PY%s\n" % string.upper(c.name))
	f.write("#define PY%s\n\n\n" % string.upper(c.name))
#f.write("#include \"%s\"\n" % shellFile(c.name))
	if c.outer != None:
		f.write("#include <%s>\n" % c.corefile)
	elif c.namespace:
		writeWithNamespace(f, c.namespace, c.corename)
	else:
		f.write("class %s;\n" % c.corename)
	#f.write("#include <%s>\n" % c.corefile)
	for b in c.base:
		f.write("#include \"%s\"\n" % pythonFile(b))
	f.write("#include \"PyCPPClassType.h\"\n")
	if c.include_extra_decl: f.write("#include \"%s\"\n" % c.include_extra_decl)
	f.write("\n\n")
	f.write("class Py%s: public PyCPPClassType< %s > {\n" % (c.name, c.fullCoreName()))
	f.write("\tpublic:\n")
	if c.base:
		f.write("\t\tstatic const PyTypeObject* bases[%d];\n" % (len(c.base)+1))
		f.write("\t\t//this is done like this to avoid static initialization order problems\n")
		f.write("\t\tstatic const PyTypeObject** get_bases() {\n")
		cnt = 0;
		for b in c.base: 
			f.write("\t\t\tbases[%d] = Py%s::Type;\n" % (cnt, b))
			cnt = cnt +1
		f.write("\t\t\tbases[%d] = 0;\n" % cnt)
		f.write("\t\t\treturn bases;\n")
		f.write("\t\t}\n")
	# count methods
	count = 0
	methods = set()
	for m in c.methods.values():
		for ol in m.ol:
			if ol.private or ol.isimpl:
				continue
			methods.add(m.name)
	count = len(methods)
	if c.destructionmethod: count = count+1
	if count > 0: f.write("\t\tMethodDef mdefs[%d];\n" % (count+1,))
	if len(c.members) > 0: f.write("\t\tMemberDef membdefs[%d];\n" % (len(c.members)+1,))

	f.write("\t\tPy%s();\n" % (c.name))

	if c.constructor:
		f.write("\t\tstatic void baseConstructor(PyCPPClassInstance* inst, PyObject* args, PyObject* kwargs, int flags);\n")

	for m in c.methods.values():
		if m.name not in methods: continue
		f.write("\t\tstatic PyObject* s_%s(PyCPPClassInstance* self, PyObject* args, PyObject* kwargs);\n" % (m.name))

	if c.destructionmethod:
		f.write("\t\tstatic PyObject* destructionmethod(PyCPPClassInstance* self, PyObject* args, PyObject* kwargs);\n")
		

	for m in c.members.values():
		f.write("\t\tstatic PyObject* sget_%s(PyCPPClassInstance* self, void*);\n" % (m.name))
		f.write("\t\tstatic int sset_%s(PyCPPClassInstance* self, PyObject* value, void*);\n" % (m.name))
	f.write("};\n\n\n")
	f.write("\n\n\n")

	f.write("#endif\n\n\n")
	f.close()

	fname = moduledir + "/" + pythonCFile(c.name)
	f = open(fname, 'w')
	f.write("#include \"%s\"\n" % shellFile(c.name))
	f.write("#include <%s>\n" % c.corefile) # added for missing EventSignal?
# I don't understand why, but it seems that this should be written
# as a template specialization
	for (name, value) in c.innerclasses.items():
		f.write("#include \"%s\"\n" % pythonFile(value.name));
	if c.doc:
		f.write("\t\t\textern const char* doc_class_%s;\n" % escapeName(c.fullCoreName()))
	for m in c.methods.values():
		if m.getDocumentation():
			f.write("\t\t\textern const char* doc_method_%s_%s;\n" % (escapeName(c.fullCoreName()), escapeName(m.name)))
	for m in c.members.values():
		if m.getDocumentation():
			f.write("\t\t\textern const char* doc_member_%s_%s;\n" % (escapeName(c.fullCoreName()), escapeName(m.name)))
	f.write("\n")
	if c.base:
		f.write("const PyTypeObject* Py%s::bases[%d];" % (c.name, len(c.base)+1));
	f.write("template <>\n")
	f.write("PyCPPClassType< %s >* PyCPPClassType< %s >::Type = 0;\n\n\n" % (c.fullCoreName(), c.fullCoreName()))
	if c.base:
		f.write("Py%s::Py%s(): PyCPPClassType< %s >(MODPREFIX \"%s\", get_bases()){\n" % (c.name, c.name, c.fullCoreName(), c.name)) 
	else:
		f.write("Py%s::Py%s(): PyCPPClassType< %s >(MODPREFIX \"%s\") {\n" % (c.name, c.name, c.fullCoreName(), c.name))
	for e in c.enums.values():
		generateClassEnumDef(e, f)
	if c.nonpolymorphic:
		f.write("\t\t\tType->Factory = new NonPoly_PyInstanceFactory< %s >;\n" % c.fullCoreName())
	elif len(c.base):
		f.write("\t\t\tType->Factory = new Poly_PyInstanceFactory< %s, %s >;\n" % (c.fullCoreName(), c.baseclass[0].fullCoreName()))
	else:
		f.write("\t\t\tType->Factory = new Poly_PyInstanceFactory< %s, %s >;\n" % (c.fullCoreName(), c.fullCoreName()))

	if c.base and not c.nonpolymorphic:
		f.write("\t\t\tPyCPPClassType< %s >::Type->Factory->addSubClass(Factory);\n" % c.baseclass[0].fullCoreName())
	index = 0
	for m in c.methods.values():
		if m.name not in methods: continue
		staticprefix = ""
		if m.ol[0].static: staticprefix = "Static"
		docname = "0"
		if m.getDocumentation(): docname = "doc_method_%s_%s" % (escapeName(c.fullCoreName()), escapeName(m.name))
		f.write("\t\t\tmdefs[%d] = %sMethodDef(\"%s\\x00\\%s\", &Py%s::s_%s, %s );\n" % (index, staticprefix, m.pyname, hexchar(len(m.ol[0].args.args)), c.name, m.name, docname))
		index = index +1
	if c.destructionmethod:
		f.write("\t\t\tmdefs[%d] = MethodDef(\"%s\\x00\\xff\", &Py%s::destructionmethod, 0 );\n" % (index, c.destructionmethod, c.name))
	f.write("\t\t\t// Inherited methods\n")
	if c.instaciatable:
#f.write("\t\t\tsetInstanceConstructorPtr(&Py%s::baseConstructor);\n" % c.name)
		f.write("\t\t\tmakeInstanciatable();\n")
	if c.constructor:
		f.write("\t\t\tsetBaseConstructorPtr(&Py%s::baseConstructor);\n" % c.name)
	if count > 0: f.write("\t\t\tsetMethods(mdefs);\n")
	index = 0
	for m in c.members.values():
		f.write("\t\t\tmembdefs[%d] = MemberDef(\"%s\", &Py%s::sget_%s, &Py%s::sset_%s, doc_member_%s_%s);\n" % (index, m.name, c.name, m.name, c.name, m.name, escapeName(c.fullCoreName()), escapeName(m.name)))
		index = index +1
	if len(c.members) > 0: f.write("\t\t\tsetMembers(membdefs);\n")
	for (name, value) in c.innerclasses.items():
		f.write("\t\t\taddInnerClass(\"%s\", new Py%s());\n"% (name, value.name))
	if c.doc:
		f.write("\t\t\ttp_doc = doc_class_%s;\n" % escapeName(c.fullCoreName()))
	if (c.extra_typedef): f.write("\t\t\t%s\n" % c.extra_typedef)

	f.write("\t\t\ttp_richcompare = %s;\n" % c.comparefunc)

	if len(c.staticmembers) > 0:
		# because following may call factory, which calls s_new
		f.write("\t\t\tPyType_Ready(this);\n")
	for (name, value) in c.staticmembers.items():
		f.write("\t\t\taddStaticMember(\"%s\", C2Python<%s >::ret(%s::%s));\n"% 
				(name, value.type, c.fullCoreName(), name))

	f.write("};\n\n")


	if c.instaciatable or c.constructor:
		f.write("void Py%s::baseConstructor(PyCPPClassInstance* inst, PyObject* args, PyObject* kwargs, int flags) {\n" % (c.name))
		if c.final and c.instaciatable:
			f.write("\t%s* rv = 0;\n" % (c.fullCoreName()))
			writeConstructorCallSimple(f, c, c.fullCoreName(), c.constructor)
			f.write("\tinst->proxy = Proxy<%s >::create(inst, rv, flags);\n" % c.fullCoreName())
		elif (not c.final) and (not c.instaciatable):
			f.write("\t%s_shell* rv = 0;\n" % (c.name))
			writeConstructorCallSimple(f, c, "%s_shell" % c.name, c.constructor)
			f.write("\tinst->proxy = Proxy<%s >::create(inst, rv, flags);\n" % c.fullCoreName())
			f.write("\trv->setPyObject(inst);\n")
		else:
			f.write("\tbool subclass = ((flags & AbstractProxy::SUBCLASS_INSTANCE) != 0);\n")
			f.write("\t%s* rv = 0;\n" % (c.fullCoreName()))
			f.write("\t%s_shell* rvs = 0;\n" % (c.name))
			writeConstructorCallDouble(f, c, c.fullCoreName(), "%s_shell" % c.name, c.constructor)

			f.write("\t if (rvs) {\n")
			f.write("\t\tinst->proxy = Proxy<%s >::create(inst, rvs, flags);\n" % c.fullCoreName())
			f.write("\t\trvs->setPyObject(inst);\n")
			f.write("\t} else {\n")
			f.write("\t\tinst->proxy = Proxy<%s >::create(inst, rv, flags);\n" % c.fullCoreName())
			f.write("\t}\n")
		index = 1
		f.write("}\n\n")

	for m in c.methods.values():
		if m.name not in methods: continue
		f.write("PyObject* Py%s::s_%s(PyCPPClassInstance* self, PyObject* args, PyObject* kwargs) {\n" % (c.name, m.name))
		f.write("\ttry {\n")
		if not m.ol[0].static:
			f.write("\t\t%s* me = PyCPPClassType< %s >::getCore(self);\n"% (c.fullCoreName(), c.fullCoreName()))
			f.write("\t\tif (!me) return 0;\n")
		if len(m.sets) > 1:
			f.write("\t\tint nargs = PyTuple_Size(args) + (kwargs?PyDict_Size(kwargs):0);\n")
		for s in m.sets:
			if len(m.sets) > 1:
				if s[0] == s[1]:
					f.write("\t\tif (nargs == %d) {\n" % (s[0]))
				else:
					f.write("\t\tif (nargs >= %d && nargs <= %d) {\n" % (s[0], s[1]))
			if len(s[2]) == 1:
				writeSingleOverloadMethodCall(f, "\t\t", s[2][0], m, c);
			else:
				writeMultiOverloadMethodCall(f, "\t\t", s[2], m, c);
			if len(m.sets) > 1:
				f.write("\t\t}\n")
		if len(m.sets) > 1:
			f.write("\t\tPyErr_SetString(PyExc_TypeError, \"Wrong Arg number\");\n")
			f.write("\t\treturn 0;\n")
		f.write("\t}\n")
		f.write("\tcatch (PyException&) {}\n")
		f.write("\treturn 0;\n")
		f.write("}\n\n")

	if c.destructionmethod:
		f.write("PyObject* Py%s::destructionmethod(PyCPPClassInstance* self, PyObject* args, PyObject* kwargs) {\n" % (c.name,))
		
		f.write("\ttry {\n")
		f.write("\t\tchar* names[1] = {0};\n")
		f.write("\t\tif (!PyArg_ParseTupleAndKeywords(args, kwargs, \"\", names)) {\n")
		f.write("\t\t\tthrow PyException();\n")
		f.write("\t\t}\n")
		f.write("\t\tself->proxy->deleteCore();\n")
		f.write("\t\tPy_RETURN_NONE;\n")
		f.write("\t}\n")
		f.write("\tcatch (PyException&) {}\n")
		f.write("\treturn 0;\n")
		f.write("}\n\n")


	for m in c.members.values():
		f.write("PyObject* Py%s::sget_%s(PyCPPClassInstance* self, void*) {\n" % (c.name, m.name))
		f.write("\ttry {\n")
		f.write("\t\t%s* me = PyCPPClassType< %s >::getCore(self);\n"% (c.fullCoreName(), c.fullCoreName()))
		f.write("\t\tif (!me) return 0;\n")
		f.write("\t\treturn C2Python< %s >::ret(me->%s);\n" % (m.type, m.name))
		f.write("\t}\n")
		f.write("\tcatch (PyException&) {}\n")
		f.write("\treturn 0;\n")
		f.write("}\n\n")
		f.write("int Py%s::sset_%s(PyCPPClassInstance* self, PyObject* value, void*) {\n" % (c.name, m.name))
		f.write("\ttry {\n")
		f.write("\t\t%s* me = PyCPPClassType< %s >::getCore(self);\n"% (c.fullCoreName(), c.fullCoreName()))
		f.write("\t\tif (!me) return -1;\n")
#TODO: value can be 0, which means delete
		f.write("\t\t%s v = Arg< %s >(value).v;\n" % (m.type, m.type))
		f.write("\t\tme->%s = v;\n" % m.name)
		f.write("\t\treturn 0;\n")
		f.write("\t}\n")
		f.write("\tcatch (PyException&) {}\n")
		f.write("\treturn -1;\n")
		f.write("}\n\n")
	if c.include_extra_impl: f.write("#include \"%s\"\n" % c.include_extra_impl)
	f.close()

def generateShell(c):
	if (c.noshell): return
	fname = moduledir + "/" + shellFile(c.name)
	f = open(fname, 'w')
	f.write("#ifndef %s_SHELL\n" % string.upper(c.name))
	f.write("#define %s_SHELL\n\n\n" % string.upper(c.name))
	f.write("#include <%s>\n" % c.corefile)
	for base in c.base:
		f.write("#include \"%s\"\n" % shellFile(base))

	if not c.final: 
		f.write("#include \"CoreShell.h\"\n")
		f.write("\n\n")
		f.write("class %s_shell: public %s, public CoreShell {\n" %
					(c.name, c.fullCoreName()))
		f.write("\tpublic:\n")
		if not c.final and c.constructor:
			for ol in c.constructor.ol:
				f.write("\t\t%s_shell(%s):\n" % (c.name, ol.args.asDecl()))
				f.write("\t\t\t%s(%s)  {}\n" % (c.fullCoreName(), ol.args.asArgs()))

			#this code provides a public version of all protected methods
			#this is only usefull if there is a constructor, i.e. classes can inherit
			#in python
		f.write("\t\t/**** interface for protected methods ****/\n")
		f.write("\tprotected:\n")
		#for m in c.inheritedmethods.values()+c.methods.values():
		for m in c.methods.values():
			for o in m.ol:
				if o.protected and not o.static and not o.isimpl:
					f.write("\t\t%s p_%s(%s) {\n" % (o.retval, m.name, o.args.asDecl()))
					if (o.retval == 'void'): f.write("\t\t\t")
					else: f.write("\t\t\treturn ")
					f.write("%s::%s(%s);\n" % (c.fullCoreName(), m.name, o.args.asArgs()))
					f.write("\t\t}\n\n")
		f.write("\tpublic:\n")
		for m in c.methods.values():
		#for m in c.inheritedmethods.values()+c.methods.values():
			for o in m.ol:
				if o.isimpl: continue
				if o.protected:
					if o.static:
						f.write("\t\tstatic %s base_%s(%s) {\n" % (o.retval, m.name, o.args.asDecl()))
					else:
						f.write("\t\tstatic %s base_%s(%s* self%s) {\n" % (o.retval, m.name, c.fullCoreName(), o.args.asDecl(True)))
					if (o.retval == 'void'): f.write("\t\t\t")
					else: f.write("\t\t\treturn ")
					if o.static:
						f.write("%s::%s(%s);\n" % (c.fullCoreName(), m.name, o.args.asArgs()))
					else:
						f.write("((%s_shell*)self)->p_%s(%s);\n" % (c.name, m.name, o.args.asArgs()))
					f.write("\t\t}\n\n")
				

		
		f.write("\t\t/**** implementation of virtual method dispatchers ****/\n")
		for m in c.methods.values():
			for o in m.ol:
				if o.virtual:
					comma = ""
					if len(o.args.args) != 0: comma = ", "
					f.write("\t\tstatic %s static_%s(PyCPPClassInstance* me%s%s);\n"%(o.retval, m.name, comma, o.args.asDecl()))
	
	
		f.write("\tprotected:\n")
		f.write("\t\t/**** virtual methods ****/\n")
		if c.constructor:
			for m in c.inheritedmethods.values()+c.methods.values():
				for o in m.ol:
					if o.virtual:
						cst = ""
						if o.const: cst = " const"
						f.write("\t\tvirtual %s %s(%s)%s;\n" % (o.retval, m.name, o.args.asDecl(), cst))

		if c.isWObject:
			f.write("\t\tvirtual void setParent(Wt::WObject*);\n")
	
		if c.isWStandardItem:
			f.write("\t\tvirtual void setModel(Wt::WStandardItemModel*);\n")
	
	
		f.write("};\n\n\n")

	f.write("\n\n\n")
	f.write("#endif\n\n\n")
	f.close()

	fname = moduledir + "/" + shellCFile(c.name)
	f = open(fname, 'w')
	if c.final:
		f.close()
		return

	for m in c.methods.values():
		for o in m.ol:
			if o.virtual:
				comma = ""
				if len(o.args.args) != 0: comma = ", "
				f.write("\t\t%s %s_shell::static_%s(PyCPPClassInstance* me%s%s) {\n"%(o.retval, c.name, m.name, comma, o.args.asDecl()))
	
				f.write("\t\t\tPyObject* f = PyObject_GetAttrString(me, \"%s\");\n" % m.pyname)
				f.write("\t\t\tif (PyErr_Occurred()) throw PyException();\n")
				i = 0
				for a in o.args.args:
					i = i+1
					f.write("\t\t\tPyObject* %sp = C2Python< %s >::arg(%s);\n" % (a[1], a[0], a[1]))
				f.write("\t\t\tPyObject* _args = PyTuple_New(%d);\n" % i)
				i = 0
				for a in o.args.args:
					f.write("\t\t\tPyTuple_SetItem(_args, %d, %sp);\n" % (i, a[1]))
					i = i+1
	
				f.write("\t\t\tPyObject* pyr = PyObject_Call(f, _args, 0);\n")
				f.write("\t\t\tPy_XDECREF(_args);\n")
				f.write("\t\t\tPy_XDECREF(f);\n")
				f.write("\t\t\tif (!pyr) {throw PyException();}\n")
				for a in o.args.args:
					if a[2].has_key('ownresultif'):
						f.write("\t\t\tif (%s(%s.a)) ((PyCPPClassInstance*)pyr)->setCOwnership();\n" % (a[2]['ownresultif'], a[1]))
				if (o.retval == 'void'):
					#TODO: check for none
					f.write("\t\t\tPy_XDECREF(pyr);\n")
				else:
					f.write("\t\t\t%s r = Python2C< %s >::ret(pyr);\n" % (o.retval, o.retval))
					f.write("\t\t\tPy_XDECREF(pyr);\n")
					f.write("\t\t\treturn r;\n")

				f.write("\t\t}\n\n")

	if c.constructor:
		for m in c.inheritedmethods.values()+c.methods.values():
			for o in m.ol:
				if o.virtual:
					cst = ""
					if o.const: cst = " const"
					f.write("%s %s_shell::%s(%s)%s {\n" % (o.retval, c.name, m.name, o.args.asDecl(), cst))
					comma = ""
					if (len(o.args.args) > 0): comma = ", "
					if (o.retval != 'void'): 
						f.write("\t%s rv = %s_shell::static_%s(pyobj%s%s);\n" % (o.retval, m.baseclass.name, m.name, comma, o.args.asArgs()))
						f.write("\treturn rv;\n");
					else:
						f.write("\t%s_shell::static_%s(pyobj%s%s);\n" % (m.baseclass.name, m.name, comma, o.args.asArgs()))
		
					f.write("}\n\n")
	if c.isWObject:
		f.write("void %s_shell::setParent(Wt::WObject* p) {\n" % c.name)
		f.write("\t%s::setParent(p);\n" % c.name)
		f.write("\tpyobj->proxy->setPythonOwnership(!hasParent());\n")
		f.write("}\n")
	
	if c.isWStandardItem:
		f.write("void %s_shell::setModel(Wt::WStandardItemModel* p) {\n" % c.name)
		f.write("\tpyobj->proxy->setPythonOwnership((p == 0));\n")
		f.write("\t%s::setModel(p);\n" % c.name)
		f.write("}\n")

	f.close()

def traverse(c, classlist, virtuals, protected):
	for baseclass in c.baseclass: 
		if not c in baseclass.derived: baseclass.derived.append(c)
		traverse(baseclass, classlist, virtuals, protected)

		c.inheritedmethods = baseclass.methods.copy()
		c.inheritedmethods.update(baseclass.inheritedmethods)
		#remove methods redefined in this class
		for m in c.methods.keys():
			if m in c.inheritedmethods.keys():
				del c.inheritedmethods[m]
#else:
#		 c.inheritedmethods = {}

def calcInheritance(classlist, globlist):
	for c in classlist:
		c.baseclass = []
	for c in classlist:
		for basename in c.base:
			baseclass = None
			for v in classlist+globlist:
				if v.name == basename: 
					baseclass = v
				else:
					for w in  v.innerclasses:
						if w == basename:
							baseclass = v.innerclasses[w]
			if baseclass != None: c.baseclass.append(baseclass)


def findInheritance(c, classlist, globlist):
	virtuals = {}
	protecteds = {}
	print "Processing", c.name
	traverse(c, classlist+globlist, virtuals, protecteds)


def checkWObject(aclass):
	if hasattr(aclass, 'isWObject'):
		return aclass.isWObject
	else:
		if aclass.name == "WObject":
			aclass.isWObject = True
			return True
		elif aclass.baseclass == []:
			aclass.isWObject = False
			return False
		else: 
			aclass.isWObject = False
			for base in aclass.baseclass:
				aclass.isWObject = aclass.isWObject or checkWObject(base)
			return aclass.isWObject


def checkWStandardItem(aclass):
	if hasattr(aclass, 'isWStandardItem'):
		return aclass.isWStandardItem
	else:
		if aclass.name == "WStandardItem":
			aclass.isWStandardItem = True
			return True
		elif aclass.baseclass == []:
			aclass.isWStandardItem = False
			return False
		else: 
			aclass.isWStandardItem = False
			for base in aclass.baseclass:
				aclass.isWStandardItem = aclass.isWStandardItem or aclass.isWStandardItem or checkWStandardItem(base)
			return aclass.isWStandardItem


def findInClass(m, ol, c, debug):
	if debug: print "looking in ", c.name
	# if this base class has the method, search for an identical
	# documented overload
	if c.methods.has_key(m.name):
		if debug: print "found method"
		for ol2 in c.methods[m.name].ol:
			if ol == ol2 and not ol2.isimpl: 
				if debug: print "found documented overload"
				ol.doc = ol2.doc #inherit the documentation
				return True
		return False

	# otherwise look deeper
	for b in c.baseclass:
		if findInClass(m, ol, b, debug): return True
	return False


			
def validateImplementations(c):
	debug = False
	for m in c.methods.values():
#		if (c.name == "WAbstractItemView" and m.name == "mouseWentUp"): debug = True
		for ol in m.ol:
			if ol.isimpl:
				if debug: print "Looking for ", m.name, ol.asPySignature()
				# this is a undocumented method
				#treat it as  documented if it is documented
		    	# in one of the bases
				found = False
				for b in c.baseclass:
					if findInClass(m, ol, b, debug): 
						found = True;
						break
				if found:
					ol.isimpl = 0
	# now remove all overloads which are still marked with isimpl
	for m in c.methods.values():
		ol2 = []
		for ol in m.ol:
			if not ol.isimpl: ol2.append(ol)
		m.ol = ol2
		m.update()
			

def assignComparisonFunc(c):
	if c.operators.has_key('==') and c.operators.has_key('<'):
		c.comparefunc = "compare_val_eq_lt<%s>" % c.fullCoreName()
	elif c.operators.has_key('=='):
		c.comparefunc = "compare_val_eq<%s>" % c.fullCoreName()
	else:
		c.comparefunc = "compare_ptr" 



def Generate(enumlist, classlist, prefix):

	global globlist
	if only == "" or only == prefix:
		generateGlobalEnumDecl(enumlist, prefix)
	for c in classlist:
		calcInheritance(classlist, globlist)
	classlist = sortClasslist(classlist)

	for c in classlist:
		if not c.constructor:
			c.instanciatable = False

#for c in classlist:
#		if c.min:
#			while c.baseclass:
#				c.baseclass.min = 1
#				c = c.baseclass

	# set derived classes
	for c in classlist:
		for base in c.baseclass:
			base.childs.append(c)

	#wt specific code
	for c in classlist:
		checkWObject(c)
		checkWStandardItem(c)

	import assignProxyType
	for c in classlist:
		assignProxyType.assign(c)

	for c in classlist:
		assignComparisonFunc(c)

	import defineTemplateTypes
	ttypes = defineTemplateTypes.defineTemplateTypes(classlist)

	for c in classlist:
		c.resolveUsing()

	for c in classlist:
		validateImplementations(c)

#	for c in classlist:
#		for n, m in c.methods.items()[:]:
#			if m.ol[0].isimpl:
#				#print "Removing %s::%s" % (c.name, n)
#				del c.methods[n]
#			else:
#				pass
#				#print "Keeping %s::%s" % (c.name, n)

	for c in classlist:
		findInheritance(c, classlist, globlist)

	if only == "" or only == prefix:

		for c in classlist:
			if minonly and not c.min: continue
			print "Generating ", c.name
			generateShell(c)
			generatePyObject(c)

		generateIncludes(classlist, prefix)
		generateConversions(classlist, enumlist, prefix)
		generateStrings(classlist, enumlist, prefix)
		generateTypeDeclarations(classlist, prefix)
		generateProxyTypes(classlist, prefix)
		generateCompareFuncs(classlist, prefix)

	generateTemplateTypeDefs(ttypes, prefix)

	globlist = globlist + classlist




def createUnit(descrdir, destdir, prefix):

	flist = getEnumFiles(descrdir)
	#flist = glob.glob("%s/*.enum" % descrdir)
	enums = []
	for f in flist:
		print "Reading", f
		execfile(f)



	previous = ""
	#flist = glob.glob("%s/*.cl" % descrdir)
	flist = getClassFiles(descrdir)
	classlist = []
	globs = {}
	for f in flist:
		print "Reading", f
		execfile(f, globs)
		if ((not globs.has_key('c')) or globs['c'].name == previous):
			print "   WARNING: No class defined"
			continue
		classlist.append(globs['c'])
		previous = globs['c'].name

	#add inner classes to the list
	innerclasses = []
	for c in classlist:
		innerclasses = innerclasses + c.innerclasses.values()
	classlist = classlist + innerclasses
		
	
	import patchClasses
	classmap = {}
	for c in classlist:
		classmap[c.name] = c
	patchClasses.patch(prefix, classmap)
	classlist = classmap.values()


	global moduledir
	moduledir = destdir
	import os
	os.system("mkdir -p %s" % destdir)
	Generate(enums, classlist, prefix)




if __name__ == "__main__":
	print "EXECUTING"
	import glob
	import sys



	root = sys.argv[1]

	# check root dir
	flist = glob.glob("%s/descr/*.cl" % root)
	if len(flist) == 0:
		print "Wrong dir"
		sys.exit(1)

	if only == "": GenAutoFunctions.write(root + "/src/python")
	createUnit(root + "/descr/Http", root + "/src/generated/wt/http", "wthttp_")
	createUnit(root + "/descr", root + "/src/generated/wt", "wt_")
	createUnit(root + "/descr/Chart", root + "/src/generated/wt/chart", "wtchart_")
	GenAutoFunctions.write(root + "/src/python")

