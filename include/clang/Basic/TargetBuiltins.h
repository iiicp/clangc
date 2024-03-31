#pragma once
#include "clang/Basic/Builtins.h"

namespace clang {
/// X86 builtins
namespace X86 {
enum {
  LastTIBuiltin = clang::Builtin::FirstTSBuiltin - 1,
#define BUILTIN(ID, TYPE, ATTRS) BI##ID,
#include "clang/Basic/BuiltinsX86.def"
  LastTSBuiltin
};
} // namespace X86
} // namespace lcc