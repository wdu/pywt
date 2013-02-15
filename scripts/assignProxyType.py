# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.


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
				aclass.isWStandardItem = aclass.isWStandardItem or checkWStandardItem(base)
			return aclass.isWStandardItem


def assign( c ):
	checkWObject(c)
	checkWStandardItem(c)
	deletespec = ""
	if c.undeletable: deletespec = ", false"
	if c.name == "WApplication":
		c.proxy = "ProxyWApplication"
	elif c.isWStandardItem:
		c.proxy = "ProxyWStandardItem<%s%s>" % (c.fullCoreName(), deletespec)
	elif c.isWObject:
		c.proxy = "ProxyWObject<%s%s>" % (c.fullCoreName(), deletespec)
	else:
		c.proxy = "ProxyBase<%s%s>" % (c.fullCoreName(), deletespec)


