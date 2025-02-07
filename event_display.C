#include "Display/MyMainFrame.h"
#include "Display/EventDisplay.h"

int main(int argc, const char** inputName) {
    auto* frame = new MyMainFrame(inputName[1]);
    return(0);
}
