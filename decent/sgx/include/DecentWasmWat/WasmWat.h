// Copyright (c) 2022 Haofan Zheng
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#ifndef WABT_DECENT_WASMWAT_H_
#define WABT_DECENT_WASMWAT_H_

#include <cstdint>
#include <string>
#include <vector>

namespace wabt
{
  struct Module;
} // namespace wabt

namespace DecentWasmWat
{

  struct ModWrapper
  {
    ModWrapper(wabt::Module* ptr) :
      m_ptr(ptr)
    {}

    ~ModWrapper();

    wabt::Module* release()
    {
      wabt::Module* tmp = m_ptr;
      m_ptr = nullptr;
      return tmp;
    }

    wabt::Module* m_ptr;
  }; // struct ModWrap

  struct Wat2WasmConfig
  {
    Wat2WasmConfig() :
      validate(true),
      relocatable(false),
      canonicalize_lebs(true),
      write_debug_names(false)
    {}

    /**
     * @brief check for invalid modules
     *
     */
    bool validate;

    /**
     * @brief Create a relocatable wasm binary (suitable for linking with e.g. lld)
     *
     */
    bool relocatable;

    /**
     * @brief Write all LEB128 sizes as their minimal size instead of 5-bytes
     *
     */
    bool canonicalize_lebs;

    /**
     * @brief Write debug names to the generated binary file
     *
     */
    bool write_debug_names;
  };  // struct Wat2WasmConfig

  struct Wasm2WatConfig
  {
    Wasm2WatConfig() :
      validate(true),
      read_debug_names(true),
      stop_on_fist_error(true),
      fail_on_custom_section_error(true),
      generate_names(false),
      fold_exprs(false),
      inline_export(false),
      inline_import(false)
    {}

    /**
     * @brief check for invalid modules
     *
     */
    bool validate;

    /**
     * @brief read debug names in the binary file
     *
     */
    bool read_debug_names;

    /**
     * @brief stop on first error
     *
     */
    bool stop_on_fist_error;

    /**
     * @brief fail on errors in custom sections
     *
     */
    bool fail_on_custom_section_error;

    /**
     * @brief Give auto-generated names to non-named functions, types, etc.
     *
     */
    bool generate_names;

    /**
     * @brief Write folded expressions where possible
     *
     */
    bool fold_exprs;

    /**
     * @brief Write all exports inline
     *
     */
    bool inline_export;

    /**
     * @brief Write all imports inline
     *
     */
    bool inline_import;

  };  // struct Wasm2WatConfig

  std::string Wasm2Wat(
    const std::string& filename,
    const std::vector<uint8_t>& wasmSrc,
    const Wasm2WatConfig& config);

  std::vector<uint8_t> Wat2Wasm(
    const std::string& filename,
    const std::string& watSrc,
    const Wat2WasmConfig& config);

  ModWrapper Wasm2Mod(
    const std::string& filename,
    const std::vector<uint8_t>& wasmSrc,
    const Wasm2WatConfig& config);

  ModWrapper Wat2Mod(
    const std::string& filename,
    const std::string& watSrc,
    const Wat2WasmConfig& config);

  struct FuncInfo
  {
    std::string name;
  };

  std::vector<FuncInfo> GetFuncsInfo(
    const wabt::Module& mod);

}  // namespace DecentWasmWat

#endif /* WABT_DECENT_WASMWAT_H_ */
