#pragma once

// =============================================================================
// tcxTemplate - A minimal TrussC addon template.
//
// Rename everything ("tcxTemplate" / "Template") to your addon name, then
// replace this stub with your feature. The class does nothing useful on
// purpose — it just logs a greeting — so it doesn't bias your addon toward any
// particular shape (per-frame drawing, networking, pure logic, ...). The house
// style and conventions live in README.md.
// =============================================================================

#include <TrussC.h>

// Addons live in the `tcx` namespace (framework core uses `tc`). Users do
// `using namespace tcx;`, so keep public names short and unprefixed.
namespace tcx {

class Template {
public:
    Template() = default;

    // Setters return *this so calls can chain — a TrussC idiom:
    //   Template t; t.setName("foo").hello();
    Template& setName(const std::string& name) { name_ = name; return *this; }

    void hello();   // replace with your addon's real API

private:
    std::string name_ = "world";
};

} // namespace tcx
