//===--- TokenConcatenation.h - Token Concatenation Avoidance ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the TokenConcatenation class.
//
//===----------------------------------------------------------------------===//

#ifndef CLANG_LEX_TOKEN_CONCATENATION_H
#define CLANG_LEX_TOKEN_CONCATENATION_H

#include "clang/Basic/TokenKinds.h"

namespace clang {
  class Preprocessor;
  class Token;

  /// TokenConcatenation class, which answers the question of
  ///   "Is it safe to emit two tokens without a whitespace between them, or
  ///    would that cause implicit concatenation of the tokens?"
  ///
  /// For example, it emitting two identifiers "foo" and "bar" next to each
  /// other would cause the lexer to produce one "foobar" token.  Emitting "1"
  /// and ")" next to each other is safe.
  ///
  class TokenConcatenation {
    Preprocessor &PP;

    enum AvoidConcatInfo {
      /// By default, a token never needs to avoid concatenation.  Most tokens
      /// (e.g. ',', ')', etc) don't cause a problem when concatenated.
      aci_never_avoid_concat = 0,

      /// aci_custom_firstchar - AvoidConcat contains custom code to handle this
      /// token's requirements, and it needs to know the first character of the
      /// token.
      aci_custom_firstchar = 1,

      /// aci_custom - AvoidConcat contains custom code to handle this token's
      /// requirements, but it doesn't need to know the first character of the
      /// token.
      aci_custom = 2,

      /// aci_avoid_equal - Many tokens cannot be safely followed by an '='
      /// character.  For example, "<<" turns into "<<=" when followed by an =.
      aci_avoid_equal = 4
    };

    /// TokenInfo - This array contains information for each token on what
    /// action to take when avoiding concatenation of tokens in the AvoidConcat
    /// method.
    char TokenInfo[tok::NUM_TOKENS];

public:
    TokenConcatenation(Preprocessor &PP);

    bool AvoidConcat(const Token &PrevTok, const Token &Tok) const;

private:
    /// StartsWithL - Return true if the spelling of this token starts with 'L'.
    bool StartsWithL(const Token &Tok) const;

    /// IsIdentifierL - Return true if the spelling of this token is literally
    /// 'L'.
    bool IsIdentifierL(const Token &Tok) const;
  };
}// namespace clang

#endif
