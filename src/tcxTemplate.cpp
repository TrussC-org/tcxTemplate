#include "tcxTemplate.h"

using namespace std;
using namespace tc;

namespace tcx {

void Template::setup() {
    // One-time initialization goes here (load resources, allocate buffers...).
    logNotice() << "tcxTemplate ready";
}

void Template::update() {
    // Pulse the radius using elapsed time. Angles are in radians; TAU is a
    // full turn, so this completes one pulse cycle per second.
    float t = getElapsedTimef();
    currentRadius_ = baseRadius_ * (1.0f + 0.25f * sinf(t * TAU));
}

void Template::draw() {
    // tc::setColor (not the member setColor) — qualify so the drawing call
    // doesn't resolve to this class's chainable setter of the same name.
    tc::setColor(color_);
    drawCircle(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f, currentRadius_);
}

} // namespace tcx
