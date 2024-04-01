// #include "clang/Basic/FileManager.h"
// #include "clang/Basic/LangOptions.h"
// #include "clang/Basic/SourceLocation.h"
// #include "clang/Basic/SourceManager.h"
#include "clang/Basic/Version.h"
// #include "llvm/IR/LLVMContext.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Process.h"
#include "llvm/Support/Signals.h"
// #include "llvm/Support/TargetSelect.h"
// #include "llvm/Support/Timer.h"

using namespace clang;

static const char *Head =
    "clang c compiler: https://github.com/iiicp/clangc";

void printVersion() {
  llvm::outs() << Head << " " << clang::getClangVersion() << "\n";
}

// //===----------------------------------------------------------------------===//
// // Global options.
// //===----------------------------------------------------------------------===//

// static bool HadErrors = false;

// static llvm::cl::opt<std::string>
//     OutputFile("o", llvm::cl::value_desc("path"),
//                llvm::cl::desc("Specify output file"));

// enum ProgActions {
//   EmitAssembly,           // Emit a .s file.
//   EmitLLVM,               // Emit a .ll file.
//   ASTPrint,               // Parse ASTs and print them
//   ASTDump,                // Parse ASTs and dump them
//   ParseSyntaxOnly,        // Parse and perform semantic analysis.
//   ParseNoop,              // Parse with noop callbacks.
//   RunPreprocessorOnly,    // Just lex, no output
//   PrintPreprocessedInput, // -E mode
//   DumpTokens,             // Dump out preprocessed tokens
//   DumpRawTokens           // Dump out raw tokens
// };

// static llvm::cl::opt<ProgActions> ProgAction(
//     llvm::cl::desc("Choose output type:"), llvm::cl::ZeroOrMore,
//     llvm::cl::init(ParseSyntaxOnly),
//     llvm::cl::values(
//         clEnumValN(RunPreprocessorOnly, "Eonly",
//                    "Just run preprocessor, no output (for timings)"),
//         clEnumValN(PrintPreprocessedInput, "E",
//                    "Run preprocessor, emit preprocessed file"),
//         clEnumValN(DumpRawTokens, "dump-raw-tokens",
//                    "Lex file in raw mode and dump raw tokens"),
//         clEnumValN(DumpTokens, "dump-tokens",
//                    "Run preprocessor, dump internal rep of tokens"),
//         clEnumValN(ParseNoop, "parse-noop",
//                    "Run Parser with noop callbacks (for timings)"),
//         clEnumValN(ParseSyntaxOnly, "fsyntax-only",
//                    "Run parser and perform semantic analysis"),
//         clEnumValN(ASTPrint, "ast-print",
//                    "Build ASTs and then pretty-print them"),
//         clEnumValN(ASTDump, "ast-dump", "Build ASTs and then debug dump
//         them"), clEnumValN(EmitAssembly, "S", "Emit native assembly code"),
//         clEnumValN(EmitLLVM, "emit-llvm",
//                    "Build ASTs then convert to LLVM, emit .ll file")));

// //===----------------------------------------------------------------------===//
// // Diagnostic Options
// //===----------------------------------------------------------------------===//

// static llvm::cl::opt<bool> VerifyDiagnostics(
//     "verify", llvm::cl::desc("Verify emitted diagnostics and warnings"));

// static llvm::cl::opt<bool>
//     NoShowColumn("fno-show-column",
//                  llvm::cl::desc("Do not include column number on
//                  diagnostics"));

// static llvm::cl::opt<bool> NoShowLocation(
//     "fno-show-source-location",
//     llvm::cl::desc("Do not include source location information with"
//                    " diagnostics"));

// static llvm::cl::opt<bool> NoCaretDiagnostics(
//     "fno-caret-diagnostics",
//     llvm::cl::desc("Do not include source line and caret with"
//                    " diagnostics"));

// static llvm::cl::opt<bool>
//     NoDiagnosticsFixIt("fno-diagnostics-fixit-info",
//                        llvm::cl::desc("Do not include fixit information in"
//                                       " diagnostics"));

// static llvm::cl::opt<bool> PrintSourceRangeInfo(
//     "fdiagnostics-print-source-range-info",
//     llvm::cl::desc("Print source range spans in numeric form"));

