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
 .global _start
.type _start, @function
_start:
  pop %rdi
  movq %rsp, %rsi
  movq %rsi, %rdx
initEnvp:
  cmpq $0x0,(%rdx)
  lea 0x8(%rdx),%rdx
  jne initEnvp
  movq %rdx, %rcx
initAuxv:
  cmpq $0x0,(%rcx)
  lea 0x8(%rcx),%rcx
  jne initAuxv
  andq $-16, %rsp
  call init
  movq %rax, %rdi
  movq $60, %rax
  syscall