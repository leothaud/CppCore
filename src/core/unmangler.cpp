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
export module core:unmangler;

import :string;
import :traits;

namespace core {

// https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling
export String unmangle(String name) {
  if (!name.startsWith("_Z"))
    return name;
  // u64 index = 2, size = name.length() - 1;
  // const char *base = name.ptr();

  return "";
}

} // namespace core