// static llvm::cl::opt<bool> PrintDiagnosticOption(
//     "fdiagnostics-show-option",
//     llvm::cl::desc("Print diagnostic name with mappable diagnostics"));

// static llvm::cl::opt<unsigned>
//     MessageLength("fmessage-length",
//                   llvm::cl::desc("Format message diagnostics so that they fit
//                   "
//                                  "within N columns or fewer, when
//                                  possible."),
//                   llvm::cl::value_desc("N"));

// static llvm::cl::opt<bool> NoColorDiagnostic(
//     "fno-color-diagnostics",
//     llvm::cl::desc("Don't use colors when showing diagnostics "
//                    "(automatically turned off if output is not a "
//                    "terminal)."));

// //===----------------------------------------------------------------------===//
// // Language Options
// //===----------------------------------------------------------------------===//

// enum LangKind {
//   langkind_unspecified,
//   langkind_c,
//   langkind_c_cpp,
//   langkind_asm_cpp
// };

// static llvm::cl::opt<LangKind> BaseLang(
//     "x", llvm::cl::desc("Base language to compile"),
//     llvm::cl::init(langkind_unspecified),
//     llvm::cl::values(clEnumValN(langkind_c, "c", "C"),
//                      clEnumValN(langkind_c_cpp, "cpp-output", "Preprocessed
//                      C"), clEnumValN(langkind_asm_cpp, "assembler-with-cpp",
//                                 "Preprocessed asm")));

// static llvm::cl::opt<LangOptions::VisibilityMode> SymbolVisibility(
//     "fvisibility", llvm::cl::desc("Set the default symbol visibility:"),
//     llvm::cl::init(LangOptions::Default),
//     llvm::cl::values(clEnumValN(LangOptions::Default, "default",
//                                 "Use default symbol visibility"),
//                      clEnumValN(LangOptions::Hidden, "hidden",
//                                 "Use hidden symbol visibility"),
//                      clEnumValN(LangOptions::Protected, "protected",
//                                 "Use protected symbol visibility")));

// /// LangStds - Language standards we support.
// enum LangStds {
//   lang_unspecified,
//   lang_c89,
//   lang_c99,
//   lang_c11,
//   lang_gnu89,
//   lang_gnu99,
// };

// static llvm::cl::opt<LangStds> LangStd(
//     "std", llvm::cl::desc("Language standard to compile for"),
//     llvm::cl::init(lang_unspecified),
//     llvm::cl::values(
//         clEnumValN(lang_c89, "c89", "ISO C 1990"),
//         clEnumValN(lang_c89, "c90", "ISO C 1990"),
//         clEnumValN(lang_c99, "c99", "ISO C 1999"),
//         clEnumValN(lang_c11, "c11", "ISO C 2011"),
//         clEnumValN(lang_gnu89, "gnu89", "ISO C 1990 with GNU extensions"),
//         clEnumValN(lang_gnu99, "gnu99",
//                    "ISO C 1999 with GNU extensions (default for C)")));
// static llvm::cl::opt<bool>
//     Trigraphs("trigraphs", llvm::cl::desc("Process trigraph sequences"));

// static llvm::cl::opt<bool>
//     DollarsInIdents("fdollars-in-identifiers",
//                     llvm::cl::desc("Allow '$' in identifiers"));

// static llvm::cl::opt<std::string>
//     MainFileName("main-file-name",
//                  llvm::cl::desc("Main file name to use for debug info"));

// static llvm::cl::opt<bool> OptSize("Os", llvm::cl::desc("Optimize for
// size"));

// // It might be nice to add bounds to the CommandLine library directly.
// // struct OptLevelParser : public llvm::cl::parser<unsigned> {
// //   bool parse(llvm::cl::Option &O, llvm::StringRef ArgName, llvm::StringRef
// //   Arg,
// //              unsigned &Val) {
// //     if (llvm::cl::parser<unsigned>::parse(O, ArgName, Arg, Val))
// //       return true;
// //     if (Val > 3)
// //       return O.error("'" + Arg + "' invalid optimization level!");
// //     return false;
// //   }
// // };

