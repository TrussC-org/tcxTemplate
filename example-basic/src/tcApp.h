#pragma once

#include <TrussC.h>
#include <tcxTemplate.h>

using namespace tc;
using namespace tcx;

class tcApp : public App {
public:
    void setup() override;
    void update() override;
    void draw() override;

private:
    Template tpl_;
};
