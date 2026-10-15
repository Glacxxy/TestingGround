#include <pl/Mod.hpp>

namespace basescout {
class BaseScoutMod {
public:
    static BaseScoutMod& instance() {
        static BaseScoutMod mod;
        return mod;
    }

    BaseScoutMod() : self(*ll::mod::NativeMod::current()) {}

    bool load() {
        self.getLogger().info("BaseScout loaded. Diagnostic prototype only; player tracking is not active.");
        return true;
    }

    bool enable() {
        self.getLogger().info("BaseScout enabled. Waiting for a verified remote-player data source.");
        return true;
    }

    bool disable() {
        self.getLogger().info("BaseScout disabled.");
        return true;
    }

private:
    ll::mod::NativeMod& self;
};
}

PL_REGISTER_MOD(basescout::BaseScoutMod, basescout::BaseScoutMod::instance());
