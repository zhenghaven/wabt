/*
 * Copyright 2016 WebAssembly Community Group participants
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Copyright (c) 2022 Haofan Zheng
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

// Most part of implementation is derived from:
// 1) src/tools/wasm2wat.cc
// 2) src/tools/wat2wasm.cc

#include <DecentWasmWat/WasmWat.h>

#include <stdexcept>

#include <src/apply-names.h>
#include <src/binary-reader-ir.h>
#include <src/binary-reader.h>
#include <src/feature.h>
#include <src/generate-names.h>
#include <src/ir.h>
#include <src/stream.h>
#include <src/validator.h>
#include <src/wat-writer.h>

#include <src/binary-writer.h>
#include <src/wast-lexer.h>
#include <src/wast-parser.h>

static std::string GenerateErrorMsg(const wabt::Errors& errors)
{
  std::string errMsg;
  for(const auto& err : errors)
  {
    if (errMsg.size() > 0)
    {
      errMsg += '\n';
    }
    errMsg += err.message;
  }

  return "WASM <=> WAT conversion failed: " + errMsg;
}

std::vector<uint8_t> DecentWasmWat::Wat2Wasm(
  const std::string& filename,
  const std::string& watSrc,
  const Wat2WasmConfig& config)
{
  wabt::Result result;
  wabt::Features features;

  wabt::WriteBinaryOptions write_binary_options;
  write_binary_options.relocatable       = config.relocatable;
  write_binary_options.canonicalize_lebs = config.canonicalize_lebs;
  write_binary_options.write_debug_names = config.write_debug_names;

  static_assert(sizeof(typename std::string::value_type) == sizeof(uint8_t),
    "The value size of std::string doesn't match size of uint8_t");
  std::unique_ptr<wabt::WastLexer> lexer =
    wabt::WastLexer::CreateBufferLexer(
      filename.c_str(),
      reinterpret_cast<const uint8_t*>(watSrc.data()),
      watSrc.size());

  wabt::Errors errors;
  std::unique_ptr<wabt::Module> module;
  wabt::WastParseOptions parse_wast_options(features);
  result = wabt::ParseWatModule(
    lexer.get(), &module, &errors, &parse_wast_options);

  if (wabt::Succeeded(result) && config.validate)
  {
    wabt::ValidateOptions options(features);
    result = ValidateModule(module.get(), &errors, options);
  }

  if (wabt::Succeeded(result))
  {
    wabt::MemoryStream stream;
    write_binary_options.features = features;
    result = wabt::WriteBinaryModule(
      &stream, module.get(), write_binary_options);

    if (wabt::Succeeded(result))
    {
      return std::vector<uint8_t>(
        stream.output_buffer().data.data(),
        stream.output_buffer().data.data() +
          stream.output_buffer().data.size());
    }
  }

  throw std::runtime_error(GenerateErrorMsg(errors));
}

std::string DecentWasmWat::Wasm2Wat(
  const std::string& filename,
  const std::vector<uint8_t>& wasmSrc,
  const Wasm2WatConfig& config)
{
  wabt::Result result;
  wabt::Errors errors;
  wabt::Module module;
  wabt::Features features;

  wabt::WriteWatOptions s_write_wat_options;
  s_write_wat_options.fold_exprs    = config.fold_exprs;
  s_write_wat_options.inline_export = config.inline_export;
  s_write_wat_options.inline_import = config.inline_import;

  wabt::ReadBinaryOptions options(features, nullptr,
              config.read_debug_names,
              config.stop_on_fist_error,
              config.fail_on_custom_section_error);
  result = wabt::ReadBinaryIr(
    filename.c_str(), wasmSrc.data(), wasmSrc.size(),
    options, &errors, &module);

  if (wabt::Succeeded(result))
  {
    if (wabt::Succeeded(result) && config.validate)
    {
      wabt::ValidateOptions options(features);
      result = wabt::ValidateModule(&module, &errors, options);
    }

    if (config.generate_names)
    {
      result = wabt::GenerateNames(&module);
    }

    if (wabt::Succeeded(result))
    {
      /* TODO(binji): This shouldn't fail; if a name can't be applied
      * (because the index is invalid, say) it should just be skipped. */
      wabt::Result dummy_result = wabt::ApplyNames(&module);
      static_cast<void>(dummy_result);
    }

    if (wabt::Succeeded(result))
    {
      wabt::MemoryStream stream;
      result = wabt::WriteWat(&stream, &module, s_write_wat_options);

      static_assert(
        sizeof(typename std::string::value_type) == sizeof(uint8_t),
        "The value size of std::string doesn't match size of uint8_t");
      const char* strBegin =
        reinterpret_cast<const char*>(
          stream.output_buffer().data.data());
      return std::string(
        strBegin, strBegin + stream.output_buffer().data.size());
    }
  }

  throw std::runtime_error(GenerateErrorMsg(errors));
}
