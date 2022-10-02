/*
 * Copyright (c) 2017-2022, Rauli Laine
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <plorth/config.hpp>
#include <plorth/cli/utils.hpp>

#include <peelo/unicode/encoding/utf8.hpp>

namespace plorth::cli::utils
{
#if PLORTH_ENABLE_32BIT_INT
  using uint_type = std::uint32_t;
#else
  using uint_type = std::uint64_t;
#endif

  static const char digitmap[] = "0123456789abcdefghijklmnopqrstuvwxyz";

  std::u32string to_unistring(number::int_type number)
  {
    const bool negative = number < 0;
    uint_type mag = static_cast<uint_type>(negative ? -number : number);
    std::u32string result;

    if (mag != 0)
    {
      result.reserve(negative ? 21 : 20);
      do
      {
        result.insert(result.begin(), digitmap[mag % 10]);
        mag /= 10;
      }
      while (mag);
    } else {
      result.insert(result.begin(), '0');
    }
    if (negative)
    {
      result.insert(result.begin(), '-');
    }

    return result;
  }


#if PLORTH_ENABLE_FILE_SYSTEM_MODULES
  void scan_module_path(const std::shared_ptr<runtime>& rt)
  {
#if defined(_WIN32)
    static const char path_separator = ';';
#else
    static const char path_separator = ':';
#endif
    std::vector<std::u32string> module_paths;
    auto begin = std::getenv("PLORTHPATH");
    auto end = begin;

    if (end)
    {
      for (; *end; ++end)
      {
        if (*end != path_separator)
        {
          continue;
        }

        if (end - begin > 0)
        {
          module_paths.push_back(peelo::unicode::encoding::utf8::decode(
            std::string(begin, end - begin)
          ));
        }
        begin = end + 1;
      }

      if (end - begin > 0)
      {
        module_paths.push_back(peelo::unicode::encoding::utf8::decode(
          std::string(begin, end - begin)
        ));
      }
    }

#if defined(PLORTH_RUNTIME_LIBRARY_PATH)
    if (module_paths.empty())
    {
      module_paths.push_back(PLORTH_RUNTIME_LIBRARY_PATH);
    }
#endif

    rt->module_manager() = module::manager::file_system(
      rt->memory_manager(),
      module_paths
    );
  }
#endif
}