// // static llvm::cl::opt<unsigned, false, OptLevelParser>
// //     OptLevel("O", llvm::cl::Prefix, llvm::cl::desc("Optimization level"),
// //              llvm::cl::init(0u));

// static llvm::cl::opt<bool>
//     EmitAllDecls("femit-all-decls",
//                  llvm::cl::desc("Emit all declarations, even if unused"));

// static LangKind GetLanguage(const std::string &Filename) {
//   if (BaseLang != langkind_unspecified)
//     return BaseLang;

//   std::string::size_type DotPos = Filename.rfind('.');

//   if (DotPos == std::string::npos) {
//     BaseLang = langkind_c; // Default to C if no extension.
//     return langkind_c;
//   }

//   std::string Ext = std::string(Filename.begin() + DotPos + 1,
//   Filename.end());
//   // C header: .h
//   // C++ header: .hh or .H;
//   // assembler no preprocessing: .s
//   // assembler: .S
//   if (Ext == "c")
//     return langkind_c;
//   else if (Ext == "S" ||
//            // If the compiler is run on a .s file, preprocess it as .S
//            Ext == "s")
//     return langkind_asm_cpp;
//   else if (Ext == "i")
//     return langkind_c_cpp;
//   else
//     return langkind_c;
// }

// static void InitializeLangOptions(LangOptions &Options, LangKind LK) {
//   switch (LK) {
//   default:
//     assert(0 && "Unknown language kind!");
//   case langkind_asm_cpp:
//     Options.AsmPreprocessor = true;
//     // FALLTHROUGH
//     LLVM_FALLTHROUGH;
//   case langkind_c_cpp:
//     // FALLTHROUGH
//     LLVM_FALLTHROUGH;
//   case langkind_c:
//     break;
//   }

//   Options.setVisibilityMode(SymbolVisibility);
// }

// //===----------------------------------------------------------------------===//
// // Target Triple Processing.
// //===----------------------------------------------------------------------===//

// static llvm::cl::opt<std::string> TargetTriple(
//     "triple",
//     llvm::cl::desc("Specify target triple (e.g. i686-apple-darwin9)"));

// /// CreateTargetTriple - Process the various options that affect the target
// /// triple and build a final aggregate triple that we are compiling for.
// // static std::string CreateTargetTriple() {
// //   // Initialize base triple.  If a -triple option has been specified, use
// //   // that triple.  Otherwise, default to the host triple.
// //   std::string Triple = TargetTriple;
// //   if (Triple.empty())
// //     Triple = llvm::sys::getDefaultTargetTriple();

// //   return Triple;
// // }

// //===----------------------------------------------------------------------===//
// // Preprocessor Initialization
// //===----------------------------------------------------------------------===//

// // FIXME: Preprocessor builtins to support.
// //   -A...    - Play with #assertions
// //   -undef   - Undefine all predefined macros

// static llvm::cl::list<std::string>
//     D_macros("D", llvm::cl::value_desc("macro"), llvm::cl::Prefix,
//              llvm::cl::desc("Predefine the specified macro"));
// static llvm::cl::list<std::string>
//     U_macros("U", llvm::cl::value_desc("macro"), llvm::cl::Prefix,
//              llvm::cl::desc("Undefine the specified macro"));
// static llvm::cl::list<std::string>
//     ImplicitIncludes("include", llvm::cl::value_desc("file"),
//                      llvm::cl::desc("Include file before parsing"));
// static llvm::cl::opt<bool>
//     nostdinc("nostdinc",
//              llvm::cl::desc("Disable standard #include directories"));
// static llvm::cl::list<std::string>
//     I_dirs("I", llvm::cl::value_desc("directory"), llvm::cl::Prefix,
//            llvm::cl::desc("Add directory to include search path"));
// static llvm::cl::list<std::string>
//     iquote_dirs("iquote", llvm::cl::value_desc("directory"),
//     llvm::cl::Prefix,
//                 llvm::cl::desc("Add directory to QUOTE include search
//                 path"));
// static llvm::cl::list<std::string>
//     isystem_dirs("isystem", llvm::cl::value_desc("directory"),
//     llvm::cl::Prefix,
//                  llvm::cl::desc("Add directory to SYSTEM include search
//                  path"));
// static llvm::cl::opt<std::string>
//     isysroot("isysroot", llvm::cl::value_desc("dir"), llvm::cl::init("/"),
//              llvm::cl::desc("Set the system root directory (usually /)"));
// //===----------------------------------------------------------------------===//
// // Builtin Options
// //===----------------------------------------------------------------------===//
// static llvm::cl::opt<bool> AllowBuiltins(
//     "fbuiltin", llvm::cl::init(true),
//     llvm::cl::desc("Disable implicit builtin knowledge of functions"));

