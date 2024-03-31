#include <gtest/gtest.h>

#include "clang/Basic/FileManager.h"
#include "llvm/Support/raw_ostream.h"
using namespace clang;
TEST(FMTest, test_get_file) {
  FileSystemOptions options;
  clang::FileManager fm(options);
  const char *testFile = "../CMakeLists.txt";
  const char *testDir = "..";
  const clang::FileEntry *entry = fm.getFile(testFile);
  ASSERT_STREQ(entry->getName(), testFile);
  ASSERT_STREQ(entry->getDir()->getName(), testDir);

  const clang::FileEntry *entry2 = fm.getFile(testFile);
  ASSERT_STREQ(entry2->getName(), testFile);
  ASSERT_STREQ(entry2->getDir()->getName(), testDir);

  fm.PrintStats();
}