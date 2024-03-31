/***********************************
 * File:     TokenKinds.h
 *
 * Author:   caipeng
 *
 * Email:    iiicp@outlook.com
 *
 * Date:     2022/11/13
 *
 * Sign:     enjoy life
 ***********************************/
#ifndef MINI_CLANG_TOKENKINDS_H
#define MINI_CLANG_TOKENKINDS_H
namespace clang {

  namespace tok {
    enum TokenKind {
#define TOK(X) X,
#include "clang/Basic/TokenKinds.def"
      NUM_TOKENS
    };

    /// PPKeywordKind - This provides a namespace for preprocessor keywords which
    /// start with a '#' at the beginning of the line.
    enum PPKeywordKind {
#define PPKEYWORD(X) pp_##X,
#include "clang/Basic/TokenKinds.def"
      NUM_PP_KEYWORDS
    };

    /// \brief Determines the name of a token as used within the front end.
    ///
    /// The name of a token will be an internal name (such as "l_square")
    /// and should not be used as part of diagnostic messages.
    const char *getTokenName(enum TokenKind Kind);

    /// \brief Determines the spelling of simple punctuation tokens like
    /// '!' or '%', and returns NULL for literal and annotation tokens.
    ///
    /// This routine only retrieves the "simple" spelling of the token,
    /// and will not produce any alternative spellings (e.g., a
    /// digraph). For the actual spelling of a given Token, use
    /// Preprocessor::getSpelling().
    const char *getTokenSimpleSpelling(enum TokenKind Kind);
  }// namespace tok
}// namespace clang

#endif// MINI_CLANG_TOKENKINDS_H
