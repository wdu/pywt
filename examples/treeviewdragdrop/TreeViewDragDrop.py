# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.
#

import sys
from Wt import *
import WtHttp
import CsvUtil
from FolderView import *

class FileModel(WStandardItemModel):
  def __init__(self, parent):
    WStandardItemModel.__init__(self, parent)

  def mimeType(self):
    return FolderView.FileSelectionMimeType

  dateDisplayFormat = WString("MM dd, yyyy")

  dateEditFormat = WString("dd-MM-yyyy")

# A dialog for editing a 'file'.
class FileEditDialog(WDialog):
  def __init__(self, model, item):
    WDialog.__init__(self, "Edit..."),
    self.model=model
    self.item=item
    modelRow = self.item.row()

    self.resize(300, WLength.Auto)

    # Create the form widgets, and load them with data from the model.

    # name
    self.nameEdit = WLineEdit(model.data(modelRow, 1))

    # type
    self.typeEdit = WComboBox()
    self.typeEdit.addItem("Document")
    self.typeEdit.addItem("Spreadsheet")
    self.typeEdit.addItem("Presentation")
    self.typeEdit.setCurrentIndex(self.typeEdit.findText(self.model.data(modelRow, 2)))

    # size
    self.sizeEdit = WLineEdit(str(self.model.data(modelRow, 3)))
    self.sizeEdit.setValidator(WIntValidator(0, sys.maxint, self))

    # created
    self.createdPicker = WDatePicker()
    self.createdPicker.lineEdit().validator().setMandatory(True)
    self.createdPicker.setFormat(FileModel.dateEditFormat)
    self.createdPicker.setDate(self.model.data(modelRow, 4))

    # modified
    self.modifiedPicker = WDatePicker()
    self.modifiedPicker.lineEdit().validator().setMandatory(True)
    self.modifiedPicker.setFormat(FileModel.dateEditFormat)
    self.modifiedPicker.setDate(self.model.data(modelRow, 5))

    # Use a grid layout for the labels and fields
    layout = WGridLayout()

    row = 0

    l = WLabel("Name:")
    layout.addWidget(l, row, 0)
    layout.addWidget(self.nameEdit, row, 1)
    l.setBuddy(self.nameEdit)
    row += 1

    l = WLabel("Type:")
    layout.addWidget(l, row, 0)
    layout.addWidget(self.typeEdit, row, 1, AlignTop)
    l.setBuddy(self.typeEdit)
    row += 1

    l = WLabel("Size:")
    layout.addWidget(l, row, 0)
    layout.addWidget(self.sizeEdit, row, 1)
    l.setBuddy(self.sizeEdit)
    row += 1

    l = WLabel("Created:")
    layout.addWidget(l, row, 0)
    layout.addWidget(self.createdPicker.lineEdit(), row, 1)
    layout.addWidget(self.createdPicker, row, 2)
    l.setBuddy(self.createdPicker.lineEdit())
    row += 1

    l = WLabel("Modified:")
    layout.addWidget(l, row, 0)
    layout.addWidget(self.modifiedPicker.lineEdit(), row, 1)
    layout.addWidget(self.modifiedPicker, row, 2)
    l.setBuddy(self.modifiedPicker.lineEdit())
    row += 1

    buttons = WContainerWidget()
    b = WPushButton("Save")
    buttons.addWidget(b)
    b.clicked().connect(self.accept)
    self.contents().enterPressed().connect(self.accept)
    b = WPushButton("Cancel")
    buttons.addWidget(b)
    b.clicked().connect(self.reject)

    # Focus the form widget that corresonds to the selected item.
    if item.column() == 2:
      self.typeEdit.setFocus()
    elif item.column() == 3:
      self.sizeEdit.setFocus()
    elif item.column() == 4:
      self.createdPicker.lineEdit().setFocus()
    elif item.column() == 5:
      self.modifiedPicker.lineEdit().setFocus()
    else:
      self.nameEdit.setFocus()

    layout.addWidget(buttons, row, 0, 0, 3, AlignCenter)
    layout.setColumnStretch(1, 1)

    self.contents().setLayout(layout, AlignTop | AlignJustify)

    self.finished().connect(self.handleFinish)

    self.show()

  def handleFinish(self, result):
    if result == WDialog.Accepted:
      # Update the model with data from the edit widgets.
      #
      # You will want to do some validation here...
      #
      # Note that we directly update the source model to avoid
      # problems caused by the dynamic sorting of the proxy model,
      # which reorders row numbers, and would cause us to switch to editing
      # the wrong data.
      modelRow = self.item.row()

      if isinstance(self.model, WAbstractProxyModel):
        m = self.model.sourceModel()
	modelRow = self.model.mapToSource(self.item).row()
      else:
        m = self.model

      m.setData(modelRow, 1, self.nameEdit.text())
      m.setData(modelRow, 2, self.typeEdit.currentText())
      m.setData(modelRow, 3, int(self.sizeEdit.text().toUTF8()))
      m.setData(modelRow, 4, self.createdPicker.date())
      m.setData(modelRow, 5, self.modifiedPicker.date())
    # Oops, can't translate this yet
    destroy(self)


