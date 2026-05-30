#include "tcxTemplate.h"

using namespace std;
using namespace tc;

namespace tcx {

void Template::hello() {
    logNotice() << "Hello, " << name_ << "!";
}

} // namespace tcx
