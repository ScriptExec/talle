file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
	get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
		
	add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
	target_link_libraries(${EXAMPLE_NAME} PRIVATE talle)

	if (CMAKE_VERSION VERSION_GREATER 3.12)
		set_target_properties(${EXAMPLE_NAME} PROPERTIES
			CXX_STANDARD 20
			CXX_STANDARD_REQUIRED ON
			CXX_EXTENSIONS OFF
		)
	endif()

	if (MSVC)
		target_compile_options(${EXAMPLE_NAME} PRIVATE 
			/permissive-
		)
	else()
		target_compile_options(${EXAMPLE_NAME} PRIVATE -fpermissive)
	endif()
endforeach()