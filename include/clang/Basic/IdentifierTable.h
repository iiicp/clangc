#ifndef LLVM_CLANG_BASIC_IDENTIFIERTABLE_H
#define LLVM_CLANG_BASIC_IDENTIFIERTABLE_H

#include "clang/Basic/TokenKinds.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/OwningPtr.h"
#include "llvm/Support/PointerLikeTypeTraits.h"
#include <cassert>
#include <string>

namespace clang {
class LangOptions;
class IdentifierInfo;
class IdentifierTable;
class SourceLocation;

/// IdentifierLocPair - A simple pair of identifier info and location.
typedef std::pair<IdentifierInfo *, SourceLocation> IdentifierLocPair;

/// IdentifierInfo - One of these records is kept for each identifier that
/// is lexed.  This contains information about whether the token was #define'd,
/// is a language keyword, or if it is a front-end token of some sort (e.g. a
/// variable or function name).  The preprocessor keeps this information in a
/// set, and all tok::identifier tokens have a pointer to one of these.
class IdentifierInfo {
  // Note: DON'T make TokenID a 'tok::TokenKind'; MSVC will treat it as a
  //       signed char and TokenKinds > 127 won't be handled correctly.
  tok::TokenKind TokenID;           // Front-end token ID or tok::identifier.
  // Objective-C keyword ('protocol' in '@protocol') or builtin (__builtin_inf).
  // First NUM_OBJC_KEYWORDS values are for Objective-C, the remaining values
  // are for builtins.
  unsigned BuiltinID;
  bool HasMacro;              // True if there is a #define for this.
  bool IsExtension;           // True if identifier is a lang extension.
  bool IsPoisoned;            // True if identifier is poisoned.
  bool NeedsHandleIdentifier; // See "RecomputeNeedsHandleIdentifier".
  // 9 bits left in 32-bit word.
  void *FETokenInfo; // Managed by the language front-end.
  llvm::StringMapEntry<IdentifierInfo *> *Entry;

  IdentifierInfo(const IdentifierInfo &); // NONCOPYABLE.
  void operator=(const IdentifierInfo &); // NONASSIGNABLE.

  friend class IdentifierTable;

public:
  IdentifierInfo();

  /// isStr - Return true if this is the identifier for the specified string.
  /// This is intended to be used for string literals only: II->isStr("foo").
  template <std::size_t StrLen> bool isStr(const char (&Str)[StrLen]) const {
    return getLength() == StrLen - 1 && !memcmp(getName(), Str, StrLen - 1);
  }

  /// getName - Return the actual string for this identifier.  The returned
  /// string is properly null terminated.
  ///
  const char *getName() const {
    if (Entry)
      return Entry->getKeyData();
    // FIXME: This is gross. It would be best not to embed specific details
    // of the PTH file format here.
    // The 'this' pointer really points to a
    // std::pair<IdentifierInfo, const char*>, where internal pointer
    // points to the external string data.
    return ((std::pair<IdentifierInfo, const char *> *)this)->second;
  }

  /// getLength - Efficiently return the length of this identifier info.
  ///
  unsigned getLength() const {
    if (Entry)
      return Entry->getKeyLength();
    // FIXME: This is gross. It would be best not to embed specific details
    // of the PTH file format here.
    // The 'this' pointer really points to a
    // std::pair<IdentifierInfo, const char*>, where internal pointer
    // points to the external string data.
    const char *p =
        ((std::pair<IdentifierInfo, const char *> *)this)->second - 2;
    return (((unsigned)p[0]) | (((unsigned)p[1]) << 8)) - 1;
  }

  /// hasMacroDefinition - Return true if this identifier is #defined to some
  /// other value.
  bool hasMacroDefinition() const { return HasMacro; }
  void setHasMacroDefinition(bool Val) {
    if (HasMacro == Val)
      return;

    HasMacro = Val;
    if (Val)
      NeedsHandleIdentifier = 1;
    else
      RecomputeNeedsHandleIdentifier();
  }

  /// get/setTokenID - If this is a source-language token (e.g. 'for'), this API
  /// can be used to cause the lexer to map identifiers to source-language
  /// tokens.
  tok::TokenKind getTokenID() const { return TokenID; }
  void setTokenID(tok::TokenKind ID) { TokenID = ID; }

