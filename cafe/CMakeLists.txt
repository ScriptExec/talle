project("cafe")
file(GLOB_RECURSE source_files CONFIGURE_DEPENDS 
	"${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/src/*.cpp"
	"${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/src/*.hpp"
	"${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/include/*.hpp"
)
add_executable(${PROJECT_NAME} ${source_files})

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS OFF
	)
endif()

if (MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE 
		/permissive-
	)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -fpermissive)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
target_link_libraries(${PROJECT_NAME} PRIVATE term)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/src")