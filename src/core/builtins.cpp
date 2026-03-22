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
export module core:builtins;

#if defined(__UINT64_TYPE__)
export using u64 = __UINT64_TYPE__;
#else
#error No u64 alternative
#endif

#if defined(__UINT32_TYPE__)
export using u32 = __UINT32_TYPE__;
#else
#error No u32 alternative
#endif

#if defined(__UINT16_TYPE__)
export using u16 = __UINT16_TYPE__;
#else
#error No u16 alternative
#endif

#if defined(__UINT8_TYPE__)
export using u8 = __UINT8_TYPE__;
#else
#error No u8 alternative
#endif

#if defined(__INT64_TYPE__)
export using i64 = __INT64_TYPE__;
#else
#error No i64 alternative
#endif

#if defined(__INT32_TYPE__)
export using i32 = __INT32_TYPE__;
#else
#error No i32 alternative
#endif

#if defined(__INT16_TYPE__)
export using i16 = __INT16_TYPE__;
#else
#error No i16 alternative
#endif

#if defined(__INT8_TYPE__)
export using i8 = __INT8_TYPE__;
#else
#error No i8 alternative
#endif

namespace core {

export [[clang::always_inline, noreturn]] void unreachable() {
#if __has_builtin(__builtin_unreachable)
  __builtin_unreachable();
#else
#error No unreachable alternative
#endif
}

export [[clang::always_inline, noreturn]] void trap() {
#if __has_builtin(__builtin_trap)
  __builtin_trap();
#else
#error No trap alternative
#endif
}

} // namespace core