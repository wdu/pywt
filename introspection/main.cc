#include "clang-c/Index.h"

#include <iostream>
#include <fstream>
#include <sstream>
// TODO:
// WRasterImage::measureText(): last argument has no default value annotated
//   but one-but-last argument has one
// WLocalizedString::resolveKey: comment extraction is confused
// WGLWidget: > 255 arguments

// the missing functions in libclang
extern "C" {
  CXString clang_getTypeAsString(CXType CT);
  CXString clang_getDefaultValueAsString(CXCursor C);
  bool clang_isPolymorphic(CXType CT);
  bool clang_isAbstractFunction(CXCursor C);
  bool clang_isConstMethod(CXType CT);
  bool clang_isAbstractClass(CXType CT);
}

CXTranslationUnit TU;

struct VisitorContext {
  VisitorContext(const std::string &ns = "", const std::string &classname = "",
		 std::ostream &o = *(new std::ofstream())):
    nsname(ns),
    classname(classname),
    o(o),
    first(true)
  {}
  std::string nsname;
  std::string classname;
  std::ostream &o;
  CX_CXXAccessSpecifier access;
  bool first;
};

std::string stripClass(const std::string &s)
{
  if (s.substr(0, 6) == "class ")
    return s.substr(6, std::string::npos);
  if (s == "_Bool")
    return "bool";
  if (s.find("_Bool") != std::string::npos) {
    std::string retval = s;
    while (retval.find("_Bool") != std::string::npos) {
      unsigned i = retval.find("_Bool");
      retval[i] = ' ';
      retval[i+1] = 'b';
    }
    return retval;
  }
  return s;
}

std::string typeString(CXType type)
{
  std::string s = clang_getCString(clang_getTypeAsString(type));
  std::stringstream ss;
  if (s.find("vector") != std::string::npos)
    ss << clang_getCString(clang_getTypeAsString(type));
  if (type.kind == CXType_Typedef) {
    // TODO: didn't I fix this?
    // local defined methods are not prefixed by the class in which they
    // appear. Not good.
    // (const) Field, Method (WObject),
    if (s == "Method")
      s = "Wt::WObject::Method";
    else if (s == "DataMap")
      s = "Wt::WAbstractItemModel::DataMap";
    else if (s == "Field")
      s = "Wt::WFormModel::Field";
    else if (s == "const Field")
      s = "const Wt::WFormModel::Field";
  }
  return stripClass(s);
}

std::string getCursorFilename(CXCursor cursor)
{
  CXSourceLocation location;
  CXSourceRange range = clang_getCursorExtent(cursor);
  location = clang_getRangeStart(range);
  CXString headerfile;
  clang_getPresumedLocation(location, &headerfile, 0, 0);
  std::string file = clang_getCString(headerfile);
  int pos = file.rfind("/Wt/");
  if (pos != std::string::npos)
    return file.substr(pos + 1);
  else
    return file;
}

std::string typeString(CXCursor cursor)
{
  return typeString(clang_getCursorType(cursor));
}

std::string docString(CXCursor cursor)
{
  std::stringstream co;
  CXComment comment = clang_Cursor_getParsedComment(cursor);
  if (clang_Comment_getKind(comment) == CXComment_FullComment) {
    co << "\"\"\"\n";
    co << clang_getCString(clang_FullComment_getAsHTML(comment));
    co << "\n\"\"\"";
  } else {
    if (clang_getCString(clang_Cursor_getRawCommentText (cursor))) {
      std::string comment2 = clang_getCString(clang_Cursor_getRawCommentText (cursor));
      if (comment2 != "") {
	co << "\"\"\"\n";
	co << comment2;
	co << "\n\"\"\"";
      } else {
	co << "\"\"";
      }
    } else {
      co << "\"\"";
    }
  }
  return co.str();
}

bool hasDoc(CXCursor cursor)
{
  CXComment comment = clang_Cursor_getParsedComment(cursor);
  return clang_Comment_getKind(comment) != CXComment_Null;
  return clang_Comment_getKind(comment) == CXComment_FullComment;
}

void dumpClass(CXCursor cursor, VisitorContext *c, bool inner);

// visitor that just prints information
enum CXChildVisitResult print_visitor(CXCursor cursor, CXCursor parent,
				      CXClientData client_data)
{
  std::cout << "cursor print: "
	    << clang_getCString(clang_getCursorSpelling(cursor))
	    << ", type " << clang_getCursorKind(cursor) << std::endl;
  return CXChildVisit_Recurse;
}

