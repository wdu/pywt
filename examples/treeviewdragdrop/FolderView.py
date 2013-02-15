# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.
#

from Wt import *

class FolderView(WTreeView):
  # Constant that indicates the mime type for a selection of files.
  #
  # Every kind of dragged data should be identified using a unique mime type.
  FileSelectionMimeType = "application/x-computers-selection"

  # Constructor.
  def __init__(self, parent = None):
    WTreeView.__init__(self, parent)
    # Accept drops for the custom mime type.
    self.acceptDrops(self.FileSelectionMimeType)

  # Drop event.
  def dropEvent(self, event, target):
    # We reimplement the drop event to handle the dropping of a
    # selection of computers.
    #
    # The test below would always be true in this case, since we only
    # indicated support for that particular mime type.
    if event.mimeType() == self.FileSelectionMimeType:
      #
      # The source object for a drag of a selection from a WTreeView is
      # a WItemSelectionModel.
      selection = event.source()

      result = WMessageBox.popup("Drop event", "Move %d files to folder '%s' ?" % (len(selection.selectedIndexes()), str(target.data(DisplayRole))), Yes | No)

      if result == Yes:
        # You can access the source model from the selection and
        # manipulate it.
        sourceModel = selection.model()

        toChange = selection.selectedIndexes()

        for index in reversed(sorted(toChange)):
          # Copy target folder to file. Since we are using a
          # dynamic WSortFilterProxyModel that filters on folder, this
          # will also result in the removal of the file from the
          # current view.
          data = self.model().itemData(target)
          data[DecorationRole] = index.data(DecorationRole)
          sourceModel.setItemData(index, data)

