#include <QGuiApplication>
#include <QQuickView>
#include <commandlineparser.h>
#include "windoweventfilter.h"

//## 03 - CLASSES
//
// Our Functions refinement brought some improvements,
//  but notably did not fix one issue:
// CWindowEventFilter still has multiple class members that are needed for different jobs.
//   While we extracted those jobs into separate functions,
//   the whole CWindowEventFilter still does all these jobs, violating
//   the "single responsibility principle".
// Let's try to improve that by separating things into different classes
//
// Again, no changes in main(), so let's head over to
//  windoweventfilter.h/cpp...

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    CCommandLineParser cmdLineParser(app);
    CWindowEventFilter eventFilter;
    eventFilter.setEditAllowed(cmdLineParser.isEditAllowed());
    eventFilter.setLassoAllowed(cmdLineParser.isLassoAllowed());

    QQuickView view;
    eventFilter.setView(&view);
    view.loadFromModule("mehQml","Main");
    view.show();
    return app.exec();
}
