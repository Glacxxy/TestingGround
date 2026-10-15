#include <pl/Mod.hpp>

#include <dlfcn.h>

#include <array>
#include <utility>

namespace basescout {
namespace {
struct SymbolProbe {
    const char* label;
    const char* mangled;
};

constexpr std::array<SymbolProbe, 5> kProbeSymbols{{
    {"LocalPlayer::normalTick", "_ZN11LocalPlayer10normalTickEv"},
    {"RemotePlayer::normalTick", "_ZN12RemotePlayer10normalTickEv"},
    {"Player::normalTick", "_ZN5Player10normalTickEv"},
    {"Actor::getPos", "_ZNK5Actor6getPosEv"},
    {"Level::getAllPlayers", "_ZN5Level13getAllPlayersEv"},
}};

void* minecraftHandle() {
#ifdef RTLD_NOLOAD
    static void* handle = dlopen("libminecraftpe.so", RTLD_NOW | RTLD_NOLOAD);
    return handle;
#else
    return nullptr;
#endif
}

void* findSymbol(const char* name) {
    if (void* symbol = dlsym(RTLD_DEFAULT, name)) {
        return symbol;
    }
    if (void* handle = minecraftHandle()) {
        return dlsym(handle, name);
    }
    return nullptr;
}
} // namespace

class BaseScoutMod {
public:
    static BaseScoutMod& instance() {
        static BaseScoutMod mod;
        return mod;
    }

    BaseScoutMod() : self(*ll::mod::NativeMod::current()) {}

    bool load() {
        self.getLogger().info("BaseScout loaded. Running non-invasive Bedrock symbol diagnostics.");
        for (const auto& probe : kProbeSymbols) {
            const bool found = findSymbol(probe.mangled) != nullptr;
            self.getLogger().info("BaseScout symbol probe: {} = {}", probe.label,
                                  found ? "found" : "not exported");
        }
        self.getLogger().warn("No player tracking hook is installed yet; symbol results are diagnostic only.");
        return true;
    }

    bool enable() {
        self.getLogger().info("BaseScout enabled. No hooks installed; waiting for validated game-version signatures.");
        return true;
    }

    bool disable() {
        self.getLogger().info("BaseScout disabled.");
        return true;
    }

private:
    ll::mod::NativeMod& self;
};
} // namespace basescout

PL_REGISTER_MOD(basescout::BaseScoutMod, basescout::BaseScoutMod::instance());