// //===----------------------------------------------------------------------===//
// // Code generation options
// //===----------------------------------------------------------------------===//

// static llvm::cl::opt<bool> GenerateDebugInfo(
//     "g", llvm::cl::desc("Generate source level debug information"));

// static llvm::cl::opt<std::string> TargetCPU(
//     "mcpu",
//     llvm::cl::desc("Target a specific cpu type (-mcpu=help for details)"));

// static llvm::cl::list<std::string>
//     TargetFeatures("target-feature",
//                    llvm::cl::desc("Target specific attributes"),
//                    llvm::cl::value_desc("+a1,+a2,-a3,..."));

// //===----------------------------------------------------------------------===//
// // Preprocessing (-E mode) Options
// //===----------------------------------------------------------------------===//
// static llvm::cl::opt<bool>
//     DisableLineMarkers("P",
//                        llvm::cl::desc("Disable linemarker output in -E
//                        mode"));
// static llvm::cl::opt<bool>
//     EnableCommentOutput("C",
//                         llvm::cl::desc("Enable comment output in -E mode"));
// static llvm::cl::opt<bool>
//     EnableMacroCommentOutput("CC",
//                              llvm::cl::desc("Enable comment output in -E
//                              mode, "
//                                             "even from macro expansions"));
// static llvm::cl::opt<bool>
//     DumpMacros("dM",
//                llvm::cl::desc("Print macro definitions in -E mode instead of"
//                               " normal output"));
// static llvm::cl::opt<bool>
//     DumpDefines("dD", llvm::cl::desc("Print macro definitions in -E mode in "
//                                      "addition to normal output"));

// //===----------------------------------------------------------------------===//
// // Main driver
// //===----------------------------------------------------------------------===//

// static void LLVMErrorHandler(void *UserData, const char *Message,
//                              bool GenCrashDiag) {

//   llvm::errs() << Message << "\n";

//   // Run the interrupt handlers to make sure any special cleanups get done,
//   in
//   // particular that we remove files registered with RemoveFileOnSignal.
//   llvm::sys::RunInterruptHandlers();

//   // We cannot recover from llvm errors.  When reporting a fatal error, exit
//   // with status 70 to generate crash diagnostics.  For BSD systems this is
//   // defined as an internal software error.  Otherwise, exit with status 1.
//   llvm::sys::Process::Exit(GenCrashDiag ? 70 : 1);
// }

// static llvm::cl::list<std::string>
//     InputFilenames(llvm::cl::Positional, llvm::cl::desc("<input files>"));