enum CXChildVisitResult type_visitor(CXCursor cursor, CXCursor parent,
				     CXClientData client_data)
{
  std::cout << "cursor print: " 
	    << clang_getCString(clang_getCursorSpelling(cursor))
	    << ", type " << clang_getCursorKind(cursor) << std::endl;
  return CXChildVisit_Recurse;
}

void print_retval(CXCursor cursor)
{
  CXSourceRange range = clang_getCursorExtent(cursor);
  unsigned numtokens;
  CXToken *tokens;
  clang_tokenize(TU, range, &tokens, &numtokens);
  for (unsigned i = 0; i < numtokens; ++i) {
    std::cout << clang_getCString(clang_getTokenSpelling(TU, tokens[i])) << " ";
  }
}

void print_param(CXCursor cursor, std::ostream &o)
{
  std::string varname = clang_getCString(clang_getCursorSpelling(cursor));
  
  CXSourceRange range = clang_getCursorExtent(cursor);
  unsigned numtokens;
  CXToken *tokens;
  clang_tokenize(TU, range, &tokens, &numtokens);
  int defaultTokenStart = 0;
  int defaultTokenEnd = 0;
  // This tokensequence needs some cleanup. There may be a default value
  // included, and sometimes there are extra tokens.
  for (unsigned int i = 0; i < numtokens; ++i) {
    if (clang_getTokenKind(tokens[i]) == CXToken_Punctuation) {
      std::string p = clang_getCString(clang_getTokenSpelling(TU, tokens[i]));
      if (p == "=") {
	defaultTokenStart = i + 1;
	defaultTokenEnd = numtokens;
	numtokens = i;
      }
    }
  }
  
  if (varname.empty()) {
    o << "\"" << typeString(cursor) << "\"";
    return;
  }
  
  std::string type = typeString(cursor);
  o << "(\"" <<  type;
  o << "\", \"" << varname;
  o << "\"";
  std::string defaultValue =
    clang_getCString(clang_getDefaultValueAsString(cursor));
  if (defaultValue == "Pixel") // TODO: fixed, I believe
    defaultValue = "Wt::WLength::Pixel";
  if (defaultValue != "") {
    o << ", {'default': r" << (type == "char" ? "\"" : "'")
      << defaultValue << (type == "char" ? "\"" : "'") << "}";
  }
  o << ")" << std::flush;
}

void print_param_list(CXCursor cursor, std::ostream &o)
{
  o << "[";
  for (unsigned i = 0; i < clang_Cursor_getNumArguments(cursor); ++i) {
    if (i != 0)
      o << ",";
    print_param(clang_Cursor_getArgument(cursor, i), o);
  }
  o << "]";
  
}

enum CXChildVisitResult enum_visitor(CXCursor cursor, CXCursor parent,
				     CXClientData client_data)
{
  VisitorContext *c = (VisitorContext *)client_data;
  switch (clang_getCursorKind(cursor)) {
  case CXCursor_EnumConstantDecl:
    c->o << ", \"" << clang_getCString(clang_getCursorSpelling(cursor)) << "\"";
    break;
  default:
    std::cout << "enum member "
	      << clang_getCString(clang_getCursorSpelling(cursor))
	      << ", type " << clang_getCursorKind(cursor) << std::endl;
    break;
  }
  return CXChildVisit_Continue;
}

enum CXChildVisitResult enum_doc_visitor(CXCursor cursor, CXCursor parent,
					 CXClientData client_data)
{
  VisitorContext *c = (VisitorContext *)client_data;
  switch (clang_getCursorKind(cursor)) {
  case CXCursor_EnumConstantDecl:
    {
      c->o << ", " << docString(cursor);
    }
    break;
  default:
    std::cout << "enum member "
	      << clang_getCString(clang_getCursorSpelling(cursor))
	      << ", type " << clang_getCursorKind(cursor) << std::endl;
    break;
  }
  return CXChildVisit_Continue;
}

void member_enum(CXCursor cursor, std::ostream &co)
{
  std::string enumname = clang_getCString(clang_getCursorSpelling(cursor));
  co << "c.addEnum(\"" << enumname << "\"";
  VisitorContext cc("", "", co);
  clang_visitChildren(cursor, enum_visitor, (void *)&cc);
  co << ")\n";
  co << "c[\"" << enumname << "\"].setDocumentation([" << docString(cursor);
  clang_visitChildren(cursor, enum_doc_visitor, (void *)&cc);
  co << "])\n";
  
}

