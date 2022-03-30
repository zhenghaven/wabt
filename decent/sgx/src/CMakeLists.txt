# Copyright (c) 2022 Haofan Zheng
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT.

cmake_minimum_required(VERSION 3.14)

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

set_property(TARGET DecentWasmWat PROPERTY CXX_STANDARD 17)
