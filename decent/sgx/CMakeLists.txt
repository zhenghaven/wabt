cmake_minimum_required(VERSION 3.15)

project(wabt-decent-sgx LANGUAGES C CXX VERSION 1.0.28)

option(DECENT_WABT_ENABLE_ENCLAVE "Enable decent targets" ON)
if(DEFINED DECENT_FRAMEWORK_ENABLE_ENCLAVE)
	set(DECENT_WABT_ENABLE_ENCLAVE ${DECENT_FRAMEWORK_ENABLE_ENCLAVE})
endif()

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
	${CMAKE_CURRENT_LIST_DIR}/include/config.h.in
	${CMAKE_CURRENT_LIST_DIR}/include/config.h
)

if(${DECENT_WABT_ENABLE_ENCLAVE})
	if ((NOT DEFINED INTEL_SGX_SDK_INCLUDE_DIR) OR
		(NOT DEFINED INTEL_SGX_SDK_LIBC_INCLUDE_DIR) OR
		(NOT DEFINED INTEL_SGX_SDK_CXX_INCLUDE_DIR))
		message(FATAL_ERROR "Required SGX CMake variables are not defined")
	endif()
endif()

set(WABT_SOURCES_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../..)

set(WASM_WAT_LIBRARY_SRC
	${CMAKE_CURRENT_LIST_DIR}/include/config.h

	${WABT_SOURCES_ROOT_DIR}/src/apply-names.h
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
	# ${WABT_SOURCES_ROOT_DIR}/src/binary-writer-spec.h
	# ${WABT_SOURCES_ROOT_DIR}/src/binary-writer-spec.cc
	${WABT_SOURCES_ROOT_DIR}/src/binding-hash.h
	${WABT_SOURCES_ROOT_DIR}/src/binding-hash.cc
	# ${WABT_SOURCES_ROOT_DIR}/src/color.h
	# ${WABT_SOURCES_ROOT_DIR}/src/color.cc
	${WABT_SOURCES_ROOT_DIR}/src/common.h
	${WABT_SOURCES_ROOT_DIR}/src/common.cc
	# ${WABT_SOURCES_ROOT_DIR}/src/config.h.in
	${WABT_SOURCES_ROOT_DIR}/src/config.cc
	# ${WABT_SOURCES_ROOT_DIR}/src/decompiler.h
	# ${WABT_SOURCES_ROOT_DIR}/src/decompiler-ast.h
	# ${WABT_SOURCES_ROOT_DIR}/src/decompiler-ls.h
	# ${WABT_SOURCES_ROOT_DIR}/src/decompiler-naming.h
	# ${WABT_SOURCES_ROOT_DIR}/src/decompiler.cc
	# ${WABT_SOURCES_ROOT_DIR}/src/error-formatter.h
	# ${WABT_SOURCES_ROOT_DIR}/src/error-formatter.cc
	${WABT_SOURCES_ROOT_DIR}/src/expr-visitor.h
	${WABT_SOURCES_ROOT_DIR}/src/expr-visitor.cc
	${WABT_SOURCES_ROOT_DIR}/src/feature.h
	${WABT_SOURCES_ROOT_DIR}/src/feature.cc
	# ${WABT_SOURCES_ROOT_DIR}/src/filenames.h
	# ${WABT_SOURCES_ROOT_DIR}/src/filenames.cc
	${WABT_SOURCES_ROOT_DIR}/src/generate-names.h
	${WABT_SOURCES_ROOT_DIR}/src/generate-names.cc
	${WABT_SOURCES_ROOT_DIR}/src/ir.h
	${WABT_SOURCES_ROOT_DIR}/src/ir.cc
	${WABT_SOURCES_ROOT_DIR}/src/ir-util.h
	${WABT_SOURCES_ROOT_DIR}/src/ir-util.cc
	${WABT_SOURCES_ROOT_DIR}/src/leb128.h
	${WABT_SOURCES_ROOT_DIR}/src/leb128.cc
	${WABT_SOURCES_ROOT_DIR}/src/lexer-source.h # Wat2Wasm
	${WABT_SOURCES_ROOT_DIR}/src/lexer-source.cc
	# ${WABT_SOURCES_ROOT_DIR}/src/lexer-source-line-finder.h
	# ${WABT_SOURCES_ROOT_DIR}/src/lexer-source-line-finder.cc
	${WABT_SOURCES_ROOT_DIR}/src/literal.h
	${WABT_SOURCES_ROOT_DIR}/src/literal.cc
	${WABT_SOURCES_ROOT_DIR}/src/opcode.h
	${WABT_SOURCES_ROOT_DIR}/src/opcode.cc
	${WABT_SOURCES_ROOT_DIR}/src/opcode-code-table.h
	${WABT_SOURCES_ROOT_DIR}/src/opcode-code-table.c
	# ${WABT_SOURCES_ROOT_DIR}/src/option-parser.h
	# ${WABT_SOURCES_ROOT_DIR}/src/option-parser.cc
	${WABT_SOURCES_ROOT_DIR}/src/resolve-names.h # Wat2Wasm
	${WABT_SOURCES_ROOT_DIR}/src/resolve-names.cc
	${WABT_SOURCES_ROOT_DIR}/src/shared-validator.h
	${WABT_SOURCES_ROOT_DIR}/src/shared-validator.cc
	${WABT_SOURCES_ROOT_DIR}/src/stream.h
	${WABT_SOURCES_ROOT_DIR}/src/stream.cc
	# ${WABT_SOURCES_ROOT_DIR}/src/string-util.h
	${WABT_SOURCES_ROOT_DIR}/src/token.h # Wat2Wasm
	${WABT_SOURCES_ROOT_DIR}/src/token.cc
	# ${WABT_SOURCES_ROOT_DIR}/src/tracing.h
	# ${WABT_SOURCES_ROOT_DIR}/src/tracing.cc
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

if(${DECENT_WABT_ENABLE_ENCLAVE})
	add_library(DecentWasmWat_core STATIC ${WASM_WAT_LIBRARY_SRC})

	target_include_directories(DecentWasmWat_core
		PRIVATE ${WABT_SOURCES_ROOT_DIR}
				${INTEL_SGX_SDK_INCLUDE_DIR}
				${INTEL_SGX_SDK_LIBC_INCLUDE_DIR}
				${INTEL_SGX_SDK_CXX_INCLUDE_DIR}
		PUBLIC  ${CMAKE_CURRENT_LIST_DIR}/include)

	target_compile_definitions(DecentWasmWat_core
		PRIVATE SECURE_ENCLAVE_ENV INTEL_SGX)

	target_compile_options(DecentWasmWat_core
		PRIVATE ${INTEL_SGX_SDK_TRUSTED_C_FLAGS}
				$<$<COMPILE_LANGUAGE:CXX>:${INTEL_SGX_SDK_TRUSTED_CXX_FLAGS}>)

	target_link_options(DecentWasmWat_core
		PRIVATE ${INTEL_SGX_SDK_TRUSTED_LINKER_FLAGS})

	set_property(TARGET DecentWasmWat_core PROPERTY
		MSVC_RUNTIME_LIBRARY
			"MultiThreaded$<$<OR:$<CONFIG:Debug>, $<CONFIG:DebugSimulation> >:Debug>")
	set_property(TARGET DecentWasmWat_core PROPERTY CXX_STANDARD 11)
endif()

##################################################
# target for untrusted side, for testing purpose
##################################################

add_library(DecentWasmWat_core_untrusted STATIC ${WASM_WAT_LIBRARY_SRC})

target_include_directories(DecentWasmWat_core_untrusted
	PRIVATE ${WABT_SOURCES_ROOT_DIR}
	PUBLIC  ${CMAKE_CURRENT_LIST_DIR}/include)

set_property(TARGET DecentWasmWat_core_untrusted PROPERTY
	MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET DecentWasmWat_core_untrusted PROPERTY CXX_STANDARD 17)

##################################################
# Add source directory
##################################################

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src)

set(WABT_SOURCES_ROOT_DIR ${WABT_SOURCES_ROOT_DIR} PARENT_SCOPE)
