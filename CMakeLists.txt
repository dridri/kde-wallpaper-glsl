project(glsl)
 
find_package(Qt4 REQUIRED)
find_package(KDE4 REQUIRED)
find_package(KDE4Workspace REQUIRED)
include(KDE4Defaults)
 
add_definitions(${KDE4_DEFINITIONS} -DPLATFORM_qt)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})
 
set(glsl_SRCS
	Wallpaper.cpp
	GLWidget.cpp
)
 
kde4_add_plugin(glsl ${glsl_SRCS})
target_link_libraries(glsl ${KDE4_PLASMA_LIBS} QtOpenGL ge_qt)
 
install(TARGETS glsl DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES glsl-wallpaper.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES glsl-wallpaper.vert DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES glsl-wallpaper.frag DESTINATION ${SERVICES_INSTALL_DIR})