enum CXChildVisitResult using_visitor(CXCursor cursor, CXCursor parent,
				      CXClientData client_data)
{
  VisitorContext *c = (VisitorContext *)client_data;
  switch (clang_getCursorKind(cursor)) {
  case CXCursor_TypeRef:
  case CXCursor_OverloadedDeclRef:
    if (c-> first)
      c->first = false;
    else
      c->o << ", ";
    c->o << "\""
	 << stripClass(clang_getCString(clang_getCursorSpelling(cursor)))
	 << "\"";
    break;
  default:
    std::cout << "using_visitor: weird using structure\n";
    break;
  }
  return CXChildVisit_Continue;
}

// visits class members for a class being dumped
enum CXChildVisitResult member_visitor(CXCursor cursor, CXCursor parent,
				       CXClientData client_data)
{
  // public, private, protected
  VisitorContext *c = (VisitorContext *)client_data;
  switch (clang_getCursorKind(cursor)) {
  case CXCursor_CXXAccessSpecifier:
    c->access = clang_getCXXAccessSpecifier(cursor);
    break;
    // inner class?
  case CXCursor_StructDecl:
  case CXCursor_ClassDecl:
    {
      // TODO: probably the ifdef guarding around the comments confuse the
      // doc extractor
      std::string innerClassName =
	clang_getCString(clang_getCursorSpelling(cursor));
      if (!hasDoc(cursor) && innerClassName != "UpdateLock") {
	c->o << "# skipping inner class " << innerClassName << std::endl;
	break;
      }
      
      if (clang_isCursorDefinition(cursor) && c->access != CX_CXXPrivate) {
	std::cout << "Inner class declaration: " << innerClassName << std::endl;
        c->o << "# inner class " << innerClassName << std::endl;
        c->o << "base = c\n"
	     << "c = None\n";
	VisitorContext cc(c->nsname /*+ "::" + c->classname*/, innerClassName, c->o);
        if (clang_getCursorKind(cursor) == CXCursor_StructDecl)
	  cc.access = CX_CXXPublic;
	dumpClass(cursor, &cc, true);
        c->o << "if c: base.addClass(c)\n"
	     << "c = base\n";
      }
    }
    break;
  case CXCursor_Constructor:
    std::cout << "constructor " << clang_getCString(clang_getCursorSpelling(cursor)) << std::endl;
    if (!hasDoc(cursor) && (std::string(clang_getCString(clang_getCursorSpelling(cursor))) != "WApplication")) {
      c->o << "# skipping constructor ";
      print_param_list(cursor, c->o);
      c->o << std::endl;
    } else {
      if (c->access == CX_CXXProtected || clang_isAbstractClass(clang_getCursorType(parent))) {
	c->o << "c.setBaseConstructor(";
      } else {
	c->o << "c.setConstructor(";
      }
      print_param_list(cursor, c->o);
      c->o << ", \"";
      if (c->access == CX_CXXProtected)
	c->o << "p";
      c->o << "\")\n";
      c->o << "c.constructor.setDocumentation(" << docString(cursor) << ")\n";
    }
    break;
  case CXCursor_CXXMethod:
    {
      std::string methodname =
	clang_getCString(clang_getCursorSpelling(cursor));
      std::string methodtype = typeString(cursor);
      std::cout << methodname << ": " << methodtype << std::endl;
      if (methodname.substr(0, 8) == "operator") {
	c->o << "c.addOperator(\""
	     << methodname.substr(8, std::string::npos) << "\"";
      } else {
	c->o << "c.addMethod(\"" << methodname << "\"";
      }
      // TODO: can we avoid this call to getCanonicalType here? some typedefs
      // cause problems
      c->o << ", \"" << typeString(clang_getResultType(/*clang_getCanonicalType*/(clang_getCursorType(cursor)))) << "\", ";
      print_param_list(cursor, c->o);
      c->o << ", ";
      c->o << "\"";
      if (c->access == CX_CXXProtected)
	c->o << "p";
      if (c->access == CX_CXXPrivate)
	c->o << "P";
      if (clang_isAbstractFunction(cursor))
	c->o << "a";
      if (clang_CXXMethod_isVirtual(cursor))
	c->o << "v"; // Virtual
      if (clang_isConstMethod(clang_getCursorType(cursor)))
	c->o << "c"; // Const
      if (clang_CXXMethod_isStatic(cursor))
	c->o << "s"; // Static
      if (!hasDoc(cursor))
	c->o << "i";
      c->o << "\")\n";
      c->o << "c[\"" << clang_getCString(clang_getCursorSpelling(cursor))
	   << "\"].setDocumentation(" << docString(cursor) << ")\n";
      
    }
    break;
  case CXCursor_EnumDecl:
    if (hasDoc(cursor))
      member_enum(cursor, c->o);
    else
      c->o << "# skipping enum "
	   << clang_getCString(clang_getCursorSpelling(cursor)) << "\n";
    break;
  case CXCursor_UsingDeclaration:
    {
      c->o << "c.addUsing(";
      VisitorContext cc(c->nsname, c->classname, c->o);
      clang_visitChildren(cursor, using_visitor, &cc);
      c->o << ", \"\")\n";
    }
    break;
  case CXCursor_FieldDecl: // member vars
    if (hasDoc(cursor)) {
      c->o << "c.addNonStaticMember(\""
	   << clang_getCString(clang_getCursorSpelling(cursor)) << "\", \"";
      c->o << typeString(cursor) << "\")\n";
      c->o << "c[\""
	   << clang_getCString(clang_getCursorSpelling(cursor))
	   << "\"].setDocumentation(" << docString(cursor) << ")\n";
    } else {
      c->o << "# skipping non-static member "
	   << clang_getCString(clang_getCursorSpelling(cursor)) << std::endl;
    }
    break;
  case CXCursor_VarDecl: // static member vars
    if (hasDoc(cursor)) {
      c->o << "c.addStaticMember(\""
	   << clang_getCString(clang_getCursorSpelling(cursor)) << "\", \"";
      c->o << typeString(cursor) << "\")\n";
      c->o << "c[\""
	   << clang_getCString(clang_getCursorSpelling(cursor))
	   << "\"].setDocumentation(" << docString(cursor) << ")\n";
    } else {
      c->o << "# skipping static member "
	   << clang_getCString(clang_getCursorSpelling(cursor)) << std::endl;
    }
    break;
  case CXCursor_Destructor:
    if (c->access == CX_CXXPrivate) {
      c->o << "#class is final because of private destructor\n";
      c->o << "c.final = True\n";
      c->o << "c.privatedestructor = True\n";
      
    }
    break;
  case CXCursor_CXXBaseSpecifier: // what we inherit from; handled elsewhere
  case CXCursor_UnexposedDecl: // e.g. friend declarations
  case CXCursor_TypedefDecl:
    // not interested
    break;
  default:
    std::cout << "class member "
	      << clang_getCString(clang_getCursorSpelling(cursor))
	      << ", type " << clang_getCursorKind(cursor) << std::endl;
  }
  return CXChildVisit_Continue;
}

