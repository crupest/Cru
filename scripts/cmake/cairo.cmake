find_program(MESON meson REQUIRED)
find_program(NINJA ninja REQUIRED)

set(CAIRO_BUILD_DIR ${CMAKE_BINARY_DIR}/cairo/build)
set(CAIRO_INSTALL_DIR ${CMAKE_BINARY_DIR}/cairo/install)

set(CAIRO_MESON_SETUP meson setup ${CAIRO_BUILD_DIR} --prefix=${CAIRO_INSTALL_DIR})

if(EMSCRIPTEN)
	cmake_path(GET CMAKE_C_COMPILER PARENT_PATH EMSCRIPTEN_TOOLCHAIN)
	set(EMSCRIPTEN_CROSS_FILE_TEMPLATE ${PROJECT_SOURCE_DIR}/scripts/meson-emscripten.ini.in)
	set(EMSCRIPTEN_CROSS_FILE ${CAIRO_BUILD_DIR}/emscripten.ini)
	configure_file(${EMSCRIPTEN_CROSS_FILE_TEMPLATE} ${EMSCRIPTEN_CROSS_FILE})
	set(CAIRO_MESON_SETUP ${CAIRO_MESON_SETUP} --cross-file ${EMSCRIPTEN_CROSS_FILE})
endif()

add_custom_target(
	cairo-build
	COMMAND mkdir -p ${CAIRO_BUILD_DIR} ${CAIRO_INSTALL_DIR}
	COMMAND ${CAIRO_MESON_SETUP}
	COMMAND ninja -C ${CAIRO_BUILD_DIR}
	COMMAND ninja -C ${CAIRO_BUILD_DIR} install
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/cairo
)

