#pragma once

// =============================================================================
// tcxTemplate - A minimal TrussC addon template.
//
// Rename everything ("tcxTemplate" / "Template" / the sub-namespace "myaddon")
// to your addon name, then replace this stub with your feature. The class does
// nothing useful on purpose — it just logs a greeting — so it doesn't bias your
// addon toward any particular shape (per-frame drawing, networking, pure logic,
// ...). The house style and conventions live in README.md.
// =============================================================================

#include <TrussC.h>

// Each addon gets its OWN sub-namespace under `tcx`: `tcx::<addonname>`, where
// <addonname> is the addon name minus the `tcx` prefix, ALL lowercase — no
// abbreviation, no camelCase (tcxOsc -> tcx::osc, tcxDepthCamera ->
// tcx::depthcamera, NOT tcx::depthCamera or tcx::dc). This isolates every
// addon's types, so two addons can each define a `World` / `Client` / `Message`
// without clashing, and all-lowercase keeps the spelling mechanically derivable
// and visually distinct from PascalCase types. Users opt in per addon with
// `using namespace tcx::<addonname>;`. (Framework core is `tc`.)
//
// Header hygiene: NO `using namespace` in headers — not at file scope, not
// inside your namespace; it silently injects names into every consumer.
// Qualify explicitly (`tc::Vec2`, `std::string`). File-scope using-directives
// in a .cpp are fine. Internal helpers: nested `detail` namespace (header) or
// anonymous namespace (.cpp).
//
// Migrating an existing addon that shipped in flat `tcx::` or legacy `trussc::`?
// Move the definitions here, then add temporary silent compat aliases
// (`namespace tcx { using myaddon::Foo; }` and/or `namespace trussc { ... }`) —
// see tcxOsc. Those compat layers are removed at v1.0.0.
namespace tcx::myaddon {

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

} // namespace tcx::myaddon
