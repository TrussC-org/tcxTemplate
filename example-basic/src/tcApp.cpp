#include "tcApp.h"

void tcApp::setup() {
    tpl_.setColor(colors::cornflowerBlue).setRadius(100.0f);
    tpl_.setup();
}

void tcApp::update() {
    tpl_.update();
}

void tcApp::draw() {
    clear(0.1f, 0.1f, 0.12f);
    tpl_.draw();
}