  /// getPPKeywordID - Return the preprocessor keyword ID for this identifier.
  /// For example, "define" will return tok::pp_define.
  tok::PPKeywordKind getPPKeywordID() const;

  /// getBuiltinID - Return a value indicating whether this is a builtin
  /// function.  0 is not-built-in.  1 is builtin-for-some-nonprimary-target.
  /// 2+ are specific builtin functions.
  unsigned getBuiltinID() const {
      return BuiltinID;
  }

  void setBuiltinID(unsigned ID) {
    BuiltinID = ID;
  }

  /// get/setExtension - Initialize information about whether or not this
  /// language token is an extension.  This controls extension warnings, and is
  /// only valid if a custom token ID is set.
  bool isExtensionToken() const { return IsExtension; }
  void setIsExtensionToken(bool Val) {
    IsExtension = Val;
    if (Val)
      NeedsHandleIdentifier = 1;
    else
      RecomputeNeedsHandleIdentifier();
  }

  /// setIsPoisoned - Mark this identifier as poisoned.  After poisoning, the
  /// Preprocessor will emit an error every time this token is used.
  void setIsPoisoned(bool Value = true) {
    IsPoisoned = Value;
    if (Value)
      NeedsHandleIdentifier = true;
    else
      RecomputeNeedsHandleIdentifier();
  }

  /// isPoisoned - Return true if this token has been poisoned.
  bool isPoisoned() const { return IsPoisoned; }

  /// getFETokenInfo/setFETokenInfo - The language front-end is allowed to
  /// associate arbitrary metadata with this token.
  template <typename T> T *getFETokenInfo() const {
    return static_cast<T *>(FETokenInfo);
  }
  void setFETokenInfo(void *T) { FETokenInfo = T; }

  /// isHandleIdentifierCase - Return true if the Preprocessor::HandleIdentifier
  /// must be called on a token of this identifier.  If this returns false, we
  /// know that HandleIdentifier will not affect the token.
  bool isHandleIdentifierCase() const { return NeedsHandleIdentifier; }

private:
  /// RecomputeNeedsHandleIdentifier - The Preprocessor::HandleIdentifier does
  /// several special (but rare) things to identifiers of various sorts.  For
  /// example, it changes the "for" keyword token from tok::identifier to
  /// tok::for.
  ///
  /// This method is very tied to the definition of HandleIdentifier.  Any
  /// change to it should be reflected here.
  void RecomputeNeedsHandleIdentifier() {
    NeedsHandleIdentifier =
        (isPoisoned() || hasMacroDefinition() || isExtensionToken());
  }
};

/// IdentifierInfoLookup - An abstract class used by IdentifierTable that
///  provides an interface for performing lookups from strings
/// (const char *) to IdentiferInfo objects.
class IdentifierInfoLookup {
public:
  virtual ~IdentifierInfoLookup();

  /// get - Return the identifier token info for the specified named identifier.
  ///  Unlike the version in IdentifierTable, this returns a pointer instead
  ///  of a reference.  If the pointer is NULL then the IdentifierInfo cannot
  ///  be found.
  virtual IdentifierInfo *get(const char *NameStart, const char *NameEnd) = 0;
};

/// \brief An abstract class used to resolve numerical identifier
/// references (meaningful only to some external source) into
/// IdentifierInfo pointers.
class ExternalIdentifierLookup {
public:
  virtual ~ExternalIdentifierLookup();

  /// \brief Return the identifier associated with the given ID number.
  ///
  /// The ID 0 is associated with the NULL identifier.
  virtual IdentifierInfo *GetIdentifier(unsigned ID) = 0;
};

/// IdentifierTable - This table implements an efficient mapping from strings to
/// IdentifierInfo nodes.  It has no other purpose, but this is an
/// extremely performance-critical piece of the code, as each occurrance of
/// every identifier goes through here when lexed.
class IdentifierTable {
  // Shark shows that using MallocAllocator is *much* slower than using this
  // BumpPtrAllocator!
  typedef llvm::StringMap<IdentifierInfo *, llvm::BumpPtrAllocator> HashTableTy;
  HashTableTy HashTable;

  IdentifierInfoLookup *ExternalLookup;

public:
  /// IdentifierTable ctor - Create the identifier table, populating it with
  /// info about the language keywords for the language specified by LangOpts.
  IdentifierTable(const LangOptions &LangOpts,
                  IdentifierInfoLookup *externalLookup = 0);

