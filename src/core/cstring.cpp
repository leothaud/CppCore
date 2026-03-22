/* Copyright 2026 Dylan Leothaud
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
export module core:cstring;

import :builtins;

namespace core {

export u64 strlen(const char *buffer) {
  u64 res = 0;
  while (buffer[res])
    ++res;
  return res;
}

export void memcpy(char *dst, const char *src, u64 size) asm("memcpy");

export void memcpy(char *dst, const char *src, u64 size) {
  for (u64 i = 0; i < size; ++i)
    dst[i] = src[i];
}

export bool streq(const char *a, const char *b, u64 size) {
  for (u64 i = 0; i < size; ++i)
    if (a[i] != b[i])
      return false;
  return true;
}

export bool streq(const char *a, const char *b) {
  while (*a && *b) {
    if (*a != *b)
      return false;
    ++a, ++b;
  }
  return *a == *b;
}

export void *memset(char *s, char c, u64 n) asm("memset");

export void *memset(char *s, char c, u64 n) {
  for (u64 i = 0; i < n; ++i)
    s[i] = c;
  return s;
}

} // namespace core