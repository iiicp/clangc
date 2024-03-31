//===--- LangOptions.h - C Language Family Language Options -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file defines the LangOptions interface.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LANGOPTIONS_H
#define LLVM_CLANG_LANGOPTIONS_H
#include <string>
namespace clang {

  /// LangOptions - This class keeps track of the various options that can be
  /// enabled, which controls the dialect of C that is accepted.
  class LangOptions {
public:
    bool Trigraphs;      // Trigraphs in source files.
    bool BCPLComment;    // BCPL-style '//' comments.
    bool Bool;           // 'bool', 'true', 'false' keywords.
    bool DollarIdents;   // '$' allowed in identifiers.
    bool AsmPreprocessor;// Preprocessor in asm mode.
    bool GNUMode;        // True in gnu99 mode false in c99 mode (etc)
    bool ImplicitInt;    // C89 implicit 'int'.
    bool Digraphs;       // C94, C99 and C++
    bool HexFloats;      // C99 Hexadecimal float constants.
    bool C99;            // C99 Support
    bool NoBuiltin;      // Do not use builtin functions (-fno-builtin)
                              // by locks.
    bool EmitAllDecls; // Emit all declarations, even if
                              // they are unused.

    bool Optimize;    // Whether __OPTIMIZE__ should be defined.
    bool OptimizeSize;// Whether __OPTIMIZE_SIZE__ should be defined.
    bool NoInline; // Should __NO_INLINE__ be defined.
public:
    enum VisibilityMode {
      Default,
      Protected,
      Hidden
    };
private:
  VisibilityMode SymbolVisibility = Default;// Symbol's visibility.

    /// The user provided name for the "main file", if non-null. This is
    /// useful in situations where the input file name does not match
    /// the original input file, for example with -save-temps.
    std::string MainFileName;

public:
    LangOptions() {
      Trigraphs = BCPLComment = Bool = DollarIdents = AsmPreprocessor = false;
      GNUMode = ImplicitInt = Digraphs = false;
      HexFloats = false;
      C99 = false;
      NoBuiltin = false;
      SymbolVisibility = Default;

      EmitAllDecls = false;

      Optimize = false;
      OptimizeSize = false;

      NoInline = 0;

      MainFileName = "";
    }

    const std::string &getMainFileName() const { return MainFileName; }
    void setMainFileName(const char *Name) { MainFileName = Name; }

    VisibilityMode getVisibilityMode() const {
      return (VisibilityMode) SymbolVisibility;
    }
    void setVisibilityMode(VisibilityMode v) { SymbolVisibility =  v; }
  };

}// namespace clang

#endif
