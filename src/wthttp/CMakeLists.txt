
ADD_LIBRARY(wthttpmodule SHARED
		wthttpmodule.cpp
		)

TARGET_LINK_LIBRARIES(wthttpmodule ${PYTHON_LIBRARY} ${Wt_HTTP_LIBRARY})

SET_TARGET_PROPERTIES(wthttpmodule PROPERTIES
		OUTPUT_NAME WtHttp
		PREFIX "" )


	INCLUDE_DIRECTORIES(
			.
			../wt
			../python
			${WT_SOURCE_DIR}/src
			${PYTHON_INC_DIR}
			)

INSTALL(TARGETS wthttpmodule DESTINATION ${PYTHON_INSTALL_DIR})


