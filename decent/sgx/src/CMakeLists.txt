# Copyright (c) 2022 Haofan Zheng
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT.

cmake_minimum_required(VERSION 3.15)

if(${DECENT_WABT_ENABLE_ENCLAVE})
	add_library(DecentWasmWat STATIC ${CMAKE_CURRENT_LIST_DIR}/WasmWat.cc)

	target_include_directories(DecentWasmWat
		PRIVATE ${WABT_SOURCES_ROOT_DIR}
				${INTEL_SGX_SDK_INCLUDE_DIR}
				${INTEL_SGX_SDK_LIBC_INCLUDE_DIR}
				${INTEL_SGX_SDK_CXX_INCLUDE_DIR}
				${CMAKE_CURRENT_LIST_DIR}/../include)

	#target_link_libraries(DecentWasmWat DecentWasmWat_core)

	target_compile_definitions(DecentWasmWat
		PRIVATE SECURE_ENCLAVE_ENV INTEL_SGX)

	target_compile_options(DecentWasmWat
		PRIVATE ${INTEL_SGX_SDK_TRUSTED_C_FLAGS}
				$<$<COMPILE_LANGUAGE:CXX>:${INTEL_SGX_SDK_TRUSTED_CXX_FLAGS}>)

	target_link_options(DecentWasmWat
		PRIVATE ${INTEL_SGX_SDK_TRUSTED_LINKER_FLAGS})

	set_property(TARGET DecentWasmWat PROPERTY
		MSVC_RUNTIME_LIBRARY
			"MultiThreaded$<$<OR:$<CONFIG:Debug>, $<CONFIG:DebugSimulation> >:Debug>")
	set_property(TARGET DecentWasmWat PROPERTY CXX_STANDARD 11)
endif()

##################################################
# target for untrusted side, for testing purpose
##################################################

add_library(DecentWasmWat_untrusted STATIC ${CMAKE_CURRENT_LIST_DIR}/WasmWat.cc)

target_include_directories(DecentWasmWat_untrusted
	PRIVATE ${WABT_SOURCES_ROOT_DIR}
			${CMAKE_CURRENT_LIST_DIR}/../include)

target_link_libraries(DecentWasmWat_untrusted DecentWasmWat_core_untrusted)

set_property(TARGET DecentWasmWat_untrusted PROPERTY
	MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET DecentWasmWat_untrusted PROPERTY CXX_STANDARD 17)
