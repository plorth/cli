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
#pragma once

#include <stack>

#include <plorth/config.hpp>
#include <plorth/runtime.hpp>

namespace plorth::cli::utils
{
  std::u32string to_unistring(number::int_type);

#if PLORTH_ENABLE_FILE_SYSTEM_MODULES
  void scan_module_path(const std::shared_ptr<runtime>&);
#endif

  template<class StringT>
  static void count_open_braces(const StringT& input,
                                std::size_t length,
                                std::stack<char32_t>& open_braces)
  {
    for (std::size_t i = 0; i < length; ++i)
    {
      const auto c = input[i];

      switch (c)
      {
      case '#':
        return;

      case '(':
        open_braces.push(')');
        break;

      case '[':
        open_braces.push(']');
        break;

      case '{':
        open_braces.push('}');
        break;

      case ')':
      case ']':
      case '}':
        if (!open_braces.empty()
            && open_braces.top() == static_cast<char32_t>(c))
        {
          open_braces.pop();
        }
        break;

      case '\'':
      case '"':
        while (++i < length)
        {
          if (input[i] == c)
          {
            break;
          }
          else if (input[i] == '\\' && i + 1 < length)
          {
            ++i;
          }
        }
        break;
      }
    }
  }
}