int main(int argc, char **argv) {

  llvm::sys::PrintStackTraceOnErrorSignal();
  llvm::PrettyStackTraceProgram X(argc, argv);

  llvm::cl::SetVersionPrinter(printVersion);
  llvm::cl::ParseCommandLineOptions(argc, argv, Head);

  return 0;

  //   llvm::InitializeAllTargets();
  //   llvm::InitializeAllTargetMCs();
  //   llvm::InitializeAllAsmPrinters();
  //   llvm::InitializeAllAsmParsers();

  //   llvm::LLVMContext context;

  //   // If -fmessage-length=N was not specified, determine whether this
  //   // is a terminal and, if so, implicitly define -fmessage-length
  //   // appropriately.
  //   if (MessageLength.getNumOccurrences() == 0)
  //     MessageLength.setValue(llvm::sys::Process::StandardErrColumns());

  //   if (!NoColorDiagnostic) {
  //     NoColorDiagnostic.setValue(!llvm::sys::Process::StandardErrHasColors());
  //   }

  //   // If no input was specified, read from stdin.
  //   if (InputFilenames.empty())
  //     InputFilenames.push_back("-");

  // /// todo diag
  // #if LCC_DIAG
  //   // Create the diagnostic client for reporting errors or for
  //   // implementing -verify.
  //   llvm::OwningPtr<DiagnosticClient> DiagClient;
  //   if (VerifyDiagnostics) {
  //     // When checking diagnostics, just buffer them up.
  //     DiagClient.reset(new TextDiagnosticBuffer());
  //     if (InputFilenames.size() != 1) {
  //       llvm::errs() << "-verify only works on single input files for
  //       now.\n"; return 1;
  //     }
  //   } else {

  //     DiagClient.reset(new TextDiagnosticPrinter(
  //         llvm::errs(), !NoShowColumn, !NoCaretDiagnostics, !NoShowLocation,
  //         PrintSourceRangeInfo, PrintDiagnosticOption, !NoDiagnosticsFixIt,
  //         MessageLength, !NoColorDiagnostic));
  //   }

  //   // Configure our handling of diagnostics.
  //   Diagnostic Diags(DiagClient.get());
  // #endif
  //   // Set an error handler, so that any LLVM backend diagnostics go through
  //   our
  //   // error handler.
  //   llvm::install_fatal_error_handler(LLVMErrorHandler, nullptr);

  // /// todo target
  // #if LCC_TARGET
  //   // Get information about the target being compiled for.
  //   std::string Triple = CreateTargetTriple();
  //   llvm::OwningPtr<TargetInfo> Target(TargetInfo::CreateTargetInfo(Triple));

  //   if (Target == 0) {
  //     Diags.Report(FullSourceLoc(), diag::err_fe_unknown_triple)
  //         << Triple.c_str();
  //     return 1;
  //   }
  // #endif

  // #if LCC_FEATURE_MAP
  //   // Compute the feature set, unfortunately this effects the language!
  //   llvm::StringMap<bool> Features;
  //   ComputeFeatureMap(Target.get(), Features);
  // #endif

  //   std::unique_ptr<SourceManager> SourceMgr;

  //   // Create a file manager object to provide access to and cache the
  //   filesystem. FileManager FileMgr;

  //   for (unsigned i = 0, e = InputFilenames.size(); i != e; ++i) {
  //     const std::string &InFile = InputFilenames[i];

  //     /// Create a SourceManager object.  This tracks and owns all the file
  //     /// buffers allocated to a translation unit.
  //     if (!SourceMgr)
  //       SourceMgr.reset(new SourceManager());
  //     else
  //       SourceMgr->clearIDTables();

  //     // Initialize language options, inferring file types from input
  //     filenames. LangOptions LangInfo;

  //     LangKind LK = GetLanguage(InFile);
  //     InitializeLangOptions(LangInfo, LK);
  // #if 0 /// need targetinfo
  //         InitializeLanguageStandard(LangInfo, LK, Target.get(), Features);
  // #endif

  //     // Process the -I options and set them in the HeaderInfo.
  //     // HeaderSearch HeaderInfo(FileMgr);

  //     // InitializeIncludePaths(argv[0], HeaderInfo, FileMgr, LangInfo);

  //     // Set up the preprocessor with these options.
  //     // DriverPreprocessorFactory PPFactory(Diags, LangInfo, *Target,
  //     //                                     *SourceMgr.get(), HeaderInfo);

  //     // llvm::OwningPtr<Preprocessor> PP(PPFactory.CreatePreprocessor());

  //     // if (!PP)
  //     //     continue;

  //     // if (InitializeSourceManager(*PP.get(), InFile))
  //     //     continue;

  //     // // Initialize builtin info.
  //     // PP->getBuiltinInfo().InitializeBuiltins(PP->getIdentifierTable(),
  //     // PP->getLangOptions().NoBuiltin);

  //     // Process the source file.
  //     // ProcessInputFile(*PP, PPFactory, InFile, ProgAction, Features,
  //     Context);

  //     // HeaderInfo.ClearFileInfo();
  //     // DiagClient->setLangOptions(0);
  //   }

  //   // Managed static deconstruction. Useful for making things like
  //   // -time-passes usable.
  //   llvm::llvm_shutdown();

  //   return HadErrors /*|| (Diags.getNumErrors() != 0)*/;
}