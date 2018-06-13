#ifndef RUBY_TYPER_REAL_MAIN_H
#define RUBY_TYPER_REAL_MAIN_H
#include "ast/ast.h"
#include "common/KeyValueStore.h"
#include "common/ProgressIndicator.h"
#include "common/Timer.h"
#include "common/WorkerPool.h"
#include "common/common.h"
#include "core/StrictLevel.h"
#include "spdlog/spdlog.h"
#include <cxxopts.hpp>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace sorbet {
namespace realmain {
const auto PROGRESS_REFRESH_TIME_MILLIS = ProgressIndicator::REPORTING_INTERVAL();
int realmain(int argc, const char *argv[]);

// Terminate execution of sorbet with specific return code
class EarlyReturnWithCode : public SRubyException {
public:
    EarlyReturnWithCode(int returnCode);
    const int returnCode;
};

extern std::shared_ptr<spdlog::logger> logger;

struct Printers {
    bool ParseTree = false;
    bool ParseTreeJSON = false;
    bool Desugared = false;
    bool DesugaredRaw = false;
    bool DSLTree = false;
    bool DSLTreeRaw = false;
    bool NameTree = false;
    bool NameTreeRaw = false;
    bool NameTable = false;
    bool NameTableJson = false;
    bool NameTableFull = false;
    bool CFG = false;
    bool CFGRaw = false;
    bool TypedSource = false;
};

enum Phase {
    INIT,
    PARSER,
    DESUGARER,
    DSL,
    NAMER,
    CFG,
    INFERENCER,
};

struct Options {
    Printers print;
    Phase stopAfterPhase = Phase::INFERENCER;
    bool noStdlib = false;

    // Should we monitor STDOUT for HUP and exit if it hangs up. This is a
    // workaround for https://bugzilla.mindrot.org/show_bug.cgi?id=2863
    bool stdoutHUPHack = false;

    core::StrictLevel forceMinStrict = core::StrictLevel::Stripe;
    core::StrictLevel forceMaxStrict = core::StrictLevel::Strong;

    bool showProgress = false;
    bool suggestTyped = false;
    bool silenceErrors = false;
    bool supressNonCriticalErrors = false;
    int threads = 0;
    int logLevel = 0; // number of time -v was passed
    std::string typedSource = "";
    std::string cacheDir = "";
    std::vector<std::string> configatronDirs;
    std::vector<std::string> configatronFiles;
    std::string storeState = "";
    bool enableCounters = false;
    std::vector<std::string> someCounters;

    u4 reserveMemKiB;

    std::string statsdHost;
    std::string statsdPrefix;
    int statsdPort;

    std::string metricsFile;
    std::string metricsRepo;
    std::string metricsPrefix;
    std::string metricsBranch;
    std::string metricsSha;

    std::vector<std::string> inputFileNames;
    std::string inlineInput; // passed via -e
    std::string debugLogFile;

    Options() = default;
    Options(const Options &) = delete;
    Options(Options &&) = default;
    const Options &operator=(const Options &) = delete;
    const Options &operator=(Options &&) = delete;
};

void readOptions(Options &, int argc, const char *argv[]) throw(EarlyReturnWithCode);
std::unique_ptr<ast::Expression> indexOne(const Options &opts, core::GlobalState &lgs, core::FileRef file,
                                          std::unique_ptr<KeyValueStore> &kvstore,
                                          const std::shared_ptr<core::GlobalState> &pgs);
std::vector<std::unique_ptr<ast::Expression>> index(std::shared_ptr<core::GlobalState> &gs,
                                                    std::vector<std::string> frs,
                                                    std::vector<core::FileRef> mainThreadFiles, const Options &opts,
                                                    WorkerPool &workers, std::unique_ptr<KeyValueStore> &kvstore);
std::vector<std::unique_ptr<ast::Expression>>
resolve(core::GlobalState &gs, std::vector<std::unique_ptr<ast::Expression>> what, const Options &opts);
void typecheck(std::shared_ptr<core::GlobalState> &gs, std::vector<std::unique_ptr<ast::Expression>> what,
               const Options &opts, WorkerPool &workers);
std::unique_ptr<ast::Expression> typecheckOne(core::Context ctx, std::unique_ptr<ast::Expression> resolved,
                                              const Options &opts);
} // namespace realmain
} // namespace sorbet
#endif // RUBY_TYPER_REAL_MAIN_H
