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
export module core:elf64;

import :builtins;

namespace core {

export constexpr u64 EI_NIDENT = 16;

export struct Elf64Hdr {
  unsigned char e_ident[EI_NIDENT];
  u64 e_type;
  u64 e_machine;
  u32 e_version;
  u64 e_entry;
  u64 e_phoff;
  u64 e_shoff;
  u32 e_flags;
  u64 e_ehsize;
  u64 e_phentsize;
  u64 e_phnum;
  u64 e_shentsize;
  u64 e_shnum;
  u64 e_shstrndx;
};

export struct Elf64Phdr {
  u32 p_type;
  u32 p_flags;
  u64 p_offset;
  u64 p_vaddr;
  u64 p_paddr;
  u64 p_filesz;
  u64 p_memsz;
  u64 p_align;
};

export enum Elf64Cst {
  PT_NULL = 0,
  PT_LOAD = 1,
  PT_DYNAMIC = 2,
  PT_INTERP = 3,
  PT_NOTE = 4,
  PT_SHLIB = 5,
  PT_PHDR = 6,
  PT_TLS = 7,
  PT_LOOS = 0x60000000,
  PT_HIOS = 0x6fffffff,
  PT_LOPROC = 0x70000000,
  PT_HIPROC = 0x7fffffff,
  PT_GNU_EH_FRAME = (PT_LOOS + 0x474e550),
  PT_GNU_STACK = (PT_LOOS + 0x474e551),
  PT_GNU_RELRO = (PT_LOOS + 0x474e552),
  PT_GNU_PROPERTY = (PT_LOOS + 0x474e553)
};

} // namespace core