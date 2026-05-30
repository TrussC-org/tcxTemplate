#pragma once

// =============================================================================
// tcxTemplate - A minimal TrussC addon template.
//
// Rename everything ("tcxTemplate" / "Template") to your addon name, then
// implement your feature. The class below just draws a pulsing circle so the
// example has something to show.
// =============================================================================

#include <TrussC.h>

// Addons live in the `tcx` namespace (framework core uses `tc`). Users do
// `using namespace tcx;`, so keep public names short and unprefixed.
namespace tcx {

class Template {
public:
    Template() = default;

    // Call once from your app's setup().
    void setup();

    // Call every frame from update() / draw().
    void update();
    void draw();

    // Setters return *this for method chaining:
    //   tpl.setColor(colors::cornflowerBlue).setRadius(80.0f);
    Template& setColor(const tc::Color& c) { color_ = c; return *this; }
    Template& setRadius(float r) { baseRadius_ = r; return *this; }

    float getRadius() const { return currentRadius_; }

private:
    tc::Color color_ = tc::Color(0.4f, 0.6f, 0.9f);
    float baseRadius_ = 60.0f;     // radius midpoint, in pixels
    float currentRadius_ = 60.0f;  // animated radius for this frame
};

} // namespace tcx
