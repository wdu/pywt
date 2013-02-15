# Copyright (C) 2013 Emweb bvba, Belgium.
#
# See the LICENSE file for terms of use.
#

from Wt import *

def removeQuotes(line):
	for i in range(len(line)):
		if line[i][0] == '"': line[i] = line[i][1:]
		if line[i][-1] == '"': line[i] = line[i][:-1]

def readFromCsv(f, model, numRows = -1, firstLineIsHeaders = True):
	csvRow = 0
	lines = f.readlines()
	f.close()

	for line in lines:
		line = line.strip()
		tok = line.split(',')
		removeQuotes(tok)
		if len(tok) > model.columnCount():
			model.insertColumns(model.columnCount(), len(tok)-model.columnCount())

		col = 0
		for tok_it in tok:
			if firstLineIsHeaders and csvRow == 0:
				model.setHeaderData(col, WString.fromUTF8(tok_it))
			else:
				dataRow = csvRow
				if firstLineIsHeaders: dataRow = csvRow-1

				if dataRow >= model.rowCount(): 
	    				model.insertRows(model.rowCount(), dataRow + 1 - model.rowCount())

				try:
					value = int(tok_it)
				except:
					value = tok_it

				model.setData(dataRow, col, value)

			col = col +1



		csvRow = csvRow+1

