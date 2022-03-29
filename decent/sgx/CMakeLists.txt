cmake_minimum_required(VERSION 3.18)

project(wabt-decent-sgx LANGUAGES C CXX VERSION 1.0.28)

if (MSVC)
	set(COMPILER_IS_CLANG 0)
	set(COMPILER_IS_GNU 0)
	set(COMPILER_IS_MSVC 1)
elseif (CMAKE_C_COMPILER_ID MATCHES "Clang")
	set(COMPILER_IS_CLANG 1)
	set(COMPILER_IS_GNU 0)
	set(COMPILER_IS_MSVC 0)
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
	set(COMPILER_IS_CLANG 0)
	set(COMPILER_IS_GNU 1)
	set(COMPILER_IS_MSVC 0)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
	set(COMPILER_IS_CLANG 1)
	set(COMPILER_IS_GNU 0)
	set(COMPILER_IS_MSVC 0)
else ()
	set(COMPILER_IS_CLANG 0)
	set(COMPILER_IS_GNU 0)
	set(COMPILER_IS_MSVC 0)
endif ()

include(CheckTypeSize)
check_type_size(ssize_t SSIZE_T)
check_type_size(size_t SIZEOF_SIZE_T)

configure_file(
	${CMAKE_CURRENT_LIST_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/config.h
)

set(WABT_SOURCES_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../..)

set(WASM_WAT_LIBRARY_SRC
	${CMAKE_CURRENT_BINARY_DIR}/config.h

	${WABT_SOURCES_ROOT_DIR}/src/apply-names.h  # executable
	${WABT_SOURCES_ROOT_DIR}/src/apply-names.cc
	${WABT_SOURCES_ROOT_DIR}/src/binary.h
	${WABT_SOURCES_ROOT_DIR}/src/binary.cc
	${WABT_SOURCES_ROOT_DIR}/src/binary-reader.h
	${WABT_SOURCES_ROOT_DIR}/src/binary-reader.cc
	${WABT_SOURCES_ROOT_DIR}/src/binary-reader-ir.h
	${WABT_SOURCES_ROOT_DIR}/src/binary-reader-ir.cc
	${WABT_SOURCES_ROOT_DIR}/src/binary-reader-logging.h
	${WABT_SOURCES_ROOT_DIR}/src/binary-reader-logging.cc
	${WABT_SOURCES_ROOT_DIR}/src/binary-writer.h
	${WABT_SOURCES_ROOT_DIR}/src/binary-writer.cc
	${WABT_SOURCES_ROOT_DIR}/src/binary-writer-spec.h
	${WABT_SOURCES_ROOT_DIR}/src/binary-writer-spec.cc
	${WABT_SOURCES_ROOT_DIR}/src/binding-hash.h
	${WABT_SOURCES_ROOT_DIR}/src/binding-hash.cc
	${WABT_SOURCES_ROOT_DIR}/src/color.h
	${WABT_SOURCES_ROOT_DIR}/src/color.cc
	${WABT_SOURCES_ROOT_DIR}/src/common.h  # executable
	${WABT_SOURCES_ROOT_DIR}/src/common.cc
	# src/config.h.in
	${WABT_SOURCES_ROOT_DIR}/src/config.cc
	# src/decompiler.h
	# src/decompiler-ast.h
	# src/decompiler-ls.h
	# src/decompiler-naming.h
	# src/decompiler.cc
	${WABT_SOURCES_ROOT_DIR}/src/error-formatter.h
	${WABT_SOURCES_ROOT_DIR}/src/error-formatter.cc
	${WABT_SOURCES_ROOT_DIR}/src/expr-visitor.h
	${WABT_SOURCES_ROOT_DIR}/src/expr-visitor.cc
	${WABT_SOURCES_ROOT_DIR}/src/feature.h  # executable
	${WABT_SOURCES_ROOT_DIR}/src/feature.cc
	${WABT_SOURCES_ROOT_DIR}/src/filenames.h  # executable
	${WABT_SOURCES_ROOT_DIR}/src/filenames.cc
	${WABT_SOURCES_ROOT_DIR}/src/generate-names.h  # executable
	${WABT_SOURCES_ROOT_DIR}/src/generate-names.cc
	${WABT_SOURCES_ROOT_DIR}/src/ir.h
	${WABT_SOURCES_ROOT_DIR}/src/ir.cc
	${WABT_SOURCES_ROOT_DIR}/src/ir-util.h
	${WABT_SOURCES_ROOT_DIR}/src/ir-util.cc
	${WABT_SOURCES_ROOT_DIR}/src/leb128.h
	${WABT_SOURCES_ROOT_DIR}/src/leb128.cc
	${WABT_SOURCES_ROOT_DIR}/src/lexer-source.h
	${WABT_SOURCES_ROOT_DIR}/src/lexer-source.cc
	${WABT_SOURCES_ROOT_DIR}/src/lexer-source-line-finder.h
	${WABT_SOURCES_ROOT_DIR}/src/lexer-source-line-finder.cc
	${WABT_SOURCES_ROOT_DIR}/src/literal.h
	${WABT_SOURCES_ROOT_DIR}/src/literal.cc
	${WABT_SOURCES_ROOT_DIR}/src/opcode.h
	${WABT_SOURCES_ROOT_DIR}/src/opcode.cc
	${WABT_SOURCES_ROOT_DIR}/src/opcode-code-table.h
	${WABT_SOURCES_ROOT_DIR}/src/opcode-code-table.c
	${WABT_SOURCES_ROOT_DIR}/src/option-parser.h
	${WABT_SOURCES_ROOT_DIR}/src/option-parser.cc
	${WABT_SOURCES_ROOT_DIR}/src/resolve-names.h
	${WABT_SOURCES_ROOT_DIR}/src/resolve-names.cc
	${WABT_SOURCES_ROOT_DIR}/src/shared-validator.h
	${WABT_SOURCES_ROOT_DIR}/src/shared-validator.cc
	${WABT_SOURCES_ROOT_DIR}/src/stream.h  # executable
	${WABT_SOURCES_ROOT_DIR}/src/stream.cc
	# src/string-util.h
	${WABT_SOURCES_ROOT_DIR}/src/token.h
	${WABT_SOURCES_ROOT_DIR}/src/token.cc
	# src/tracing.h
	# src/tracing.cc
	${WABT_SOURCES_ROOT_DIR}/src/type.h
	${WABT_SOURCES_ROOT_DIR}/src/type-checker.h
	${WABT_SOURCES_ROOT_DIR}/src/type-checker.cc
	${WABT_SOURCES_ROOT_DIR}/src/utf8.h
	${WABT_SOURCES_ROOT_DIR}/src/utf8.cc
	${WABT_SOURCES_ROOT_DIR}/src/validator.h
	${WABT_SOURCES_ROOT_DIR}/src/validator.cc
	${WABT_SOURCES_ROOT_DIR}/src/wast-lexer.h
	${WABT_SOURCES_ROOT_DIR}/src/wast-lexer.cc
	${WABT_SOURCES_ROOT_DIR}/src/wast-parser.h
	${WABT_SOURCES_ROOT_DIR}/src/wast-parser.cc
	${WABT_SOURCES_ROOT_DIR}/src/wat-writer.h
	${WABT_SOURCES_ROOT_DIR}/src/wat-writer.cc

  # TODO(binji): Move this into its own library?
#   src/interp/binary-reader-interp.h
#   src/interp/binary-reader-interp.cc
#   src/interp/interp.h
#   src/interp/interp.cc
#   src/interp/interp-inl.h
#   src/interp/interp-math.h
#   src/interp/interp-util.h
#   src/interp/interp-util.cc
#   src/interp/istream.h
#   src/interp/istream.cc
)

add_library(libwasmwat STATIC ${WASM_WAT_LIBRARY_SRC})
target_include_directories(libwasmwat
	PRIVATE ${WABT_SOURCES_ROOT_DIR} ${CMAKE_CURRENT_BINARY_DIR})
set_property(TARGET libwasmwat PROPERTY CXX_STANDARD 17)

#wat2wasm
add_executable(wat2wasm ${WABT_SOURCES_ROOT_DIR}/src/tools/wat2wasm.cc)
target_link_libraries(wat2wasm libwasmwat)
target_include_directories(wat2wasm
	PRIVATE ${WABT_SOURCES_ROOT_DIR} ${CMAKE_CURRENT_BINARY_DIR})
set_property(TARGET wat2wasm PROPERTY CXX_STANDARD 17)

#wasm2wat
add_executable(wasm2wat ${WABT_SOURCES_ROOT_DIR}/src/tools/wasm2wat.cc)
target_link_libraries(wasm2wat libwasmwat)
target_include_directories(wasm2wat
	PRIVATE ${WABT_SOURCES_ROOT_DIR} ${CMAKE_CURRENT_BINARY_DIR})
set_property(TARGET wasm2wat PROPERTY CXX_STANDARD 17)