class TreeViewDragDrop(WApplication):
  def __init__(self, env):
    WApplication.__init__(self, env)

    self.popup = 0
    self.popupActionBox = 0

    self.setCssTheme("polished")

    self.folderModel = WStandardItemModel(0, 1, self)
    self.populateFolders()

    self.fileModel = FileModel(self)
    self.populateFiles()

    self.fileFilterModel = WSortFilterProxyModel(self)
    self.fileFilterModel.setSourceModel(self.fileModel)
    self.fileFilterModel.setDynamicSortFilter(True)
    self.fileFilterModel.setFilterKeyColumn(0)
    self.fileFilterModel.setFilterRole(UserRole)

    # Setup the user interface.
    self.createUI()
    
  #Setup the user interface.
  def createUI(self):
    w = self.root()
    w.setStyleClass("maindiv")

    # The main layout is a 3x2 grid layout.
    layout = WGridLayout()
    layout.addWidget(self.createTitle("Folders"), 0, 0)
    layout.addWidget(self.createTitle("Files"), 0, 1)
    layout.addWidget(self.folderView(), 1, 0)
    layout.setColumnResizable(0)

    # select the first folder
    modelindex = self.folderModel.index(0, 0, self.folderModel.index(0, 0))
    self.folderView.select(modelindex)

    vbox = WVBoxLayout()
    vbox.addWidget(self.fileView(), 1)
    vbox.addWidget(self.pieChart(), 1)
    vbox.setResizable(0)

    layout.addLayout(vbox, 1, 1)

    layout.addWidget(self.aboutDisplay(), 2, 0, 1, 2, AlignTop)

    # Let row 1 and column 1 take the excess space.
    layout.setRowStretch(1, 1)
    layout.setColumnStretch(1, 1)

    w.setLayout(layout)

  # Creates a title widget.
  def createTitle(self, title):
    result = WText(title)
    result.setInline(False)
    result.setStyleClass("title")

    return result

  # Creates the folder WTreeView
  def folderView(self):
    treeView = FolderView()

    # To support right-click, we need to disable the built-in browser
    # context menu.
    #
    # Note that disabling the context menu and catching the
    # right-click does not work reliably on all browsers.
    treeView.setAttributeValue("oncontextmenu", "event.cancelBubble = true; event.returnValue = false; return false;")
    treeView.setModel(self.folderModel)
    treeView.resize(200, WLength.Auto)
    treeView.setSelectionMode(SingleSelection)
    treeView.expandToDepth(1)
    treeView.selectionChanged().connect(self.folderChanged)

    treeView.mouseWentUp().connect(self.showPopup)

    self.folderView = treeView

    return treeView

  # Creates the file table view (a WTableView)
  def fileView(self):
    tableView = WTableView()

    tableView.setAlternatingRowColors(True)

    tableView.setModel(self.fileFilterModel)
    tableView.setSelectionMode(ExtendedSelection)
    tableView.setDragEnabled(True)

    tableView.setColumnWidth(0, 100)
    tableView.setColumnWidth(1, 150)
    tableView.setColumnWidth(2, 100)
    tableView.setColumnWidth(3, 60)
    tableView.setColumnWidth(4, 100)
    tableView.setColumnWidth(5, 100)

    delegate = WItemDelegate(self)
    delegate.setTextFormat(FileModel.dateDisplayFormat)
    tableView.setItemDelegateForColumn(4, delegate)
    tableView.setItemDelegateForColumn(5, delegate)

    tableView.setColumnAlignment(3, AlignRight)
    tableView.setColumnAlignment(4, AlignRight)
    tableView.setColumnAlignment(5, AlignRight)

    tableView.sortByColumn(1, AscendingOrder)

    tableView.doubleClicked().connect(self.editFile)

    self.fileView = tableView

    return tableView

  # Edit a particular row.
  def editFile(self, item):
    self.foo = FileEditDialog(self.fileView.model(), item)

  # Creates the chart.
  def pieChart(self):
    chart = Chart.WPieChart()
    chart.setModel(self.fileFilterModel)
    chart.setTitle("File sizes")

    chart.setLabelsColumn(1); # Name
    chart.setDataColumn(3);   # Size

    chart.setPerspectiveEnabled(True, 0.2)
    chart.setDisplayLabels(Chart.Outside | Chart.TextLabel)

    if not WApplication.instance().environment().ajax():
      chart.resize(500, 200)
      chart.setMargin(WLength.Auto, Left | Right)
      w = WContainerWidget()
      w.addWidget(chart)
      w.setStyleClass("about")
      return w
    else:
      chart.setStyleClass("about")
      return chart

  # Creates the hints text.
  def aboutDisplay(self):
    result = WText(WString.tr("about-text"))
    result.setStyleClass("about")
    return result

  # Change the filter on the file view when the selected folder
  # changes.
  def folderChanged(self):
    for selected in self.folderView.selectedIndexes():
      folder = selected.data(UserRole)
      if folder:
        self.fileFilterModel.setFilterRegExp(folder)
      return

  # Show a popup for a folder item.
  def showPopup(self, item, event):
    if event.button() == WMouseEvent.RightButton:
      # Select the item, it was not yet selected.
      if not self.folderView.isSelected(item):
	self.folderView.select(item)

      self.popup = WPopupMenu()
      # note to dev: if this crashes, documentation is missing for an
      # overloaded WMenu::addItem() method
      self.popup.addItem("icons/folder_new.gif", "Create a New Folder")
      self.popup.addItem("Rename this Folder").setCheckable(True)
      self.popup.addItem("Delete this Folder")
      self.popup.addSeparator()
      self.popup.addItem("Folder Details")
      self.popup.addSeparator()
      self.popup.addItem("Application Inventory")
      self.popup.addItem("Hardware Inventory")
      self.popup.addSeparator()

      subMenu = WPopupMenu()
      subMenu.addItem("Sub Item 1")
      subMenu.addItem("Sub Item 2")
      self.popup.addMenu("File Deployments", subMenu)

      
      # This is one method of executing a popup, which does not block a
      # thread for a reentrant event loop, and thus scales.
      #
      # Alternatively you could call WPopupMenu.exec(), which returns
      # the result, but while waiting for it, blocks the thread.
      self.popup.aboutToHide().connect(self.popupAction)
      self.popup.popup(event)

  # Process the result of the popup menu
  def popupAction(self):
    if self.popup.result():
      # You could also bind extra data to an item using setData() and
      # check here for the action asked. For now, we just use the text.
      text = self.popup.result().text()

      self.popupActionBox = WMessageBox("Sorry.","Action '" + text + "' is not implemented.", NoIcon, Ok)
      self.popupActionBox.buttonClicked().connect(self.dialogDone)
      self.popupActionBox.show()
    destroy(self.popup)

  # Process the result of the message box.
  def dialogDone(self):
    destroy(self.popupActionBox)

  # Populate the files model.
  #
  # Data (and headers) is read from the CSV file data/files.csv. We
  # add icons to the first column, resolve the folder id to the
  # actual folder name, and configure item flags, and parse date
  # values.
  def populateFiles(self):
    self.fileModel.invisibleRootItem().setRowCount(0)

    f = open(self.appRoot() + "data/files.csv")
    #if (not f):
    #  raise Exception("Could not read: data/files.csv")
    CsvUtil.readFromCsv(f, self.fileModel)

    for i in range(self.fileModel.rowCount()):
      item = self.fileModel.item(i, 0)
      item.setFlags(item.flags() | ItemIsDragEnabled)
      item.setIcon("icons/file.gif")

      folderId = item.text().toUTF8()

      item.setData(folderId, UserRole)
      item.setText(self.folderNameMap[folderId])

      self.convertToDate(self.fileModel.item(i, 4))
      self.convertToDate(self.fileModel.item(i, 5))

  # Convert a string to a date.
  def convertToDate(self, item):
    d = WDate.fromString(item.text(), FileModel.dateEditFormat)
    item.setData(d, DisplayRole)

  # Populate the folders model.
  def populateFolders(self):

    level1 = self.createFolderItem("San Fransisco", "")
    self.folderModel.appendRow(level1)
    level1.appendRow(self.createFolderItem("Investors", "sf-investors"))
    level1.appendRow(self.createFolderItem("Fellows", "sf-fellows"))

    level1 = self.createFolderItem("Sophia Antipolis", "")
    self.folderModel.appendRow(level1)
    level1.appendRow(self.createFolderItem("R&D", "sa-r_d"))
    level1.appendRow(self.createFolderItem("Services", "sa-services"))
    level1.appendRow(self.createFolderItem("Support", "sa-support"))
    level1.appendRow(self.createFolderItem("Billing", "sa-billing"))

    level1 = self.createFolderItem("New York", "")
    self.folderModel.appendRow(level1)
    level1.appendRow(self.createFolderItem("Marketing", "ny-marketing"))
    level1.appendRow(self.createFolderItem("Sales", "ny-sales"))
    level1.appendRow(self.createFolderItem("Advisors", "ny-advisors"))

    level1 = self.createFolderItem("Frankfurt", "")
    self.folderModel.appendRow(level1)
    level1.appendRow(self.createFolderItem("Sales", "frank-sales"))

    self.folderModel.setHeaderData(0, Horizontal, "SandBox")

  folderNameMap = {}

  # Create a folder item.
  #
  # Configures flags for drag and drop support.
  def createFolderItem(self, location, folderId):
    result = WStandardItem(location)

    if folderId != "":
      result.setData(folderId)
      result.setFlags(result.flags() | ItemIsDropEnabled)
      self.folderNameMap[folderId] = location
    else:
      #result.setFlags(result.flags().clear(ItemIsSelectable))
      pass

    result.setIcon("icons/folder.gif")

    return result


def createApplication(env):
  app = TreeViewDragDrop(env)
  app.setTwoPhaseRenderingThreshold(0)
  app.setTitle("WTreeView Drag & Drop")
  app.useStyleSheet(WLink("styles.css"))
  app.messageResourceBundle().use(WApplication.appRoot() + "about")
  app.refresh()
  return app


if __name__ == "__main__":
  import WtHttp
  WtHttp.WRun(sys.argv, createApplication)