  /// \brief Set the external identifier lookup mechanism.
  void setExternalIdentifierLookup(IdentifierInfoLookup *IILookup) {
    ExternalLookup = IILookup;
  }

  llvm::BumpPtrAllocator &getAllocator() { return HashTable.getAllocator(); }

  /// get - Return the identifier token info for the specified named identifier.
  ///
  IdentifierInfo &get(const char *NameStart, const char *NameEnd) {
    llvm::StringRef Name(NameStart, NameEnd - NameStart);
    llvm::StringMapEntry<IdentifierInfo*> &Entry =
        HashTable.GetOrCreateValue(Name);

    IdentifierInfo *II = Entry.getValue();
    if (II) return *II;

    // No entry; if we have an external lookup, look there first.
    if (ExternalLookup) {
      II = ExternalLookup->get(NameStart, NameEnd);
      if (II) {
        // Cache in the StringMap for subsequent lookups.
        Entry.setValue(II);
        return *II;
      }
    }

    // Lookups failed, make a new IdentifierInfo.
    void *Mem = getAllocator().Allocate<IdentifierInfo>();
    II = new (Mem) IdentifierInfo();
    Entry.setValue(II);

    // Make sure getName() knows how to find the IdentifierInfo
    // contents.
    II->Entry = &Entry;

    return *II;
  }

  /// \brief Creates a new IdentifierInfo from the given string.
  ///
  /// This is a lower-level version of get() that requires that this
  /// identifier not be known previously and that does not consult an
  /// external source for identifiers. In particular, external
  /// identifier sources can use this routine to build IdentifierInfo
  /// nodes and then introduce additional information about those
  /// identifiers.
  IdentifierInfo &CreateIdentifierInfo(const char *NameStart,
                                       const char *NameEnd) {
    llvm::StringRef Name(NameStart, NameEnd - NameStart);
    llvm::StringMapEntry<IdentifierInfo*> &Entry =
        HashTable.GetOrCreateValue(Name);

    IdentifierInfo *II = Entry.getValue();
    assert(!II && "IdentifierInfo already exists");

    // Lookups failed, make a new IdentifierInfo.
    void *Mem = getAllocator().Allocate<IdentifierInfo>();
    II = new (Mem) IdentifierInfo();
    Entry.setValue(II);

    // Make sure getName() knows how to find the IdentifierInfo
    // contents.
    II->Entry = &Entry;

    return *II;
  }

  IdentifierInfo &get(const char *Name) {
    return get(Name, Name + strlen(Name));
  }
  IdentifierInfo &get(const std::string &Name) {
    // Don't use c_str() here: no need to be null terminated.
    const char *NameBytes = Name.data();
    return get(NameBytes, NameBytes + Name.size());
  }

  typedef HashTableTy::const_iterator iterator;
  typedef HashTableTy::const_iterator const_iterator;

  iterator begin() const { return HashTable.begin(); }
  iterator end() const { return HashTable.end(); }
  unsigned size() const { return HashTable.size(); }

  /// PrintStats - Print some statistics to stderr that indicate how well the
  /// hashing is doing.
  void PrintStats() const;

  void AddKeywords(const LangOptions &LangOpts);
};
} // end namespace lcc

namespace llvm {

// Provide PointerLikeTypeTraits for IdentifierInfo pointers, which
// are not guaranteed to be 8-byte aligned.
template <> class PointerLikeTypeTraits<clang::IdentifierInfo *> {
public:
  static inline void *getAsVoidPointer(clang::IdentifierInfo *P) { return P; }
  static inline clang::IdentifierInfo *getFromVoidPointer(void *P) {
    return static_cast<clang::IdentifierInfo *>(P);
  }
  enum { NumLowBitsAvailable = 1 };
};

template <> class PointerLikeTypeTraits<const clang::IdentifierInfo *> {
public:
  static inline const void *getAsVoidPointer(const clang::IdentifierInfo *P) {
    return P;
  }
  static inline const clang::IdentifierInfo *getFromVoidPointer(const void *P) {
    return static_cast<const clang::IdentifierInfo *>(P);
  }
  enum { NumLowBitsAvailable = 1 };
};
} // end namespace llvm
#endif