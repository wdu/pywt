#
# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.
#


path = "."

import sys
import Wt

import FileTreeTableNode

class FileTreeTable(Wt.WTreeTable):
	def __init__(self, path, parent = None):
		Wt.WTreeTable.__init__(self, parent)

		self.addColumn("Size", Wt.WLength(80));
		self.addColumn("Modified", Wt.WLength(110));

		self.header(1).setStyleClass("fsize");
		self.header(2).setStyleClass("date");

		self.root = FileTreeTableNode.FileTreeTableNode(path)
		self.setTreeRoot(self.root, "File");

		self.treeRoot().setImagePack("icons/");
		self.treeRoot().expand();


def createApplication(env):
	app = Wt.WApplication(env)

	app.setTitle("File explorer example")
	app.useStyleSheet(Wt.WLink("filetree.css"))

	treeTable = FileTreeTable(path)
	treeTable.resize(Wt.WLength(500), Wt.WLength(600))

	treeTable.tree().setSelectionMode(Wt.ExtendedSelection)
	treeTable.treeRoot().setNodeVisible(False)
	treeTable.treeRoot().setChildCountPolicy(Wt.WTreeNode.Enabled);

	app.root().addWidget(treeTable)

	return app

if __name__ == "__main__":
	import WtHttp
	WtHttp.WRun(sys.argv, createApplication)

