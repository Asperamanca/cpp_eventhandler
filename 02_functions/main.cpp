#include <QGuiApplication>
#include <QQuickView>
#include <commandlineparser.h>
#include "windoweventfilter.h"

//## 02 - FUNCTIONS
//
//## In "The Big Switch", we identified two main issues:
// 1) The function with the switch was too big
// 2) The switch mixed code which has different purposes, making
//      it harder to follow and reason about a single feature
// Let's address these issues.
//
// The main function is identical, minus the comments,
//  so let's continue in file windoweventfilter.h/cpp

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
