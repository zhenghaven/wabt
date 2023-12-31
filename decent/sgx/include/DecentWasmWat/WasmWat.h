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

    ModWrapper(const ModWrapper&) = delete;

    ModWrapper(ModWrapper&& other) :
      m_ptr(other.m_ptr)
    {
      other.m_ptr = nullptr;
    }

    ~ModWrapper();

    ModWrapper& operator=(const ModWrapper&) = delete;

    ModWrapper& operator=(ModWrapper&& other)
    {
      if (this != &other)
      {
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
      }
      return *this;
    }

    wabt::Module* release()
    {
      wabt::Module* tmp = m_ptr;
      m_ptr = nullptr;
      return tmp;
    }

    wabt::Module* m_ptr;
  }; // struct ModWrap

  struct ReadWatConfig
  {
    ReadWatConfig() :
      validate(true)
    {}

    /**
     * @brief check for invalid modules
     *
     */
    bool validate;
  }; // struct ReadWatConfig

  struct ReadWasmConfig
  {
    ReadWasmConfig() :
      validate(true),
      read_debug_names(true),
      stop_on_fist_error(true),
      fail_on_custom_section_error(true),
      generate_names(false),
      apply_names(false)
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
     * @brief Use function, import, function type, parameter and local names
     *        in Vars that reference them.
     *
     */
    bool apply_names;
  }; // struct ReadWasmConfig

  struct WriteWatConfig
  {
    WriteWatConfig() :
      fold_exprs(false),
      inline_export(false),
      inline_import(false)
    {}

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
  }; // struct WriteWatConfig

  struct WriteWasmConfig
  {
    WriteWasmConfig() :
      relocatable(false),
      canonicalize_lebs(true),
      write_debug_names(false)
    {}

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
  }; // struct WriteWasmConfig

  std::string Wasm2Wat(
    const std::string& filename,
    const std::vector<uint8_t>& wasmSrc,
    const ReadWasmConfig& rWasmCfg,
    const WriteWatConfig& wWatCfg);

  std::vector<uint8_t> Wat2Wasm(
    const std::string& filename,
    const std::string& watSrc,
  const ReadWatConfig& rWatCfg,
  const WriteWasmConfig& wWasmCfg);

  ModWrapper Wasm2Mod(
    const std::string& filename,
    const std::vector<uint8_t>& wasmSrc,
    const ReadWasmConfig& rWasmCfg);

  ModWrapper Wat2Mod(
    const std::string& filename,
    const std::string& watSrc,
    const ReadWatConfig& rWatCfg);

  std::string Mod2Wat(
    const wabt::Module& mod,
    const WriteWatConfig& wWatCfg);

  std::vector<uint8_t> Mod2Wasm(
    const wabt::Module& mod,
    const WriteWasmConfig& wWasmCfg);

  struct FuncInfo
  {
    std::string name;
  };

  std::vector<FuncInfo> GetFuncsInfo(
    const wabt::Module& mod);

}  // namespace DecentWasmWat

#endif /* WABT_DECENT_WASMWAT_H_ */