enum CXChildVisitResult inheritance_visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
  VisitorContext *c = (VisitorContext *)client_data;
  switch (clang_getCursorKind(cursor)) {
  case CXCursor_CXXBaseSpecifier:
    {
      std::string baseclass = clang_getCString(clang_getCursorSpelling(cursor));
      if (baseclass.find("Wt::") == std::string::npos)
	break;
      if (c-> first)
	c->first = false;
      else
	c->o << ", ";
      if (baseclass.substr(0, 6) == "class ")
	baseclass = baseclass.substr(6);
      c->o << "\"" << baseclass << "\"";
    }
    break;
  default:
    break;
  }
  return CXChildVisit_Continue;
}

// used for both struct and class
void dumpClass(CXCursor cursor, VisitorContext *c, bool inner)
{
  std::string classname = clang_getCString(clang_getCursorSpelling(cursor));
  std::cout << "Class " << c->nsname << "::" << classname << std::endl;
  std::ostream *co;
  if (!inner) {
    std::string header = getCursorFilename(cursor);
    int begin = header.rfind("/Wt/");
    int end = header.rfind("/");
    std::string subdir;
    if (begin + 4 < end)
      subdir = header.substr(begin + 4, end - begin - 4);
    mkdir(("descr/" + subdir).c_str(), 0777);
    co = new std::ofstream(("descr/" + subdir + "/" + classname + ".cl").c_str());
  } else {
    co = &c->o;
  }
  (*co) << "# Generated Python script for class " << classname << std::endl;
  (*co) << "from CClass import *\n";
  (*co) << "c = CClass(\"" << classname << "\", [";
  VisitorContext cc_inh(c->nsname, c->classname, *co);
  clang_visitChildren(cursor, inheritance_visitor, &cc_inh);
  (*co) << "], \"" << c->nsname << "\");\n";
  
  *co << "c.corefile = \"" << getCursorFilename(cursor) << "\"\n";
  if (!clang_isPolymorphic(clang_getCursorType(cursor))) {
    *co << "c.nonPolymorphic()\n";
  }
  *co << "c.setDocumentation(" << docString(cursor) << ")\n";
  VisitorContext cc(c->nsname, c->classname, *co);
  if (clang_getCursorKind(cursor) == CXCursor_StructDecl)
    cc.access = CX_CXXPublic;
  else
    cc.access = CX_CXXPrivate;
  clang_visitChildren(cursor, member_visitor, &cc);
  if (!inner)
    delete co;
  std::cout << "End class " << c->nsname << "::" << classname << std::endl;
}

