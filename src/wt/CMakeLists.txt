# the library containing wt
ADD_LIBRARY(wtmodule SHARED
		../python/Enum.cpp
		../python/SpecialConversion.cpp

		http/HttpShells.cpp
		http/HttpConversions.cpp
		http/HttpWrappers1.cpp
		http/PyHttp.cpp
		../generated/wt/http/gwthttp_Strings.cpp

		chart/ChartShells.cpp
		chart/ChartConversions.cpp
		chart/ChartWrappers1.cpp
		chart/PyChart.cpp
		../generated/wt/chart/gwtchart_Strings.cpp

		WtShells.cpp
		Threading.cpp
		WtSpecialConversion.cpp
		WtConversions.cpp
		../generated/wt/gwt_Strings.cpp
		../generated/wt/gwt_Compare.cpp
		WtWrappers1.cpp
		WtWrappers2.cpp
		WtWrappers3.cpp
		WtWrappers4.cpp
		WtWrappers5.cpp
#		WtWrappers6.cpp
#		WtWrappers7.cpp
#		WtWrappers8.cpp
#		WtWrappers9.cpp
#		WtWrappers10.cpp
		PyWt.cpp

		wtmodule.cpp


		../python/PyCPPClassType.cpp
		../python/Compare.cpp
		../python/PyException.cpp
		SlotConnector.cpp
		PythonApplication.cpp
#		WtString.cpp
#		WtUnicode.cpp
		)


TARGET_LINK_LIBRARIES(wtmodule ${Wt_LIBRARY} ${PYTHON_LIBRARY} boost_signals-mt)

SET_TARGET_PROPERTIES(wtmodule PROPERTIES
		OUTPUT_NAME Wt 
		PREFIX  "")
		
IF(MSVC)
SET_TARGET_PROPERTIES(wtmodule
  PROPERTIES
    COMPILE_FLAGS
    "/wd4251 /wd4275 /wd4099 /MP"
  )
ENDIF(MSVC)
		
INCLUDE_DIRECTORIES(
	.
	../python
	./http
	./chart
	../generated/wt
	../generated/wt/http
	../generated/wt/chart
	${WT_SOURCE_DIR}/src
	${PYTHON_INC_DIR}
)


INSTALL(TARGETS wtmodule DESTINATION ${PYTHON_INSTALL_DIR})

