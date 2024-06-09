#include <QGuiApplication>
#include <QQuickView>
#include <commandlineparser.h>
#include "inputrulemanagerfactory.h"
#include "windoweventfilter.h"

//## 04 - INHERITANCE
//
// Our separation into classes brought some improvements again,
//  but we saw a few more issues:
// 1) Passing multiple parameters of the same type to the event handler
//      functions is dangerous
// 2) While the code is no longer in CWindowEventFilter, that class still
//      knows about every feature implemented, and uses and ugly "if else"
//      ladder to call them in order
// 3) Also, the input rules to use are hardwired in CWindowEventFilter,
//      we cannot change them easily e.g. for testing.
// The first point is actually pretty easy to fix: We create a simple type
//  that contains all the information we want to pass to the event handler
//  functions (see inputinfo.h/cpp)
// This has the added benefit that extending the parameter list later
//  becomes a trivial task, and does not require changing function signatures.
//  (In some situation there might be a counter-argument that keeping parameters
//  separate forces you to touch and re-test all interfaces that change, making
//  you aware of all the places you influence by the change of interface.
//  I don't follow that logic, at least not for a larger code base: If you change
//  existing members of such a "parameter pack type" later on, you will be greeted
//  by compiler errors as existing functions using that type no longer understand it
//  (as you should).
//  And if you choose to extend the type, it makes sense only to also extend the
//  functions that will benefit from the new member. This also nicely follows the advice
//  from the so-called  "Open Closed Principle": Symbols should be "open for extension",
//  but "closed to change". In short, extensions that will not adversely affect existing
//  code is welcome, but changes in meaning or behavior are not: They carry a large risk
//  of introducing new bugs.
//
// The second point requires a bigger refinement-
//  When we look closely, we realize that CWindowEventFilter still had
//  more than one job:
//  a) Add context information to events so an event handler can actually do
//      something useful with them
//  b) Know which event handlers exist, and call them in the correct order
// Let's separate those concerns: CWindowEventFilter keeps adding the context to events,
//  but should no longer know about the event handlers or their call order (priority).
//  We'll create a new class to handle that, see inputrulemanager.h/cpp
// However, the issue of the ugly "else if" ladder remains. To solve that,
//  let's create a simple base class for event handlers (see inputrule.h)
//  The new CInputRuleManager only needs to know about the base class
//  (IInputRule), and does not need to worry about the concrete implementation
//  That will allow us to treat the event handlers (which I now call "input rules")
//  in a generic fashion.
//
// This time, there are some changes in main(), so please read on here...

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    CCommandLineParser cmdLineParser(app);

    // CWindowEventFilter now needs the input rule manager to pass on events
    // The rule manager is created using a factory method that in turn needs the
    // command line parser to configure the input rules correctly
    //
    // Take a look a inputrulemanagerfactory.h/cpp to see how the input manager is created...
    CWindowEventFilter eventFilter(CInputRuleManagerFactory::createInputRuleManager(cmdLineParser));
    // ..and then let's read on in windoweventfilter.h/cpp

    QQuickView view;
    eventFilter.setView(&view);
    view.loadFromModule("mehQml","Main");
    view.show();
    return app.exec();
}