void dumpEnum(CXCursor cursor, VisitorContext *c)
{
  std::string enumname = clang_getCString(clang_getCursorSpelling(cursor));
  std::string header = getCursorFilename(cursor);
  int begin = header.rfind("/Wt/");
  int end = header.rfind("/");
  std::string subdir;
  if (begin + 4 < end)
    subdir = header.substr(begin + 4, end - begin - 4);
  mkdir(("descr/" + subdir).c_str(), 0777);
  std::ofstream co(("descr/" + subdir + "/" + enumname + ".cl").c_str());
  co << "# Generated Python script for enum " << enumname << std::endl;
  co << "from CClass import *\n";
  co << "e = Enum(\"" << enumname << "\"";
  VisitorContext cc(c->nsname, c->classname, co);
  clang_visitChildren(cursor, enum_visitor, &cc);
  co << ")\n";
  
  co << "e.corefile = \"" << getCursorFilename(cursor) << "\"\n";
  co << "e.setNamespace(\"" << c->nsname << "\")\n";
  co << "e.setDocumentation([" << docString(cursor);
  clang_visitChildren(cursor, enum_doc_visitor, &cc);
  co << "])\n";
  co << "enums.append(e)\n";
}

// Visits Wt namespace
enum CXChildVisitResult Wt_visitor(CXCursor cursor,
				   CXCursor parent,
				   CXClientData client_data)
{
  VisitorContext *c = (VisitorContext *)client_data;
  switch (clang_getCursorKind(cursor)) {
  case CXCursor_StructDecl:
  case CXCursor_ClassDecl:
    if (clang_isCursorDefinition(cursor)) {
      std::string classname = clang_getCString(clang_getCursorSpelling(cursor));
      std::cout << "Class declaration: "
		<< c->nsname << "::" << classname << std::endl;
      dumpClass(cursor, c, false);
    }
    break;
  case CXCursor_EnumDecl: 
    {
      std::string enumname = clang_getCString(clang_getCursorSpelling(cursor));
      std::cout << "Enum declaration: " << enumname << std::endl;
      dumpEnum(cursor, c);
    }
    break;
  case CXCursor_Namespace: {
    std::string nsname = clang_getCString(clang_getCursorSpelling(cursor));
    std::string new_namespace = c->nsname + "::" + nsname;
    std::cout << "Namespace: " << new_namespace << std::endl;
    // visit all sub-namespaces
    VisitorContext cc(new_namespace, "");
    clang_visitChildren(cursor, Wt_visitor, &cc);
  }
    break;
  default:
    std::cout << "Wt::stuff " << clang_getCString(clang_getCursorSpelling(cursor)) << ", type " << clang_getCursorKind(cursor) << std::endl;
    break;
  }
  return CXChildVisit_Continue;
}

enum CXChildVisitResult TU_visitor(CXCursor cursor,
				   CXCursor parent,
				   CXClientData client_data)
{
  //std::cout << (int)clang_getCursorKind(cursor) << std::endl;
  switch (clang_getCursorKind(cursor)) {
  case CXCursor_Namespace: {
    std::string nsname = clang_getCString(clang_getCursorSpelling(cursor));
    //std::cout << "Namespace: " << nsname << std::endl;
    VisitorContext cc(nsname);
    if (nsname == "Wt")
      clang_visitChildren(cursor, Wt_visitor, &cc);
  }
    break;
  default:
    break;
  }
  return CXChildVisit_Continue;
}

int main(int argc, char **argv)
{
  CXIndex Index = clang_createIndex(0, 0);
  
  TU = clang_parseTranslationUnit(Index, 0, argv, argc,
				  0, 0, CXTranslationUnit_None);
  
  
  for (unsigned I = 0, N = clang_getNumDiagnostics(TU); I != N; ++I) {    
    CXDiagnostic Diag = clang_getDiagnostic(TU, I);
    CXString String =
      clang_formatDiagnostic(Diag, 
			     clang_defaultDiagnosticDisplayOptions());
    fprintf(stderr, "%s\n", clang_getCString(String));
    clang_disposeString(String);
  }
  
  
  CXCursor C = clang_getTranslationUnitCursor(TU);
  //std::cout << (int)clang_getCursorKind(C) << std::endl;
  clang_visitChildren(C, TU_visitor, 0);
  
  clang_disposeTranslationUnit(TU);
  
  clang_disposeIndex(Index);
  return 0;
}

