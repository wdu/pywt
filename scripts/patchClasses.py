# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.

def patchWt(clist):
	# The version check happens internal, not visible in script
	clist["WApplication"].constructor = None
	clist["WApplication"].setConstructor([("const class Wt::WEnvironment &", "environment")], "")

	# constructors with argument types from the pdf lib
	for ol in clist["WPdfImage"].constructor.ol:
			if len(ol.args.args) > 0 and ol.args.args[0][1].find("pdf") != -1:
				clist["WPdfImage"].constructor.ol.remove(ol)
	clist["WPdfImage"].constructor.update()


	# methods with python incompatible signature
	del clist["WLocalizedStrings"].methods["resolveKey"] #string&
	del clist["WObject"].methods["getStateless"] # uses method pointer
	del clist["WWidget"].methods["getStateless"] # uses method pointer // FIXME: "i"

	# There is a to be investigated problem with tooltips... FIXME
	del clist["WWidget"].methods["toolTip"]
	del clist["WWebWidget"].methods["toolTip"]
	del clist["WCompositeWidget"].methods["toolTip"]

	del clist["WFormWidget"].methods["getStateless"] # uses method pointer
	del clist["WAbstractToggleButton"].methods["getStateless"] # uses method pointer
	del clist["WEnvironment"].methods["getParameter"] #std::string*
	del clist["WEnvironment"].methods["cookies"] #FIXME
	del clist["WEnvironment"].methods["getCookieValue"] #std::string*
	del clist["WApplication"].methods["readConfigurationProperty"] #string&*

	del clist["WMemoryResource"].methods["setData"] # const unsigned char*, which
	                                                #actually is a data buffer
	del clist["WMemoryResource"].methods["data"] # const vector<char> &

	# We want to support this at one point in the future...
	del clist["WInteractWidget"].methods["gestureStarted"] 
	del clist["WInteractWidget"].methods["gestureChanged"] 
	del clist["WInteractWidget"].methods["gestureEnded"] 
	del clist["WWebWidget"].methods["gestureEventSignal"] 
	del clist["WInteractWidget"].methods["touchStarted"] 
	del clist["WInteractWidget"].methods["touchMoved"] 
	del clist["WInteractWidget"].methods["touchEnded"] 
	del clist["WWebWidget"].methods["touchEventSignal"] 


	# Need extra work to support raw indexes
	del clist["WAbstractItemModel"].methods["createIndex"] #void*
	del clist["WAbstractItemModel"].methods["fromRawIndex"] #void*
	del clist["WAbstractItemModel"].methods["toRawIndex"] #void*
	del clist["WAbstractProxyModel"].methods["fromRawIndex"] #void*
	del clist["WAbstractProxyModel"].methods["toRawIndex"] #void*
	del clist["WStandardItemModel"].methods["fromRawIndex"] #void*
	del clist["WStandardItemModel"].methods["toRawIndex"] #void*

	del clist["WAbstractProxyModel"].methods["createSourceIndex"] #void *
	del clist["WModelIndex"].methods["internalPointer"] #void*
	del clist["WModelIndex"].methods["encodeAsRawIndexes"] #set<>&
	del clist["SignalBase"].methods["connect"]
	del clist["EventSignalBase"].using["connect"]
	del clist["EventSignalBase"].methods["name"]
	del clist["WLayout"].methods["getContentsMargins"] #int*

	del clist["WEnvironment"].methods["server"] # return type WAbstractServer is not public

	# may be a problem memory wise
	del clist["WFormModel"].methods["value"] # FIXME: currently problematic
	del clist["WFormModel"].methods["fields"] # FIXME: currently problematic
	del clist["WTemplate"].methods["addFunction"] # FIXME: currently problematic
	del clist["WAnchor"].methods["ref"] # FIXME: currently problematic
	del clist["WMenu"].methods["items"] # FIXME: std::vector<>
	del clist["WTemplateFormView"].methods["setFormWidget"] # FIXME
	del clist["WTextEdit"].methods["configurationSetting"] # FIXME



   #BaseItem is a protected class. This is currently not supported
    #problem is in the Python - C conversion functions, which should
    #be embedded in a class derived from WABstractProxyModel, in order
    #to be able to access this class
	del clist["WAbstractProxyModel"].innerclasses["BaseItem"]
	del clist["WAbstractProxyModel_BaseItem"]
	del clist["WAbstractProxyModel"].methods["shiftModelIndexes"]


	#void *
	#this is partial solution:
	# it causes a mem leak when a WPopupMenuItem is deleted with non-zero data
	#clist["WPopupMenuItem"].methods["setData"].ol[0].beforecall = "Py_XINCREF((PyObject*)a1c.v); Py_XDECREF((PyObject*)(me->data()));"



	# translate boost gregorian date etc to something python-like (TODO)
	for ol in clist["WDate"].constructor.ol:
			if len(ol.args.args) > 0 and ol.args.args[0][1].find("date") != -1:
				clist["WDate"].constructor.ol.remove(ol)
	clist["WDate"].constructor.update()

	del clist["WDateTime"].methods["toPosixTime"] #boost::posix_time::ptime
	del clist["WDateTime"].methods["fromPosixTime"] #boost::posix_time::ptime
	del clist["WDateTime"].methods["setPosixTime"] #boost::posix_time::ptime
	del clist["WDate"].methods["setGregorianDate"] #boost::gregorian::date
	del clist["WDate"].methods["toGregorianDate"] #boost::gregorian::date
	del clist["WCalendar"].methods["dateForCell"] #boost::gregorian::date


    # string &
	for ol in clist["WWebWidget"].methods["escapeText"].ol:
		if len(ol.args.args) == 2 and ol.args.args[0][0] == "std::string &":
			clist["WWebWidget"].methods["escapeText"].ol.remove(ol)
			clist["WWebWidget"].methods["escapeText"].update()

	# Ext class arguments - deprecated so not supported
	for ol in clist["WButtonGroup"].methods["addButton"].ol:
		if len(ol.args.args) == 2 and ol.args.args[0][0] == "Wt::Ext::RadioButton *":
			clist["WButtonGroup"].methods["addButton"].ol.remove(ol)
			clist["WButtonGroup"].methods["addButton"].update()

	for ol in clist["WLabel"].methods["setBuddy"].ol:
		if len(ol.args.args) == 1 and ol.args.args[0][0] == "Wt::Ext::FormField *":
			clist["WLabel"].methods["setBuddy"].ol.remove(ol)
			clist["WLabel"].methods["setBuddy"].update()

	for ol in clist["WButtonGroup"].methods["removeButton"].ol:
		if ol.args.args[0][0] == "Wt::Ext::RadioButton *":
			clist["WButtonGroup"].methods["removeButton"].ol.remove(ol)
	clist["WButtonGroup"].methods["removeButton"].update()

			


	   # version with 3 int&
	for ol in clist["WEnvironment"].methods["libraryVersion"].ol:
		if len(ol.args.args) == 3:
			clist["WEnvironment"].methods["libraryVersion"].ol.remove(ol)
	clist["WEnvironment"].methods["libraryVersion"].update()

	#version with 2 double*
	for ol in clist["WTransform"].methods["map"].ol:
		if len(ol.args.args) == 4:
			clist["WTransform"].methods["map"].ol.remove(ol)
	clist["WTransform"].methods["map"].update()


	# these have a private destructor, so no code should be
	# generated containing 'delete x'
	# this could be done by the introspector
	clist["WTableColumn"].undeletable = True
	clist["WTableRow"].undeletable = True
	clist["WScrollBar"].undeletable = True


	# until there is a good solution
	# reimplement notify would break the python locking/unlocking
	del clist["WApplication"].methods["notify"]

	# will have to be supported
	del clist["WApplication"].methods["theme"] #FIXME
	del clist["WApplication"].methods["setTheme"] #FIXME

	del clist["WGoogleMap"].methods["zoomWindow"] # FIXME


	# impossible to subclass or instancate in python because some
	# abstract methods are not part of the public api
    #could be done by introspector
	clist["WAbstractMedia"].constructor = None
	clist["WAbstractMedia"].instaciatable = None
	clist["WWidget"].constructor = None
	clist["WWidget"].instaciatable = None
	clist["WWebWidget"].constructor = None
	clist["WWebWidget"].instaciatable = None
	clist["WAbstractItemView"].constructor = None
	clist["WAbstractItemView"].instaciatable = None
	clist["WInteractWidget"].constructor = None
	clist["WInteractWidget"].instaciatable = None
	clist["WFormWidget"].constructor = None
	clist["WFormWidget"].instaciatable = None
	clist["WAbstractToggleButton"].constructor = None
	clist["WAbstractToggleButton"].instaciatable = None
	clist["WAbstractSpinBox"].constructor = None
	clist["WAbstractSpinBox"].instaciatable = None
	clist["WAbstractTableModel"].constructor = None
	clist["WAbstractTableModel"].instaciatable = None
	clist["WAbstractListModel"].constructor = None
	clist["WAbstractListModel"].instaciatable = None
	clist["WAbstractItemModel"].constructor = None
	clist["WAbstractItemModel"].instaciatable = None

	# exec is a keyword in python
	clist["WDialog"].methods["exec"].setPyName("execute")


	# this method is static. WMessagebox also inherits a non static
	# show method from WDialog. In python a single method cannot have
	# a static and a non static implementation
	clist["WMessageBox"].methods["popup"] = clist["WMessageBox"].methods["show"]
	del clist["WMessageBox"].methods["show"]
	clist["WMessageBox"].methods["popup"].pyname = "popup"
	clist["WMessageBox"].methods["popup"].name = "popup"



    # remove boost::trackable and boost::noncopyable base classes
	clist["WObject"].base = []
	#clist["WWidget"].base.remove("noncopyable")
	clist["SignalBase"].base = []


	# these generate Signal<some c type>
	clist["WAbstractItemModel"].methods["columnsAboutToBeInserted"].ol[0].virtual = 0
	clist["WAbstractItemModel"].methods["columnsInserted"].ol[0].virtual = 0
	clist["WAbstractItemModel"].methods["columnsAboutToBeRemoved"].ol[0].virtual = 0
	clist["WAbstractItemModel"].methods["columnsRemoved"].ol[0].virtual = 0
	clist["WAbstractItemModel"].methods["rowsAboutToBeInserted"].ol[0].virtual = 0
	clist["WAbstractItemModel"].methods["rowsInserted"].ol[0].virtual = 0
	clist["WAbstractItemModel"].methods["rowsAboutToBeRemoved"].ol[0].virtual = 0
	clist["WAbstractItemModel"].methods["rowsRemoved"].ol[0].virtual = 0
	clist["WAbstractItemModel"].methods["dataChanged"].ol[0].virtual = 0
	clist["WAbstractItemModel"].methods["headerDataChanged"].ol[0].virtual = 0
	clist["WAbstractItemModel"].methods["modelReset"].ol[0].virtual = 0
	clist["WAbstractItemModel"].methods["layoutAboutToBeChanged"].ol[0].virtual = 0
	clist["WAbstractItemModel"].methods["layoutChanged"].ol[0].virtual = 0

	# conficts with special generate implementation
	del clist["WStandardItem"].methods["setModel"]

	# add some extras for special classes
	clist["WLength"].include_extra_decl = "WLength_Xtra_decl.h"
	clist["WLength"].include_extra_impl = "WLength_Xtra_impl.inc"
	clist["WLength"].extra_typedef = """
		tp_repr = (reprfunc)wlength_repr;
	    tp_as_number = &WLength_AsNumber;
	    tp_flags |= (Py_TPFLAGS_HAVE_INPLACEOPS | Py_TPFLAGS_CHECKTYPES);
	"""

	clist["WString"].include_extra_decl = "WString_Xtra_decl.h"
	clist["WString"].include_extra_impl = "WString_Xtra_impl.inc"
	clist["WString"].extra_typedef = """
	    WString_addExtras(this);
	"""

	#rename this class
	clist["WApplication"].corename = "PythonApplication"
	clist["WApplication"].corefile = "PythonApplication.h"
	clist["WApplication"].namespace = ""

	#add an explicit method deleting the underlying C++ class
	#for all WWidget derivatives. This is the equivalent
	#of calling delete in C++
	clist["WWidget"].destructionmethod = "remove"


	# these classes need by value argument passing. This
	# is not generated default for all classes, because it
	# requires a copy constructor
	# probably this can be done automatically in some way
	clist["WDropEvent"].fromC.append('val')
	clist["WDropEvent"].toC.append('val')
	clist["WMouseEvent"].toC.append('val')
	clist["WMouseEvent"].fromC.append('val')
	clist["WScrollEvent"].fromC.append('val')
	clist["WKeyEvent"].fromC.append('val')
	#clist["WTouchEvent"].fromC.append('val')
	clist["Touch"].fromC.append('val')
	clist["WtLibVersion"].toC.append('val')
	#clist["WGestureEvent"].fromC.append('val')
	clist["WLogEntry"].fromC.append('val')
	clist["WLineF"].toC.append('val')
	clist["Coordinates"].fromC.append('val')
	clist["WLogger"]["Field"].fromC.append('val')
	clist["WLogger"]["Sep"].fromC.append('cval')
	clist["WLogger"]["TimeStamp"].fromC.append('cval')
	clist["WGoogleMap"]["Coordinate"].toC.append('val')
	clist["WGoogleMap"]["Coordinate"].fromC.append('val')
	clist["WApplication"]["UpdateLock"].fromC.append('val')
	clist["WFontMetrics"].fromC.append('val')
	clist["WFontMetrics"].toC.append('val')
	clist["WTextItem"].fromC.append('val')
	clist["WTextItem"].toC.append('val')
	clist["WValidator"]["Result"].toC.append('val')
	clist["WValidator"]["Result"].fromC.append('val')
	clist["WSslCertificate"].fromC.append('val')
	clist["WSslCertificate"]["DnAttribute"].fromC.append('val')

	# add preprocessor conditions for these classes
	#clist["WRasterImage"].condition = "#ifdef WT_HAS_WRASTERIMAGE"
	#clist["WPdfImage"].condition = "#ifdef WT_HAS_WPDFIMAGE"
	clist["WEnvironment"].undeletable = True

