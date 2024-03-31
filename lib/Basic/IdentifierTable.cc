#include "clang/Basic/IdentifierTable.h"
#include "clang/Basic/LangOptions.h"
#include "llvm/ADT/FoldingSet.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

//===----------------------------------------------------------------------===//
// IdentifierInfo Implementation
//===----------------------------------------------------------------------===//

IdentifierInfo::IdentifierInfo() {
  TokenID = tok::identifier;
  BuiltinID = 0;
  HasMacro = false;
  IsExtension = false;
  IsPoisoned = false;
  NeedsHandleIdentifier = false;
  FETokenInfo = 0;
  Entry = 0;
}

//===----------------------------------------------------------------------===//
// IdentifierTable Implementation
//===----------------------------------------------------------------------===//

IdentifierInfoLookup::~IdentifierInfoLookup() {}

ExternalIdentifierLookup::~ExternalIdentifierLookup() {}

IdentifierTable::IdentifierTable(const LangOptions &LangOpts,
                                 IdentifierInfoLookup *externalLookup)
    : HashTable(8192), // Start with space for 8K identifiers.
      ExternalLookup(externalLookup) {

  // Populate the identifier table with info about keywords for the current
  // language.
  AddKeywords(LangOpts);
}

//===----------------------------------------------------------------------===//
// Language Keyword Implementation
//===----------------------------------------------------------------------===//

// Constants for TokenKinds.def
namespace {
enum { KEYALL = 1, KEYC99 = 2, KEYGNU = 4, KEYMS = 8, BOOLSUPPORT = 16 };
}

/// AddKeyword - This method is used to associate a token ID with specific
/// identifiers because they are language keywords.  This causes the lexer to
/// automatically map matching identifiers to specialized token codes.
///
/// The C90/C99/CPP/CPP0x flags are set to 0 if the token should be
/// enabled in the specified langauge, set to 1 if it is an extension
/// in the specified language, and set to 2 if disabled in the
/// specified language.
static void AddKeyword(const char *Keyword, unsigned KWLen,
                       tok::TokenKind TokenCode, unsigned Flags,
                       const LangOptions &LangOpts, IdentifierTable &Table) {
  unsigned AddResult = 0;
  if (Flags & KEYALL)
    AddResult = 2;
  else if (LangOpts.C99 && (Flags & KEYC99))
    AddResult = 2;
  else if (LangOpts.GNUMode && (Flags & KEYGNU))
    AddResult = 1;
  else if (LangOpts.Bool && (Flags & BOOLSUPPORT))
    AddResult = 2;

  // Don't add this keyword if disabled in this language.
  if (AddResult == 0)
    return;

  IdentifierInfo &Info = Table.get(Keyword, Keyword + KWLen);
  Info.setTokenID(TokenCode);
  Info.setIsExtensionToken(AddResult == 1);
}

/// AddKeywords - Add all keywords to the symbol table.
///
void IdentifierTable::AddKeywords(const LangOptions &LangOpts) {
  // Add keywords and tokens for the current language.
#define KEYWORD(NAME, FLAGS)                                                   \
  AddKeyword(#NAME, strlen(#NAME), tok::kw_##NAME, FLAGS, LangOpts, *this);
#define ALIAS(NAME, TOK, FLAGS)                                                \
  AddKeyword(NAME, strlen(NAME), tok::kw_##TOK, FLAGS, LangOpts, *this);
#include "clang/Basic/TokenKinds.def"
}

tok::PPKeywordKind IdentifierInfo::getPPKeywordID() const {
  // We use a perfect hash function here involving the length of the keyword,
  // the first and third character.  For preprocessor ID's there are no
  // collisions (if there were, the switch below would complain about duplicate
  // case values).  Note that this depends on 'if' being null terminated.

#define HASH(LEN, FIRST, THIRD)                                                \
  (LEN << 5) + (((FIRST - 'a') + (THIRD - 'a')) & 31)
#define CASE(LEN, FIRST, THIRD, NAME)                                          \
  case HASH(LEN, FIRST, THIRD):                                                \
    return memcmp(Name, #NAME, LEN) ? tok::pp_not_keyword : tok::pp_##NAME

  unsigned Len = getLength();
  if (Len < 2)
    return tok::pp_not_keyword;
  const char *Name = getName();
  switch (HASH(Len, Name[0], Name[2])) {
  default:
    return tok::pp_not_keyword;
    CASE(2, 'i', '\0', if);
    CASE(4, 'e', 'i', elif);
    CASE(4, 'e', 's', else);
    CASE(4, 'l', 'n', line);
    CASE(4, 's', 'c', sccs);
    CASE(5, 'e', 'd', endif);
    CASE(5, 'e', 'r', error);
    CASE(5, 'i', 'e', ident);
    CASE(5, 'i', 'd', ifdef);
    CASE(5, 'u', 'd', undef);

    CASE(6, 'a', 's', assert);
    CASE(6, 'd', 'f', define);
    CASE(6, 'i', 'n', ifndef);
    CASE(6, 'i', 'p', import);
    CASE(6, 'p', 'a', pragma);

    CASE(7, 'd', 'f', defined);
    CASE(7, 'i', 'c', include);
    CASE(7, 'w', 'r', warning);

    CASE(8, 'u', 'a', unassert);
    CASE(12, 'i', 'c', include_next);

    CASE(16, '_', 'i', __include_macros);
#undef CASE
#undef HASH
  }
}

//===----------------------------------------------------------------------===//
// Stats Implementation
//===----------------------------------------------------------------------===//

/// PrintStats - Print statistics about how well the identifier table is doing
/// at hashing identifiers.
void IdentifierTable::PrintStats() const {
  unsigned NumBuckets = HashTable.getNumBuckets();
  unsigned NumIdentifiers = HashTable.getNumItems();
  unsigned NumEmptyBuckets = NumBuckets - NumIdentifiers;
  unsigned AverageIdentifierSize = 0;
  unsigned MaxIdentifierLength = 0;

  // TODO: Figure out maximum times an identifier had to probe for -stats.
  for (llvm::StringMap<IdentifierInfo *, llvm::BumpPtrAllocator>::const_iterator
           I = HashTable.begin(),
           E = HashTable.end();
       I != E; ++I) {
    unsigned IdLen = I->getKeyLength();
    AverageIdentifierSize += IdLen;
    if (MaxIdentifierLength < IdLen)
      MaxIdentifierLength = IdLen;
  }

  llvm::errs() << "\n*** Identifier Table Stats:\n";
  llvm::errs() << "# Identifiers:   " << NumIdentifiers << "\n";
  llvm::errs() << "# Empty Buckets: " << NumEmptyBuckets << "\n";
  llvm::errs() << "Hash density (#identifiers per bucket): "
               << NumIdentifiers / (double)NumBuckets << "\n";
  llvm::errs() << "Ave identifier length: "
               << (AverageIdentifierSize / (double)NumIdentifiers) << "\n";
  llvm::errs() << "Max identifier length: " << MaxIdentifierLength << "\n";

  // Compute statistics about the memory allocated for identifiers.
  HashTable.getAllocator().PrintStats();
}
