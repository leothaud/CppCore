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
import core;

int main() {
  int *x = new int[128];
  for (int i = 0; i < 128; ++i) {
    x[i] = 12;
  }
  return 0;
}
/*
// Check that all symbols of the executable can be demangled.
u64 fail = 0;
for (auto &[_, name] : core::symbolNames) {
  auto demangled = core::demangle(name);
  if (demangled.failed()) {
    core::serr << demangled.getMessage() << "\n";
    ++fail;
  }
}
if (fail != 0) {
  core::serr << "Demangling failed in " << fail << " functions.\n";
  return core::TestResult::fail;
}

core::Map<core::String, core::String> symbols = {
    {"_Z3foo", "foo"},
    {"_Z3foov", "foo()"},
    {"_Z3fooi", "foo(int)"},
    {"_ZN4coreW4core11symbolNamesE", "core::symbolNames@core"},
    {"__cxa_finalize", "__cxa_finalize"},
    {"__cxa_guard_acquire", "__cxa_guard_acquire"},
    {"__cxa_guard_release", "__cxa_guard_release"},
    {"__dso_handle", "__dso_handle"},
    {"__dso_handle", "__dso_handle"},
    {"_TLS_MODULE_BASE_", "_TLS_MODULE_BASE_"},
    {"_ZN4coreW4core11symbolNamesE", "core::symbolNames@core"},
    {"_ZN4coreW4core11symbolNamesE", "core::symbolNames@core"},
    {"__executable_start", "__executable_start"},
    {"end", "end"},
    {"end", "end"},
    {"etext", "etext"},
    {"etext", "etext"},
    {"__executable_start", "__executable_start"},
    {"_TLS_MODULE_BASE_", "_TLS_MODULE_BASE_"},
    {"_TLS_MODULE_BASE_", "_TLS_MODULE_BASE_"},
    {"_GLOBAL_OFFSET_TABLE_", "_GLOBAL_OFFSET_TABLE_"},
    {"__GNU_EH_FRAME_HDR", "__GNU_EH_FRAME_HDR"},
    {"init", "init"},
    {"_TLS_MODULE_BASE_", "_TLS_MODULE_BASE_"},
    {"_TLS_MODULE_BASE_", "_TLS_MODULE_BASE_"},
    {"initAuxv", "initAuxv"},
    {"initEnvp", "initEnvp"},
    {".LCPI2_0", ".LCPI2_0"},
    {".LCPI2_1", ".LCPI2_1"},
    {".LCPI2_2", ".LCPI2_2"},
    {".LCPI2_3", ".LCPI2_3"},
    {"memcpy", "memcpy"},
    {"memset", "memset"},
    {"_TLS_MODULE_BASE_", "_TLS_MODULE_BASE_"},
    {"_TLS_MODULE_BASE_", "_TLS_MODULE_BASE_"},
    {"printSubstitution", "printSubstitution"},
    {"__stack_chk_fail", "__stack_chk_fail"},
    {"__stack_chk_guard", "__stack_chk_guard"},
    {"_start", "_start"},
    {"_TLS_MODULE_BASE_", "_TLS_MODULE_BASE_"},
    {"_TLS_MODULE_BASE_", "_TLS_MODULE_BASE_"},
    {"_TLS_MODULE_BASE_", "_TLS_MODULE_BASE_"},
    {"_TLS_MODULE_BASE_", "_TLS_MODULE_BASE_"},
    {"_TLS_MODULE_BASE_", "_TLS_MODULE_BASE_"},
    {"_Z1fRKic", "f(int const&, char)"},
    {"_Z4maini", "main(int)"},
    {"_Z4mainiPPc", "main(int, char**)"},
    {"_Z4mainiPPcS0_", "main(int, char**, char**)"},
    {"_Z4mainiPPcS0_PN4coreW4core9Elf32AuxvE",
     "main(int, char**, char**, core::Elf32Auxv@core*)"},
    {"_Z4mainv", "main()"},
    {"_Z4testPFiRKicE", "test(int (*)(int const&, char))"},
    {"_ZdaPv", "operator delete[](void*)"},
    {"_ZdaPvm", "operator delete[](void*, unsigned long)"},
    {"_ZdlPv", "operator delete(void*)"},
    {"_ZdlPvm", "operator delete(void*, unsigned long)"},
    {"_ZGVZN4coreW4core9AllocatorILm32076800ELm1024EE3getEvE9allocator",
     "guard variable for core::Allocator@core<32076800ul, "
     "1024ul>::get()::allocator"},
    {"_ZN4coreW4core10BaseSymbolC2ENS_S0_6StringE",
     "core::BaseSymbol@core::BaseSymbol(core::String@core)"},
    {"_ZN4coreW4core10BaseSymbolD0Ev",
     "core::BaseSymbol@core::~BaseSymbol()"},
    {"_ZN4coreW4core10BaseSymbolD2Ev",
     "core::BaseSymbol@core::~BaseSymbol()"},
    {"_ZN4coreW4core10fillBufferEPcm",
     "core::fillBuffer@core(char*, unsigned long)"},
    {"_ZN4coreW4core10fillBufferEPcmRm",
     "core::fillBuffer@core(char*, unsigned long, unsigned long&)"},
    {"_ZN4coreW4core10LvalSymbolC2ENS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEE",
     "core::LvalSymbol@core::LvalSymbol(core::UniquePtr@core<core::"
     "DemangledSymbol@core>)"},
    {"_ZN4coreW4core10LvalSymbolD0Ev",
     "core::LvalSymbol@core::~LvalSymbol()"},
    {"_ZN4coreW4core10LvalSymbolD2Ev",
     "core::LvalSymbol@core::~LvalSymbol()"},
    {"_ZN4coreW4core10RvalSymbolC2ENS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEE",
     "core::RvalSymbol@core::RvalSymbol(core::UniquePtr@core<core::"
     "DemangledSymbol@core>)"},
    {"_ZN4coreW4core10RvalSymbolD0Ev",
     "core::RvalSymbol@core::~RvalSymbol()"},
    {"_ZN4coreW4core10RvalSymbolD2Ev",
     "core::RvalSymbol@core::~RvalSymbol()"},
    {"_ZN4coreW4core11allocateTlsEv", "core::allocateTls@core()"},
    {"_ZN4coreW4core11ChainSymbol7addPartENS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEE",
     "core::ChainSymbol@core::addPart(core::UniquePtr@core<core::"
     "DemangledSymbol@core>)"},
    {"_ZN4coreW4core11ChainSymbolC2Ev",
     "core::ChainSymbol@core::ChainSymbol()"},
    {"_ZN4coreW4core11ChainSymbolD0Ev",
     "core::ChainSymbol@core::~ChainSymbol()"},
    {"_ZN4coreW4core11ChainSymbolD2Ev",
     "core::ChainSymbol@core::~ChainSymbol()"},
    {"_ZN4coreW4core11ConstSymbolC2ENS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEE",
     "core::ConstSymbol@core::ConstSymbol(core::UniquePtr@core<core::"
     "DemangledSymbol@core>)"},
    {"_ZN4coreW4core11ConstSymbolD0Ev",
     "core::ConstSymbol@core::~ConstSymbol()"},
    {"_ZN4coreW4core11ConstSymbolD2Ev",
     "core::ConstSymbol@core::~ConstSymbol()"},
    {"_ZN4coreW4core11getFileSizeEi", "core::getFileSize@core(int)"},
    {"_ZN4coreW4core11symbolNamesE", "core::symbolNames@core"},
    {"_ZN4coreW4core12acquireMutexEPVi",
     "core::acquireMutex@core(int volatile*)"},
    {"_ZN4coreW4core12demangleNameERPKcRNS_S0_6VectorINS_S0_9UniquePtrINS_"
     "S0_"
     "15DemangledSymbolEEEEES9_Rb",
     "core::demangleName@core(char const*&, "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>>&,"
     " "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>>&,"
     " "
     "bool&)"},
    {"_ZN4coreW4core12demangleTypeERPKcRNS_S0_6VectorINS_S0_9UniquePtrINS_"
     "S0_"
     "15DemangledSymbolEEEEES9_RbPS8_",
     "core::demangleType@core(char const*&, "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>>&,"
     " "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>>&,"
     " "
     "bool&, "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">*)"},
    {"_ZN4coreW4core12LambdaSymbol7addTypeENS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEE",
     "core::LambdaSymbol@core::addType(core::UniquePtr@core<core::"
     "DemangledSymbol@core>)"},
    {"_ZN4coreW4core12LambdaSymbolC2Ev",
     "core::LambdaSymbol@core::LambdaSymbol()"},
    {"_ZN4coreW4core12LambdaSymbolD0Ev",
     "core::LambdaSymbol@core::~LambdaSymbol()"},
    {"_ZN4coreW4core12LambdaSymbolD2Ev",
     "core::LambdaSymbol@core::~LambdaSymbol()"},
    {"_ZN4coreW4core12ModuleSymbolC2ENS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEES4_",
     "core::ModuleSymbol@core::ModuleSymbol(core::UniquePtr@core<core::"
     "DemangledSymbol@core>, "
     "core::UniquePtr@core<core::DemangledSymbol@core>)"},
    {"_ZN4coreW4core12ModuleSymbolD0Ev",
     "core::ModuleSymbol@core::~ModuleSymbol()"},
    {"_ZN4coreW4core12ModuleSymbolD2Ev",
     "core::ModuleSymbol@core::~ModuleSymbol()"},
    {"_ZN4coreW4core12releaseMutexEPVi",
     "core::releaseMutex@core(int volatile*)"},
    {"_ZN4coreW4core13BuiltinSymbolC2ENS_S0_6StringE",
     "core::BuiltinSymbol@core::BuiltinSymbol(core::String@core)"},
    {"_ZN4coreW4core13BuiltinSymbolD0Ev",
     "core::BuiltinSymbol@core::~BuiltinSymbol()"},
    {"_ZN4coreW4core13BuiltinSymbolD2Ev",
     "core::BuiltinSymbol@core::~BuiltinSymbol()"},
    {"_ZN4coreW4core13FunTypeSymbol11addArgumentENS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEE",
     "core::FunTypeSymbol@core::addArgument(core::UniquePtr@core<core::"
     "DemangledSymbol@core>)"},
    {"_ZN4coreW4core13FunTypeSymbolC2ENS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEE",
     "core::FunTypeSymbol@core::FunTypeSymbol(core::UniquePtr@core<core::"
     "DemangledSymbol@core>)"},
    {"_ZN4coreW4core13FunTypeSymbolD0Ev",
     "core::FunTypeSymbol@core::~FunTypeSymbol()"},
    {"_ZN4coreW4core13FunTypeSymbolD2Ev",
     "core::FunTypeSymbol@core::~FunTypeSymbol()"},
    {"_ZN4coreW4core13LogicalResultINS_S0_6StringEE7failureES2_",
     "core::LogicalResult@core<core::String@core>::failure(core::String@"
     "core)"},
    {"_ZN4coreW4core13LogicalResultINS_S0_6StringEE8getValueEv",
     "core::LogicalResult@core<core::String@core>::getValue()"},
    {"_ZN4coreW4core13LogicalResultINS_S0_6StringEEC2EbS2_OS2_",
     "core::LogicalResult@core<core::String@core>::LogicalResult(bool, "
     "core::String@core, core::String@core&&)"},
    {"_ZN4coreW4core13LogicalResultINS_S0_6StringEEC2EOS2_",
     "core::LogicalResult@core<core::String@core>::LogicalResult(core::"
     "String@core&&)"},
    {"_ZN4coreW4core13LogicalResultINS_S0_6StringEEC2ERKS2_",
     "core::LogicalResult@core<core::String@core>::LogicalResult(core::"
     "String@core const&)"},
    {"_ZN4coreW4core13LogicalResultINS_S0_6StringEEcvNS1_IT_EEINS_S0_"
     "9UniquePtrINS_S0_15DemangledSymbolEEEEEv",
     "core::LogicalResult@core<core::String@core>::operator "
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>><core::UniquePtr@core<core::DemangledSymbol@core>>()"},
    {"_ZN4coreW4core13LogicalResultINS_S0_6StringEED2Ev",
     "core::LogicalResult@core<core::String@core>::~LogicalResult()"},
    {"_ZN4coreW4core13LogicalResultINS_S0_6StringEEdeEv",
     "core::LogicalResult@core<core::String@core>::operator*()"},
    {"_ZN4coreW4core13LogicalResultINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE7failureENS_S0_6StringE",
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>>::failure(core::String@core)"},
    {"_ZN4coreW4core13LogicalResultINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE8getValueEv",
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>>::getValue()"},
    {"_ZN4coreW4core13LogicalResultINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEEC2EbNS_S0_6StringEOS4_",
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>>::LogicalResult(bool, core::String@core, "
     "core::UniquePtr@core<core::DemangledSymbol@core>&&)"},
    {"_ZN4coreW4core13LogicalResultINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEEC2EOS4_",
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>>::LogicalResult(core::UniquePtr@core<core::DemangledSymbol@"
     "core>&"
     "&)"},
    {"_ZN4coreW4core13LogicalResultINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEEC2EOS5_",
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>>::LogicalResult(core::LogicalResult@core<core::UniquePtr@core<"
     "core::"
     "DemangledSymbol@core>>&&)"},
    {"_ZN4coreW4core13LogicalResultINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEEcvNS1_IT_EEINS_S0_6StringEEEv",
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>>::operator "
     "core::LogicalResult@core<core::String@core><core::String@core>()"},
    {"_ZN4coreW4core13LogicalResultINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEED2Ev",
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>>::~LogicalResult()"},
    {"_ZN4coreW4core13LogicalResultINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEEdeEv",
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>>::operator*()"},
    {"_ZN4coreW4core13LogicalResultINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEEptEv",
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>>::operator->()"},
    {"_ZN4coreW4core13LogicalResultIvE7failureENS_S0_6StringE",
     "core::LogicalResult@core<void>::failure(core::String@core)"},
    {"_ZN4coreW4core13LogicalResultIvEC2EbNS_S0_6StringE",
     "core::LogicalResult@core<void>::LogicalResult(bool, "
     "core::String@core)"},
    {"_ZN4coreW4core13LogicalResultIvEcvNS1_IT_EEINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEEEv",
     "core::LogicalResult@core<void>::operator "
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>><core::UniquePtr@core<core::DemangledSymbol@core>>()"},
    {"_ZN4coreW4core13LogicalResultIvED2Ev",
     "core::LogicalResult@core<void>::~LogicalResult()"},
    {"_ZN4coreW4core13RequireSymbol7addCondENS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEE",
     "core::RequireSymbol@core::addCond(core::UniquePtr@core<core::"
     "DemangledSymbol@core>)"},
    {"_ZN4coreW4core13RequireSymbolC2Ev",
     "core::RequireSymbol@core::RequireSymbol()"},
    {"_ZN4coreW4core13RequireSymbolD0Ev",
     "core::RequireSymbol@core::~RequireSymbol()"},
    {"_ZN4coreW4core13RequireSymbolD2Ev",
     "core::RequireSymbol@core::~RequireSymbol()"},
    {"_ZN4coreW4core13VirtualSymbolC2ENS1_11virtualTypeENS_S0_"
     "9UniquePtrINS_"
     "S0_15DemangledSymbolEEE",
     "core::VirtualSymbol@core::VirtualSymbol(core::VirtualSymbol@core::"
     "virtualType, core::UniquePtr@core<core::DemangledSymbol@core>)"},
    {"_ZN4coreW4core13VirtualSymbolD0Ev",
     "core::VirtualSymbol@core::~VirtualSymbol()"},
    {"_ZN4coreW4core13VirtualSymbolD2Ev",
     "core::VirtualSymbol@core::~VirtualSymbol()"},
    {"_ZN4coreW4core14demangleModuleERPKcRNS_S0_6VectorINS_S0_"
     "9UniquePtrINS_"
     "S0_15DemangledSymbolEEEEES9_",
     "core::demangleModule@core(char const*&, "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>>&,"
     " "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">&)"},
    {"_ZN4coreW4core14TemplateSymbol11addTemplateENS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEE",
     "core::TemplateSymbol@core::addTemplate(core::UniquePtr@core<core::"
     "DemangledSymbol@core>)"},
    {"_ZN4coreW4core14TemplateSymbolC2Ev",
     "core::TemplateSymbol@core::TemplateSymbol()"},
    {"_ZN4coreW4core14TemplateSymbolD0Ev",
     "core::TemplateSymbol@core::~TemplateSymbol()"},
    {"_ZN4coreW4core14TemplateSymbolD2Ev",
     "core::TemplateSymbol@core::~TemplateSymbol()"},
    {"_ZN4coreW4core14VolatileSymbolC2ENS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEE",
     "core::VolatileSymbol@core::VolatileSymbol(core::UniquePtr@core<core::"
     "DemangledSymbol@core>)"},
    {"_ZN4coreW4core14VolatileSymbolD0Ev",
     "core::VolatileSymbol@core::~VolatileSymbol()"},
    {"_ZN4coreW4core14VolatileSymbolD2Ev",
     "core::VolatileSymbol@core::~VolatileSymbol()"},
    {"_ZN4coreW4core15DemangledSymbolC2ENS1_4typeE",
     "core::DemangledSymbol@core::DemangledSymbol(core::DemangledSymbol@"
     "core:"
     ":type)"},
    {"_ZN4coreW4core15DemangledSymbolD0Ev",
     "core::DemangledSymbol@core::~DemangledSymbol()"},
    {"_ZN4coreW4core15DemangledSymbolD2Ev",
     "core::DemangledSymbol@core::~DemangledSymbol()"},
    {"_ZN4coreW4core16demangleTemplateERPKcRNS_S0_6VectorINS_S0_"
     "9UniquePtrINS_S0_15DemangledSymbolEEEEES9_",
     "core::demangleTemplate@core(char const*&, "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>>&,"
     " "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">&)"},
    {"_ZN4coreW4core16DestructorSymbolC2Ev",
     "core::DestructorSymbol@core::DestructorSymbol()"},
    {"_ZN4coreW4core16DestructorSymbolD0Ev",
     "core::DestructorSymbol@core::~DestructorSymbol()"},
    {"_ZN4coreW4core16DestructorSymbolD2Ev",
     "core::DestructorSymbol@core::~DestructorSymbol()"},
    {"_ZN4coreW4core16ModuleBaseSymbolC2ENS_S0_6StringE",
     "core::ModuleBaseSymbol@core::ModuleBaseSymbol(core::String@core)"},
    {"_ZN4coreW4core16ModuleBaseSymbolD0Ev",
     "core::ModuleBaseSymbol@core::~ModuleBaseSymbol()"},
    {"_ZN4coreW4core16ModuleBaseSymbolD2Ev",
     "core::ModuleBaseSymbol@core::~ModuleBaseSymbol()"},
    {"_ZN4coreW4core17ConstructorSymbolC2Ev",
     "core::ConstructorSymbol@core::ConstructorSymbol()"},
    {"_ZN4coreW4core17ConstructorSymbolD0Ev",
     "core::ConstructorSymbol@core::~ConstructorSymbol()"},
    {"_ZN4coreW4core17ConstructorSymbolD2Ev",
     "core::ConstructorSymbol@core::~ConstructorSymbol()"},
    {"_ZN4coreW4core17cxaAtExitElementsE", "core::cxaAtExitElements@core"},
    {"_ZN4coreW4core17initializeLoggersEv", "core::initializeLoggers@core()"},
    {"_ZN4coreW4core18demangleExpressionILb1EEENS_S0_13LogicalResultINS_S0_"
     "9UniquePtrINS_S0_15DemangledSymbolEEEEERPKcRNS_S0_6VectorIS5_EESC_",
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>> core::demangleExpression@core<true>(char const*&, "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>>&,"
     " "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">&)"},
    {"_ZN4coreW4core18demangleNestedNameERPKcRNS_S0_6VectorINS_S0_"
     "9UniquePtrINS_S0_15DemangledSymbolEEEEES9_",
     "core::demangleNestedName@core(char const*&, "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>>&,"
     " "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">&)"},
    {"_ZN4coreW4core19demangleExprPrimaryERPKcRNS_S0_6VectorINS_S0_"
     "9UniquePtrINS_S0_15DemangledSymbolEEEEES9_",
     "core::demangleExprPrimary@core(char const*&, "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>>&,"
     " "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">&)"},
    {"_ZN4coreW4core21initializeSymbolNamesEPKc",
     "core::initializeSymbolNames@core(char const*)"},
    {"_ZN4coreW4core22parseSubstitutionIndexERmRPKc",
     "core::parseSubstitutionIndex@core(unsigned long&, char const*&)"},
    {"_ZN4coreW4core24generateTypePrefixSuffixENS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEE",
     "core::generateTypePrefixSuffix@core(core::UniquePtr@core<core::"
     "DemangledSymbol@core>)"},
    {"_ZN4coreW4core3HexC2Em", "core::Hex@core::Hex(unsigned long)"},
    {"_ZN4coreW4core3HexcvmEv", "core::Hex@core::operator unsigned long()"},
    {"_ZN4coreW4core3MapImNS_S0_6StringEE5EntryD2Ev",
     "core::Map@core<unsigned long, core::String@core>::Entry::~Entry()"},
    {"_ZN4coreW4core3MapImNS_S0_6StringEE6insertEmOS2_",
     "core::Map@core<unsigned long, core::String@core>::insert(unsigned "
     "long, core::String@core&&)"},
    {"_ZN4coreW4core3MapImNS_S0_6StringEE8IteratorC2EPNS3_9EntryListE",
     "core::Map@core<unsigned long, "
     "core::String@core>::Iterator::Iterator(core::Map@core<unsigned long, "
     "core::String@core>::EntryList*)"},
    {"_ZN4coreW4core3MapImNS_S0_6StringEE8IteratorppEv",
     "core::Map@core<unsigned long, "
     "core::String@core>::Iterator::operator++()"},
    {"_ZN4coreW4core3MapImNS_S0_6StringEE9EntryListD2Ev",
     "core::Map@core<unsigned long, "
     "core::String@core>::EntryList::~EntryList()"},
    {"_ZN4coreW4core3MapImNS_S0_6StringEED2Ev",
     "core::Map@core<unsigned long, core::String@core>::~Map()"},
    {"_ZN4coreW4core3MapINS_S0_6StringES2_E3getES2_",
     "core::Map@core<core::String@core, "
     "core::String@core>::get(core::String@core)"},
    {"_ZN4coreW4core3MapINS_S0_6StringES2_E5EntryC2ERKS4_",
     "core::Map@core<core::String@core, "
     "core::String@core>::Entry::Entry(core::Map@core<core::String@core, "
     "core::String@core>::Entry const&)"},
    {"_ZN4coreW4core3MapINS_S0_6StringES2_E5EntryD2Ev",
     "core::Map@core<core::String@core, "
     "core::String@core>::Entry::~Entry()"},
    {"_ZN4coreW4core3MapINS_S0_6StringES2_E6insertES2_OS2_",
     "core::Map@core<core::String@core, "
     "core::String@core>::insert(core::String@core, core::String@core&&)"},
    {"_ZN4coreW4core3MapINS_S0_6StringES2_E6insertES2_RKS2_",
     "core::Map@core<core::String@core, "
     "core::String@core>::insert(core::String@core, core::String@core "
     "const&)"},
    {"_ZN4coreW4core3MapINS_S0_6StringES2_E8IteratorC2EPNS3_9EntryListE",
     "core::Map@core<core::String@core, "
     "core::String@core>::Iterator::Iterator(core::Map@core<core::String@"
     "core, core::String@core>::EntryList*)"},
    {"_ZN4coreW4core3MapINS_S0_6StringES2_E8IteratorppEv",
     "core::Map@core<core::String@core, "
     "core::String@core>::Iterator::operator++()"},
    {"_ZN4coreW4core3MapINS_S0_6StringES2_E9EntryListD2Ev",
     "core::Map@core<core::String@core, "
     "core::String@core>::EntryList::~EntryList()"},
    {"_ZN4coreW4core3MapINS_S0_6StringES2_EC2EStS0_16initializer_listINS3_"
     "5EntryEE",
     "core::Map@core<core::String@core, "
     "core::String@core>::Map(std::initializer_list@core<core::Map@core<"
     "core:"
     ":String@core, core::String@core>::Entry>)"},
    {"_ZN4coreW4core3MapINS_S0_6StringES2_EC2Ev",
     "core::Map@core<core::String@core, core::String@core>::Map()"},
    {"_ZN4coreW4core3MapINS_S0_6StringES2_ED2Ev",
     "core::Map@core<core::String@core, core::String@core>::~Map()"},
    {"_ZN4coreW4core4mmapEPvmmmim",
     "core::mmap@core(void*, unsigned long, unsigned long, unsigned long, "
     "int, unsigned long)"},
    {"_ZN4coreW4core4moveIRbEENS_S0_10RValueBaseIT_E4typeEOS4_",
     "core::RValueBase@core<bool&>::type core::move@core<bool&>(bool&)"},
    {"_ZN4coreW4core4moveIRNS_S0_5MutexEEENS_S0_10RValueBaseIT_E4typeEOS5_",
     "core::RValueBase@core<core::Mutex@core&>::type "
     "core::move@core<core::Mutex@core&>(core::Mutex@core&)"},
    {"_ZN4coreW4core4moveIRNS_S0_6StringEEENS_S0_10RValueBaseIT_"
     "E4typeEOS5_",
     "core::RValueBase@core<core::String@core&>::type "
     "core::move@core<core::String@core&>(core::String@core&)"},
    {"_ZN4coreW4core4moveIRNS_S0_9UniquePtrINS_S0_15DemangledSymbolEEEEENS_"
     "S0_10RValueBaseIT_E4typeEOS7_",
     "core::RValueBase@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>&"
     ">::type "
     "core::move@core<core::UniquePtr@core<core::DemangledSymbol@core>&>("
     "core::UniquePtr@core<core::DemangledSymbol@core>&)"},
    {"_ZN4coreW4core4moveIRPNS_S0_8FdStreamEEENS_S0_10RValueBaseIT_"
     "E4typeEOS6_",
     "core::RValueBase@core<core::FdStream@core*&>::type "
     "core::move@core<core::FdStream@core*&>(core::FdStream@core*&)"},
    {"_ZN4coreW4core4openEPKcmm",
     "core::open@core(char const*, unsigned long, unsigned long)"},
    {"_ZN4coreW4core4PairINS_S0_6StringES2_ED2Ev",
     "core::Pair@core<core::String@core, core::String@core>::~Pair()"},
    {"_ZN4coreW4core4seekEill", "core::seek@core(int, long, long)"},
    {"_ZN4coreW4core4serrE", "core::serr@core"},
    {"_ZN4coreW4core4soutE", "core::sout@core"},
    {"_ZN4coreW4core5closeEi", "core::close@core(int)"},
    {"_ZN4coreW4core5Mutex7acquireEv", "core::Mutex@core::acquire()"},
    {"_ZN4coreW4core5Mutex7releaseEv", "core::Mutex@core::release()"},
    {"_ZN4coreW4core5MutexC2Ev", "core::Mutex@core::Mutex()"},
    {"_ZN4coreW4core5streqEPKcS2_",
     "core::streq@core(char const*, char const*)"},
    {"_ZN4coreW4core5streqEPKcS2_m",
     "core::streq@core(char const*, char const*, unsigned long)"},
    {"_ZN4coreW4core5writeEiPKc", "core::write@core(int, char const*)"},
    {"_ZN4coreW4core5writeEiPKcj",
     "core::write@core(int, char const*, unsigned int)"},
    {"_ZN4coreW4core6assertEb", "core::assert@core(bool)"},
    {"_ZN4coreW4core6assertEbPKc", "core::assert@core(bool, char const*)"},
    {"_ZN4coreW4core6Logger5errorIJPKcEEEvDpT_",
     "void core::Logger@core::error<char const*>(char const*)"},
    {"_ZN4coreW4core6Logger9errorImplIPKcEEvT_",
     "void core::Logger@core::errorImpl<char const*>(char const*)"},
    {"_ZN4coreW4core6LoggeraSEOS1_",
     "core::Logger@core::operator=(core::Logger@core&&)"},
    {"_ZN4coreW4core6LoggerC2EPNS_S0_8FdStreamES3_S3_bb",
     "core::Logger@core::Logger(core::FdStream@core*, "
     "core::FdStream@core*, "
     "core::FdStream@core*, bool, bool)"},
    {"_ZN4coreW4core6munmapEPvm", "core::munmap@core(void*, unsigned long)"},
    {"_ZN4coreW4core6String10startsWithES1_",
     "core::String@core::startsWith(core::String@core)"},
    {"_ZN4coreW4core6String2ofEm", "core::String@core::of(unsigned long)"},
    {"_ZN4coreW4core6String8endsWithEc", "core::String@core::endsWith(char)"},
    {"_ZN4coreW4core6StringaSEOS1_",
     "core::String@core::operator=(core::String@core&&)"},
    {"_ZN4coreW4core6StringaSERKS1_",
     "core::String@core::operator=(core::String@core const&)"},
    {"_ZN4coreW4core6StringC2Ec", "core::String@core::String(char)"},
    {"_ZN4coreW4core6StringC2EOS1_",
     "core::String@core::String(core::String@core&&)"},
    {"_ZN4coreW4core6StringC2EPKc", "core::String@core::String(char const*)"},
    {"_ZN4coreW4core6StringC2ERKS1_",
     "core::String@core::String(core::String@core const&)"},
    {"_ZN4coreW4core6StringC2Ev", "core::String@core::String()"},
    {"_ZN4coreW4core6StringD2Ev", "core::String@core::~String()"},
    {"_ZN4coreW4core6StringpLEc", "core::String@core::operator+=(char)"},
    {"_ZN4coreW4core6StringpLERKS1_",
     "core::String@core::operator+=(core::String@core const&)"},
    {"_ZN4coreW4core6StringplERS1_",
     "core::String@core::operator+(core::String@core&)"},
    {"_ZN4coreW4core6strlenEPKc", "core::strlen@core(char const*)"},
    {"_ZN4coreW4core6Symbol7addTypeENS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEE",
     "core::Symbol@core::addType(core::UniquePtr@core<core::"
     "DemangledSymbol@"
     "core>)"},
    {"_ZN4coreW4core6SymbolC2ENS_S0_9UniquePtrINS_S0_15DemangledSymbolEEE",
     "core::Symbol@core::Symbol(core::UniquePtr@core<core::DemangledSymbol@"
     "core>)"},
    {"_ZN4coreW4core6SymbolD0Ev", "core::Symbol@core::~Symbol()"},
    {"_ZN4coreW4core6SymbolD2Ev", "core::Symbol@core::~Symbol()"},
    {"_ZN4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE15ReverseIteratorC2EPS4_m",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::ReverseIterator::ReverseIterator(core::UniquePtr@core<core::"
     "DemangledSymbol@core>*, unsigned long)"},
    {"_ZN4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE15ReverseIteratorppEv",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::ReverseIterator::operator++()"},
    {"_ZN4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE4growEv",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::grow()"},
    {"_ZN4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE8IteratorC2EPS4_m",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::Iterator::Iterator(core::UniquePtr@core<core::DemangledSymbol@"
     "core>*"
     ", unsigned long)"},
    {"_ZN4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE8IteratorppEv",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::Iterator::operator++()"},
    {"_ZN4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE8pushBackEOS4_",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::pushBack(core::UniquePtr@core<core::DemangledSymbol@core>&&)"},
    {"_ZN4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEEC2Ev",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::Vector()"},
    {"_ZN4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEED2Ev",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::~Vector()"},
    {"_ZN4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEEixEm",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::operator[](unsigned long)"},
    {"_ZN4coreW4core7forwardIPKcEEOT_RNS_S0_13RemoveRefBaseIS4_E4typeE",
     "char const*&& core::forward@core<char "
     "const*>(core::RemoveRefBase@core<char const*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_10BaseSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::BaseSymbol@core*&& "
     "core::forward@core<core::BaseSymbol@core*>(core::RemoveRefBase@core<"
     "core::BaseSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_10LvalSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::LvalSymbol@core*&& "
     "core::forward@core<core::LvalSymbol@core*>(core::RemoveRefBase@core<"
     "core::LvalSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_10RvalSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::RvalSymbol@core*&& "
     "core::forward@core<core::RvalSymbol@core*>(core::RemoveRefBase@core<"
     "core::RvalSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_11ChainSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::ChainSymbol@core*&& "
     "core::forward@core<core::ChainSymbol@core*>(core::RemoveRefBase@core<"
     "core::ChainSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_11ConstSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::ConstSymbol@core*&& "
     "core::forward@core<core::ConstSymbol@core*>(core::RemoveRefBase@core<"
     "core::ConstSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_12LambdaSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::LambdaSymbol@core*&& "
     "core::forward@core<core::LambdaSymbol@core*>(core::RemoveRefBase@"
     "core<"
     "core::LambdaSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_12ModuleSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::ModuleSymbol@core*&& "
     "core::forward@core<core::ModuleSymbol@core*>(core::RemoveRefBase@"
     "core<"
     "core::ModuleSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_13BuiltinSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::BuiltinSymbol@core*&& "
     "core::forward@core<core::BuiltinSymbol@core*>(core::RemoveRefBase@"
     "core<"
     "core::BuiltinSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_13FunTypeSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::FunTypeSymbol@core*&& "
     "core::forward@core<core::FunTypeSymbol@core*>(core::RemoveRefBase@"
     "core<"
     "core::FunTypeSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_13VirtualSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::VirtualSymbol@core*&& "
     "core::forward@core<core::VirtualSymbol@core*>(core::RemoveRefBase@"
     "core<"
     "core::VirtualSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_14TemplateSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::TemplateSymbol@core*&& "
     "core::forward@core<core::TemplateSymbol@core*>(core::RemoveRefBase@"
     "core<core::TemplateSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_14VolatileSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::VolatileSymbol@core*&& "
     "core::forward@core<core::VolatileSymbol@core*>(core::RemoveRefBase@"
     "core<core::VolatileSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_16DestructorSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::DestructorSymbol@core*&& "
     "core::forward@core<core::DestructorSymbol@core*>(core::RemoveRefBase@"
     "core<core::DestructorSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_17ConstructorSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::ConstructorSymbol@core*&& "
     "core::forward@core<core::ConstructorSymbol@core*>(core::"
     "RemoveRefBase@"
     "core<core::ConstructorSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_6SymbolEEEOT_RNS_S0_13RemoveRefBaseIS4_"
     "E4typeE",
     "core::Symbol@core*&& "
     "core::forward@core<core::Symbol@core*>(core::RemoveRefBase@core<core:"
     ":"
     "Symbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_9PtrSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::PtrSymbol@core*&& "
     "core::forward@core<core::PtrSymbol@core*>(core::RemoveRefBase@core<"
     "core::PtrSymbol@core*>::type&)"},
    {"_ZN4coreW4core7forwardIPNS_S0_9StdSymbolEEEOT_RNS_S0_"
     "13RemoveRefBaseIS4_E4typeE",
     "core::StdSymbol@core*&& "
     "core::forward@core<core::StdSymbol@core*>(core::RemoveRefBase@core<"
     "core::StdSymbol@core*>::type&)"},
    {"_ZN4coreW4core7tlsInfoE", "core::tlsInfo@core"},
    {"_ZN4coreW4core8auxvValsE", "core::auxvVals@core"},
    {"_ZN4coreW4core8demangleENS_S0_6StringE",
     "core::demangle@core(core::String@core)"},
    {"_ZN4coreW4core8demangleILb0EEENS_S0_13LogicalResultINS_S0_"
     "9UniquePtrINS_S0_15DemangledSymbolEEEEERPKcRNS_S0_6VectorIS5_EESC_",
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>> core::demangle@core<false>(char const*&, "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>>&,"
     " "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">&)"},
    {"_ZN4coreW4core8demangleILb1EEENS_S0_13LogicalResultINS_S0_"
     "9UniquePtrINS_S0_15DemangledSymbolEEEEERPKcRNS_S0_6VectorIS5_EESC_",
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>> core::demangle@core<true>(char const*&, "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>>&,"
     " "
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">&)"},
    {"_ZN4coreW4core8FdStreamlsEm",
     "core::FdStream@core::operator<<(unsigned long)"},
    {"_ZN4coreW4core8FdStreamlsENS_S0_6StringE",
     "core::FdStream@core::operator<<(core::String@core)"},
    {"_ZN4coreW4core8FdStreamlsEPKc",
     "core::FdStream@core::operator<<(char const*)"},
    {"_ZN4coreW4core8printIntILb0EEERNS_S0_8FdStreamES3_m",
     "core::FdStream@core& "
     "core::printInt@core<false>(core::FdStream@core&, "
     "unsigned long)"},
    {"_ZN4coreW4core9AllocatorILm32076800ELm1024EE3getEv",
     "core::Allocator@core<32076800ul, 1024ul>::get()"},
    {"_ZN4coreW4core9AllocatorILm32076800ELm1024EE4freeEPv",
     "core::Allocator@core<32076800ul, 1024ul>::free(void*)"},
    {"_ZN4coreW4core9AllocatorILm32076800ELm1024EE5mergeEPNS2_6HeaderE",
     "core::Allocator@core<32076800ul, "
     "1024ul>::merge(core::Allocator@core<32076800ul, 1024ul>::Header*)"},
    {"_ZN4coreW4core9AllocatorILm32076800ELm1024EE5splitEPNS2_6HeaderEm",
     "core::Allocator@core<32076800ul, "
     "1024ul>::split(core::Allocator@core<32076800ul, 1024ul>::Header*, "
     "unsigned long)"},
    {"_ZN4coreW4core9AllocatorILm32076800ELm1024EE6Header7getNextEv",
     "core::Allocator@core<32076800ul, 1024ul>::Header::getNext()"},
    {"_ZN4coreW4core9AllocatorILm32076800ELm1024EE8allocateEm",
     "core::Allocator@core<32076800ul, 1024ul>::allocate(unsigned long)"},
    {"_ZN4coreW4core9AllocatorILm32076800ELm1024EE8reinsertEPNS2_6HeaderE",
     "core::Allocator@core<32076800ul, "
     "1024ul>::reinsert(core::Allocator@core<32076800ul, "
     "1024ul>::Header*)"},
    {"_ZN4coreW4core9AllocatorILm32076800ELm1024EE9findBlockEm",
     "core::Allocator@core<32076800ul, 1024ul>::findBlock(unsigned long)"},
    {"_ZN4coreW4core9AllocatorILm32076800ELm1024EE9needSplitEPNS2_"
     "6HeaderEm",
     "core::Allocator@core<32076800ul, "
     "1024ul>::needSplit(core::Allocator@core<32076800ul, "
     "1024ul>::Header*, "
     "unsigned long)"},
    {"_ZN4coreW4core9AllocatorILm32076800ELm1024EEC2Ev",
     "core::Allocator@core<32076800ul, 1024ul>::Allocator()"},
    {"_ZN4coreW4core9parseSizeERmRPKc",
     "core::parseSize@core(unsigned long&, char const*&)"},
    {"_ZN4coreW4core9PtrSymbolC2ENS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEE",
     "core::PtrSymbol@core::PtrSymbol(core::UniquePtr@core<core::"
     "DemangledSymbol@core>)"},
    {"_ZN4coreW4core9PtrSymbolD0Ev", "core::PtrSymbol@core::~PtrSymbol()"},
    {"_ZN4coreW4core9PtrSymbolD2Ev", "core::PtrSymbol@core::~PtrSymbol()"},
    {"_ZN4coreW4core9stdLoggerE", "core::stdLogger@core"},
    {"_ZN4coreW4core9StdSymbolC2Ev", "core::StdSymbol@core::StdSymbol()"},
    {"_ZN4coreW4core9StdSymbolD0Ev", "core::StdSymbol@core::~StdSymbol()"},
    {"_ZN4coreW4core9StdSymbolD2Ev", "core::StdSymbol@core::~StdSymbol()"},
    {"_ZN4coreW4core9UniquePtrINS_S0_15DemangledSymbolEEaSEOS3_",
     "core::UniquePtr@core<core::DemangledSymbol@core>::operator=(core::"
     "UniquePtr@core<core::DemangledSymbol@core>&&)"},
    {"_ZN4coreW4core9UniquePtrINS_S0_15DemangledSymbolEEC2EDn",
     "core::UniquePtr@core<core::DemangledSymbol@core>::UniquePtr(decltype("
     "nullptr))"},
    {"_ZN4coreW4core9UniquePtrINS_S0_15DemangledSymbolEEC2EOS3_",
     "core::UniquePtr@core<core::DemangledSymbol@core>::UniquePtr(core::"
     "UniquePtr@core<core::DemangledSymbol@core>&&)"},
    {"_ZN4coreW4core9UniquePtrINS_S0_15DemangledSymbolEEC2EPS2_",
     "core::UniquePtr@core<core::DemangledSymbol@core>::UniquePtr(core::"
     "DemangledSymbol@core*)"},
    {"_ZN4coreW4core9UniquePtrINS_S0_15DemangledSymbolEEC2Ev",
     "core::UniquePtr@core<core::DemangledSymbol@core>::UniquePtr()"},
    {"_ZN4coreW4core9UniquePtrINS_S0_15DemangledSymbolEED2Ev",
     "core::UniquePtr@core<core::DemangledSymbol@core>::~UniquePtr()"},
    {"_ZN4coreW4coreplEPKcRKNS_S0_6StringE",
     "core::operator+@core(char const*, core::String@core const&)"},
    {"_ZN4coreW4coreplERKNS_S0_6StringEPKc",
     "core::operator+@core(core::String@core const&, char const*)"},
    {"_Znam", "operator new[](unsigned long)"},
    {"_ZNK4coreW4core10BaseSymbol10isConstFunEv",
     "core::BaseSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core10BaseSymbol11getLastNameEv",
     "core::BaseSymbol@core::getLastName() const"},
    {"_ZNK4coreW4core10BaseSymbol11hasTemplateEv",
     "core::BaseSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core10BaseSymbol13isConstructorEv",
     "core::BaseSymbol@core::isConstructor() const"},
    {"_ZNK4coreW4core10BaseSymbol5cloneEv",
     "core::BaseSymbol@core::clone() const"},
    {"_ZNK4coreW4core10BaseSymbol8toStringEb",
     "core::BaseSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core10LvalSymbol10isConstFunEv",
     "core::LvalSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core10LvalSymbol11hasTemplateEv",
     "core::LvalSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core10LvalSymbol5cloneEv",
     "core::LvalSymbol@core::clone() const"},
    {"_ZNK4coreW4core10LvalSymbol8toStringEb",
     "core::LvalSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core10RvalSymbol10isConstFunEv",
     "core::RvalSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core10RvalSymbol11hasTemplateEv",
     "core::RvalSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core10RvalSymbol5cloneEv",
     "core::RvalSymbol@core::clone() const"},
    {"_ZNK4coreW4core10RvalSymbol8toStringEb",
     "core::RvalSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core11ChainSymbol10isConstFunEv",
     "core::ChainSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core11ChainSymbol11getLastNameEv",
     "core::ChainSymbol@core::getLastName() const"},
    {"_ZNK4coreW4core11ChainSymbol11hasTemplateEv",
     "core::ChainSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core11ChainSymbol13isConstructorEv",
     "core::ChainSymbol@core::isConstructor() const"},
    {"_ZNK4coreW4core11ChainSymbol5cloneEv",
     "core::ChainSymbol@core::clone() const"},
    {"_ZNK4coreW4core11ChainSymbol8toStringEb",
     "core::ChainSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core11ConstSymbol10isConstFunEv",
     "core::ConstSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core11ConstSymbol11hasTemplateEv",
     "core::ConstSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core11ConstSymbol13isConstructorEv",
     "core::ConstSymbol@core::isConstructor() const"},
    {"_ZNK4coreW4core11ConstSymbol5cloneEv",
     "core::ConstSymbol@core::clone() const"},
    {"_ZNK4coreW4core11ConstSymbol8toStringEb",
     "core::ConstSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core12LambdaSymbol10isConstFunEv",
     "core::LambdaSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core12LambdaSymbol11hasTemplateEv",
     "core::LambdaSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core12LambdaSymbol5cloneEv",
     "core::LambdaSymbol@core::clone() const"},
    {"_ZNK4coreW4core12LambdaSymbol8toStringEb",
     "core::LambdaSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core12ModuleSymbol10isConstFunEv",
     "core::ModuleSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core12ModuleSymbol11getLastNameEv",
     "core::ModuleSymbol@core::getLastName() const"},
    {"_ZNK4coreW4core12ModuleSymbol11hasTemplateEv",
     "core::ModuleSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core12ModuleSymbol13isConstructorEv",
     "core::ModuleSymbol@core::isConstructor() const"},
    {"_ZNK4coreW4core12ModuleSymbol5cloneEv",
     "core::ModuleSymbol@core::clone() const"},
    {"_ZNK4coreW4core12ModuleSymbol8toStringEb",
     "core::ModuleSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core13BuiltinSymbol10isConstFunEv",
     "core::BuiltinSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core13BuiltinSymbol10isVoidTypeEv",
     "core::BuiltinSymbol@core::isVoidType() const"},
    {"_ZNK4coreW4core13BuiltinSymbol11getLastNameEv",
     "core::BuiltinSymbol@core::getLastName() const"},
    {"_ZNK4coreW4core13BuiltinSymbol11hasTemplateEv",
     "core::BuiltinSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core13BuiltinSymbol13isConstructorEv",
     "core::BuiltinSymbol@core::isConstructor() const"},
    {"_ZNK4coreW4core13BuiltinSymbol5cloneEv",
     "core::BuiltinSymbol@core::clone() const"},
    {"_ZNK4coreW4core13BuiltinSymbol8toStringEb",
     "core::BuiltinSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core13FunTypeSymbol10isConstFunEv",
     "core::FunTypeSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core13FunTypeSymbol11hasTemplateEv",
     "core::FunTypeSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core13FunTypeSymbol5cloneEv",
     "core::FunTypeSymbol@core::clone() const"},
    {"_ZNK4coreW4core13FunTypeSymbol8toStringEb",
     "core::FunTypeSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core13LogicalResultINS_S0_6StringEE10getMessageEv",
     "core::LogicalResult@core<core::String@core>::getMessage() const"},
    {"_ZNK4coreW4core13LogicalResultINS_S0_6StringEE6failedEv",
     "core::LogicalResult@core<core::String@core>::failed() const"},
    {"_ZNK4coreW4core13LogicalResultINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE6failedEv",
     "core::LogicalResult@core<core::UniquePtr@core<core::DemangledSymbol@"
     "core>>::failed() const"},
    {"_ZNK4coreW4core13RequireSymbol10isConstFunEv",
     "core::RequireSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core13RequireSymbol11hasTemplateEv",
     "core::RequireSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core13RequireSymbol5cloneEv",
     "core::RequireSymbol@core::clone() const"},
    {"_ZNK4coreW4core13RequireSymbol8toStringEv",
     "core::RequireSymbol@core::toString() const"},
    {"_ZNK4coreW4core13VirtualSymbol10isConstFunEv",
     "core::VirtualSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core13VirtualSymbol11hasTemplateEv",
     "core::VirtualSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core13VirtualSymbol12typeToStringEv",
     "core::VirtualSymbol@core::typeToString() const"},
    {"_ZNK4coreW4core13VirtualSymbol5cloneEv",
     "core::VirtualSymbol@core::clone() const"},
    {"_ZNK4coreW4core13VirtualSymbol8toStringEb",
     "core::VirtualSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core14TemplateSymbol10isConstFunEv",
     "core::TemplateSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core14TemplateSymbol11hasTemplateEv",
     "core::TemplateSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core14TemplateSymbol5cloneEv",
     "core::TemplateSymbol@core::clone() const"},
    {"_ZNK4coreW4core14TemplateSymbol8toStringEb",
     "core::TemplateSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core14VolatileSymbol10isConstFunEv",
     "core::VolatileSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core14VolatileSymbol11getLastNameEv",
     "core::VolatileSymbol@core::getLastName() const"},
    {"_ZNK4coreW4core14VolatileSymbol11hasTemplateEv",
     "core::VolatileSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core14VolatileSymbol13isConstructorEv",
     "core::VolatileSymbol@core::isConstructor() const"},
    {"_ZNK4coreW4core14VolatileSymbol5cloneEv",
     "core::VolatileSymbol@core::clone() const"},
    {"_ZNK4coreW4core14VolatileSymbol8toStringEb",
     "core::VolatileSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core15DemangledSymbol10isConstFunEv",
     "core::DemangledSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core15DemangledSymbol10isVoidTypeEv",
     "core::DemangledSymbol@core::isVoidType() const"},
    {"_ZNK4coreW4core15DemangledSymbol11getLastNameEv",
     "core::DemangledSymbol@core::getLastName() const"},
    {"_ZNK4coreW4core15DemangledSymbol11hasTemplateEv",
     "core::DemangledSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core15DemangledSymbol13isConstructorEv",
     "core::DemangledSymbol@core::isConstructor() const"},
    {"_ZNK4coreW4core15DemangledSymbol5cloneEv",
     "core::DemangledSymbol@core::clone() const"},
    {"_ZNK4coreW4core15DemangledSymbol8toStringEb",
     "core::DemangledSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core16ModuleBaseSymbol11hasTemplateEv",
     "core::ModuleBaseSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core16ModuleBaseSymbol5cloneEv",
     "core::ModuleBaseSymbol@core::clone() const"},
    {"_ZNK4coreW4core16ModuleBaseSymbol8toStringEb",
     "core::ModuleBaseSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core3MapImNS_S0_6StringEE3endEv",
     "core::Map@core<unsigned long, core::String@core>::end() const"},
    {"_ZNK4coreW4core3MapImNS_S0_6StringEE5beginEv",
     "core::Map@core<unsigned long, core::String@core>::begin() const"},
    {"_ZNK4coreW4core3MapImNS_S0_6StringEE8IteratordeEv",
     "core::Map@core<unsigned long, "
     "core::String@core>::Iterator::operator*() const"},
    {"_ZNK4coreW4core3MapImNS_S0_6StringEE8IteratoreqERKS4_",
     "core::Map@core<unsigned long, "
     "core::String@core>::Iterator::operator==(core::Map@core<unsigned "
     "long, "
     "core::String@core>::Iterator const&) const"},
    {"_ZNK4coreW4core3MapImNS_S0_6StringEE8IteratorneERKS4_",
     "core::Map@core<unsigned long, "
     "core::String@core>::Iterator::operator!=(core::Map@core<unsigned "
     "long, "
     "core::String@core>::Iterator const&) const"},
    {"_ZNK4coreW4core3MapINS_S0_6StringES2_E3endEv",
     "core::Map@core<core::String@core, core::String@core>::end() const"},
    {"_ZNK4coreW4core3MapINS_S0_6StringES2_E5beginEv",
     "core::Map@core<core::String@core, core::String@core>::begin() const"},
    {"_ZNK4coreW4core3MapINS_S0_6StringES2_E7getSizeEv",
     "core::Map@core<core::String@core, core::String@core>::getSize() "
     "const"},
    {"_ZNK4coreW4core3MapINS_S0_6StringES2_E8containsES2_",
     "core::Map@core<core::String@core, "
     "core::String@core>::contains(core::String@core) const"},
    {"_ZNK4coreW4core3MapINS_S0_6StringES2_E8IteratordeEv",
     "core::Map@core<core::String@core, "
     "core::String@core>::Iterator::operator*() const"},
    {"_ZNK4coreW4core3MapINS_S0_6StringES2_E8IteratoreqERKS4_",
     "core::Map@core<core::String@core, "
     "core::String@core>::Iterator::operator==(core::Map@core<core::String@"
     "core, core::String@core>::Iterator const&) const"},
    {"_ZNK4coreW4core3MapINS_S0_6StringES2_E8IteratorneERKS4_",
     "core::Map@core<core::String@core, "
     "core::String@core>::Iterator::operator!=(core::Map@core<core::String@"
     "core, core::String@core>::Iterator const&) const"},
    {"_ZNK4coreW4core6String3ptrEv", "core::String@core::ptr() const"},
    {"_ZNK4coreW4core6String6lengthEv", "core::String@core::length() const"},
    {"_ZNK4coreW4core6StringeqERKS1_",
     "core::String@core::operator==(core::String@core const&) const"},
    {"_ZNK4coreW4core6StringplERKS1_",
     "core::String@core::operator+(core::String@core const&) const"},
    {"_ZNK4coreW4core6Symbol10isConstFunEv",
     "core::Symbol@core::isConstFun() const"},
    {"_ZNK4coreW4core6Symbol11getLastNameEv",
     "core::Symbol@core::getLastName() const"},
    {"_ZNK4coreW4core6Symbol11hasTemplateEv",
     "core::Symbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core6Symbol13isConstructorEv",
     "core::Symbol@core::isConstructor() const"},
    {"_ZNK4coreW4core6Symbol5cloneEv", "core::Symbol@core::clone() const"},
    {"_ZNK4coreW4core6Symbol8toStringEb",
     "core::Symbol@core::toString(bool) const"},
    {"_ZNK4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE15ReverseIteratordeEv",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::ReverseIterator::operator*() const"},
    {"_ZNK4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE15ReverseIteratoreqERKS6_",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::ReverseIterator::operator==(core::Vector@core<core::UniquePtr@"
     "core<"
     "core::DemangledSymbol@core>>::ReverseIterator const&) const"},
    {"_ZNK4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE15ReverseIteratorneERKS6_",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::ReverseIterator::operator!=(core::Vector@core<core::UniquePtr@"
     "core<"
     "core::DemangledSymbol@core>>::ReverseIterator const&) const"},
    {"_ZNK4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE3endEv",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::end() const"},
    {"_ZNK4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE4rendEv",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::rend() const"},
    {"_ZNK4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE5beginEv",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::begin() const"},
    {"_ZNK4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE6lengthEv",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::length() const"},
    {"_ZNK4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE6rbeginEv",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::rbegin() const"},
    {"_ZNK4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE8IteratordeEv",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::Iterator::operator*() const"},
    {"_ZNK4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE8IteratoreqERKS6_",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::Iterator::operator==(core::Vector@core<core::UniquePtr@core<core::"
     "DemangledSymbol@core>>::Iterator const&) const"},
    {"_ZNK4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEE8IteratorneERKS6_",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::Iterator::operator!=(core::Vector@core<core::UniquePtr@core<core::"
     "DemangledSymbol@core>>::Iterator const&) const"},
    {"_ZNK4coreW4core6VectorINS_S0_9UniquePtrINS_S0_"
     "15DemangledSymbolEEEEixEm",
     "core::Vector@core<core::UniquePtr@core<core::DemangledSymbol@core>"
     ">::operator[](unsigned long) const"},
    {"_ZNK4coreW4core9PtrSymbol10isConstFunEv",
     "core::PtrSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core9PtrSymbol11hasTemplateEv",
     "core::PtrSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core9PtrSymbol5cloneEv",
     "core::PtrSymbol@core::clone() const"},
    {"_ZNK4coreW4core9PtrSymbol8toStringEb",
     "core::PtrSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core9StdSymbol10isConstFunEv",
     "core::StdSymbol@core::isConstFun() const"},
    {"_ZNK4coreW4core9StdSymbol11hasTemplateEv",
     "core::StdSymbol@core::hasTemplate() const"},
    {"_ZNK4coreW4core9StdSymbol5cloneEv",
     "core::StdSymbol@core::clone() const"},
    {"_ZNK4coreW4core9StdSymbol8toStringEb",
     "core::StdSymbol@core::toString(bool) const"},
    {"_ZNK4coreW4core9UniquePtrINS_S0_15DemangledSymbolEE6getRawEv",
     "core::UniquePtr@core<core::DemangledSymbol@core>::getRaw() const"},
    {"_ZNK4coreW4core9UniquePtrINS_S0_15DemangledSymbolEEeqEDn",
     "core::UniquePtr@core<core::DemangledSymbol@core>::operator==("
     "decltype("
     "nullptr)) const"},
    {"_ZNK4coreW4core9UniquePtrINS_S0_15DemangledSymbolEEneEDn",
     "core::UniquePtr@core<core::DemangledSymbol@core>::operator!=("
     "decltype("
     "nullptr)) const"},
    {"_ZNK4coreW4core9UniquePtrINS_S0_15DemangledSymbolEEptEv",
     "core::UniquePtr@core<core::DemangledSymbol@core>::operator->() "
     "const"},
    {"_ZNStW4core16initializer_listIN4coreS_3MapINS1_S_6StringES3_"
     "E5EntryEE3endEv",
     "std::initializer_list@core<core::Map@core<core::String@core, "
     "core::String@core>::Entry>::end()"},
    {"_ZNStW4core16initializer_listIN4coreS_3MapINS1_S_6StringES3_"
     "E5EntryEE5beginEv",
     "std::initializer_list@core<core::Map@core<core::String@core, "
     "core::String@core>::Entry>::begin()"},
    {"_Znwm", "operator new(unsigned long)"},
    {"_ZTVN4coreW4core10BaseSymbolE", "vtable for core::BaseSymbol@core"},
    {"_ZTVN4coreW4core10LvalSymbolE", "vtable for core::LvalSymbol@core"},
    {"_ZTVN4coreW4core10RvalSymbolE", "vtable for core::RvalSymbol@core"},
    {"_ZTVN4coreW4core11ChainSymbolE", "vtable for core::ChainSymbol@core"},
    {"_ZTVN4coreW4core11ConstSymbolE", "vtable for core::ConstSymbol@core"},
    {"_ZTVN4coreW4core12LambdaSymbolE", "vtable for core::LambdaSymbol@core"},
    {"_ZTVN4coreW4core12ModuleSymbolE", "vtable for core::ModuleSymbol@core"},
    {"_ZTVN4coreW4core13BuiltinSymbolE",
     "vtable for core::BuiltinSymbol@core"},
    {"_ZTVN4coreW4core13FunTypeSymbolE",
     "vtable for core::FunTypeSymbol@core"},
    {"_ZTVN4coreW4core13RequireSymbolE",
     "vtable for core::RequireSymbol@core"},
    {"_ZTVN4coreW4core13VirtualSymbolE",
     "vtable for core::VirtualSymbol@core"},
    {"_ZTVN4coreW4core14TemplateSymbolE",
     "vtable for core::TemplateSymbol@core"},
    {"_ZTVN4coreW4core14VolatileSymbolE",
     "vtable for core::VolatileSymbol@core"},
    {"_ZTVN4coreW4core15DemangledSymbolE",
     "vtable for core::DemangledSymbol@core"},
    {"_ZTVN4coreW4core16DestructorSymbolE",
     "vtable for core::DestructorSymbol@core"},
    {"_ZTVN4coreW4core16ModuleBaseSymbolE",
     "vtable for core::ModuleBaseSymbol@core"},
    {"_ZTVN4coreW4core17ConstructorSymbolE",
     "vtable for core::ConstructorSymbol@core"},
    {"_ZTVN4coreW4core6SymbolE", "vtable for core::Symbol@core"},
    {"_ZTVN4coreW4core9PtrSymbolE", "vtable for core::PtrSymbol@core"},
    {"_ZTVN4coreW4core9StdSymbolE", "vtable for core::StdSymbol@core"},
    {"_ZW4coreli2_xy", "operator\"\" _x@core(unsigned long long)"},
    {"_ZZN4coreW4core22parseSubstitutionIndexERmRPKcENKUlcE_clEc",
     "core::parseSubstitutionIndex@core(unsigned long&, char "
     "const*&)::{lambda(char)#1}::operator()(char) const"},
    {"_ZZN4coreW4core9AllocatorILm32076800ELm1024EE3getEvE9allocator",
     "core::Allocator@core<32076800ul, 1024ul>::get()::allocator"}};
fail = 0;

for (auto &[mangled, golden] : symbols) {
  auto demangled = core::demangle(mangled);
  if (demangled.failed()) {
    core::serr << demangled.getMessage() << "\n";
    ++fail;
  } else if (*demangled != golden) {
    core::serr << "Demangling failed for " << mangled << " got " << *demangled
               << " but was expecting " << golden << ".\n\n";
    ++fail;
  }
}
if (fail != 0) {
  core::serr << "Wrong demangling result for " << fail << " functions.\n";
  return core::TestResult::fail;
}
return core::TestResult::success;
}*/