def patchHttp(clist):
	del clist["Request"].methods["getParameter"] #std::string*
	del clist["Request"].methods["getCookieValue"] #std::string*
	del clist["Request"].methods["getRanges"] #Fixme
	del clist["Request"].methods["getParameterMap"] #Fixme
	del clist["Request"].methods["getParameterValues"] #Fixme
	del clist["Request"].methods["cookies"] #Fixme
	del clist["ResponseContinuation"].methods["resource"] #WResource
	#del clist["ResponseContinuation"].methods["data"] #boost.any

	clist["ResponseContinuation"].undeletable = True
	clist["UploadedFile"].fromC.append('val')

#inherits from vector
	del clist["Request"].innerclasses["ByteRangeSpecifier"]
	del clist["Request_ByteRangeSpecifier"]
	#del clist["Request"].methods["getRanges"]



def patchChart(clist):
	clist["WDataSeries"].toC.append('val')
	clist["WDataSeries"].fromC.append('val')
	clist["WAxis"].toC.append('val')

	# TODO: private pure virtual methods
	# code generator is not aware of private methods. However,
	# if they are abstract, they must be reimplemented
	# in subclasses, otherwise code won't compile.
	# For the moment inheriting from this class is not allowed,
	# in order to avoid this problem
	clist["WAbstractChart"].constructor  = None
	del clist["WChart2DRenderer"].members["location_"] #AxisValue [3]


def patch(prefix, clist):
	print "Patching", prefix
	if prefix == "wt_":
		patchWt(clist)
	if prefix == "wthttp_":
		patchHttp(clist)
	if prefix == "wtchart_":
		patchChart(clist)

