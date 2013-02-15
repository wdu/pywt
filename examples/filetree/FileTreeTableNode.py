#
# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.
#



import sys
import Wt
import os.path
import time

def createIcon(path):
	if os.path.exists(path) and os.path.isdir(path):
		return Wt.WIconPair("icons/yellow-folder-closed.png",
			 "icons/yellow-folder-open.png", False, None)
	else:
		return Wt.WIconPair("icons/document.png",
			 "icons/yellow-folder-open.png", False, None)


class FileTreeTableNode(Wt.WTreeTableNode):
	def __init__(self, path):
		Wt.WTreeTableNode.__init__(self, os.path.basename(path), createIcon(path), None)
		self.path = path

		self.label().setTextFormat(Wt.PlainText);

		if os.path.exists(path):
			if not os.path.isdir(path):
				fsize = os.stat(path).st_size
				self.setColumnWidget(1, Wt.WText(str(fsize), None))
				self.columnWidget(1).setInline(False)
				self.columnWidget(1).setStyleClass("fsize")
			else:
				self.setSelectable(False)

			t = os.stat(path).st_mtime
			ttm = time.localtime(t)
			c = time.strftime("%b %d %Y", ttm)

			self.setColumnWidget(2, Wt.WText(c,  Wt.XHTMLText))
			self.columnWidget(2).setStyleClass("date")


	def expandable(self):
		if not self.populated():
			return os.path.isdir(self.path)
		else:
			return Wt.WTreeTableNode.expandable(self)

	def populate(self):
		if os.path.isdir(self.path):
			paths = os.listdir(self.path)
			for p in paths:
				self.addChildNode(FileTreeTableNode(os.path.join(self.path, p)))


