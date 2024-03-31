/***********************************
 * File:     Version.cc
 *
 * Author:   caipeng
 *
 * Email:    iiicp@outlook.com
 *
 * Date:     2023/2/4
 *
 * Sign:     enjoy life
 ***********************************/
#include "clang/Basic/Version.h"
std::string clang::getClangVersion() {
  return CLANG_VERSION_STRING;
}

