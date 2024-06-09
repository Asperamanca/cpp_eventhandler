#include <QGuiApplication>
#include <QQuickView>
#include <commandlineparser.h>
#include "inputrulemanagerfactory.h"
#include "windoweventfilter.h"

//## 05 - MULTIINHERITANCE
//
//TODO: Create MD

// Our previous version was a huge improvement, and if you expect your codebase
//  to stay small-to-mid-sized, it may be a sweet spot of flexibility and simplicity.
// However, if you expect to get a large number of input rules and event types,
//  and maybe also more functionality within an input rule, it doesn't scale that well
//  (and also requires a lot of dummy implementations of virtual functions).
//
// We specifically identified two issues:
//  1) The "if/else if" ladded by event type in CWindowEventFilter
//      somehow does not look ideal
//  2) We can't mix and match which event handler function to implement,
//      we always have to implement them all
//
// Let's start with the first issue, and head over to windoweventfilter.cpp...

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    CCommandLineParser cmdLineParser(app);

    CWindowEventFilter eventFilter(CInputRuleManagerFactory::createInputRuleManager(cmdLineParser));

    QQuickView view;
    eventFilter.setView(&view);
    view.loadFromModule("mehQml","Main");
    view.show();
    return app.exec();
}
