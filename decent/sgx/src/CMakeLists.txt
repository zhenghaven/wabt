# Copyright (c) 2022 Haofan Zheng
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT.

cmake_minimum_required(VERSION 3.15)

if(${DECENTWABT_ENABLE_ENCLAVE})
	add_library(
		DecentWasmWat_trusted
		STATIC
			${CMAKE_CURRENT_LIST_DIR}/WasmWat.cc
	)

	target_include_directories(
		DecentWasmWat_trusted
		PRIVATE
			${WABT_SOURCES_ROOT_DIR}
			${DECENTENCLAVE_SGXSDK_INCLUDE}
			${DECENTENCLAVE_SGXSDK_INCLUDE_C}
			${DECENTENCLAVE_SGXSDK_INCLUDE_CXX}
			${CMAKE_CURRENT_LIST_DIR}/../include
	)

	target_compile_definitions(
		DecentWasmWat_trusted
		PRIVATE
			DECENT_ENCLAVE_TRUSTED
			DECENT_ENCLAVE_PLATFORM_SGX_TRUSTED
	)

	target_compile_options(
		DecentWasmWat_trusted
		PRIVATE
			${DECENTENCLAVE_SGXSDK_TRUSTED_C_FLAGS}
			$<$<COMPILE_LANGUAGE:CXX>:${DECENTENCLAVE_SGXSDK_TRUSTED_CXX_FLAGS}>
	)

	target_link_libraries(
		DecentWasmWat_trusted
		PUBLIC
			DecentWasmWat_core_trusted
	)

	target_link_options(
		DecentWasmWat_trusted
		PRIVATE
			${DECENTENCLAVE_SGXSDK_TRUSTED_LINKER_FLAGS}
	)

	set_property(
		TARGET DecentWasmWat_trusted
		PROPERTY
			MSVC_RUNTIME_LIBRARY
			"MultiThreaded$<$<OR:$<CONFIG:Debug>, $<CONFIG:DebugSimulation> >:Debug>"
	)
	set_property(
		TARGET DecentWasmWat_trusted
		PROPERTY
			CXX_STANDARD 17
	)
endif()

##################################################
# target for untrusted side, for testing purpose
##################################################

add_library(
	DecentWasmWat_untrusted
	STATIC
		${CMAKE_CURRENT_LIST_DIR}/WasmWat.cc
)

target_include_directories(
	DecentWasmWat_untrusted
	PRIVATE
		${WABT_SOURCES_ROOT_DIR}
		${CMAKE_CURRENT_LIST_DIR}/../include
)

target_link_libraries(
	DecentWasmWat_untrusted
	PUBLIC
		DecentWasmWat_core_untrusted
)

set_property(
	TARGET DecentWasmWat_untrusted
	PROPERTY
		MSVC_RUNTIME_LIBRARY
		"MultiThreaded$<$<OR:$<CONFIG:Debug>, $<CONFIG:DebugSimulation> >:Debug>"
)
set_property(
	TARGET DecentWasmWat_untrusted
	PROPERTY
		CXX_STANDARD 17
)

