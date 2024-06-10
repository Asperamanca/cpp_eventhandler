#include <QGuiApplication>
#include <QQuickView>
#include <commandlineparser.h>
#include "inputrulemanagerfactory.h"
#include "windoweventfilter.h"

//## 06 - TYPE ERASURE
//
// TODO: Update all comments and MD
//
// The multiple inheritance gave us flexibility to mix-and-match
//  whatever event types we want to implement. But we have again identified
//  some issues to look into:
// 1) CInputRule looks like it shouldn't be a base class, and in any case
//      allows users of that class to cause object slicing.
// 2) The loop over input rule inside CInputRuleManager repeats for each event type
// 3) We also need to dynamic_cast our input rule types a lot
//
// Let's start in inputrulemanager.h to see the changes
//
// At the end, you can return to the end of this file for a wrap-up.

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

//## WRAP-UP
//
// You have now seen a code refinement from single function with a big switch
//  to separate functions, separate classes, abstraction via base class and
//  finally a flexible approach using type erasure.
// The order suggests that the first implementation is the worst, and the last
//  implementation is the best. But it's not that simple. Because, as so often,
//  the answer is "it depends..."
// In every refinement, we actually increased the total amount of code. In most
//  refinedments, we added new files. This is a source of complexity that should
//  be taken into account.
// So when I say "it depends..." you fairly ask "...on what?"
//  Let me try to answer in this way:
// In each refinement, the code (as it is) has become more complex IN TOTAL
//  At the same time, we have often reduced complexity LOCALLY.
//  And also, our changes will reduce the ADDITIONAL complexity required to add
//  new event handlers and event handler types.
// For a very simple example that's not expected to change anymore, it's probably
//  best to use the simplest approach possible. Here, I would probably go for "Functions"
//  as it (at least) cleanly separates the different event handlers.
// If you are at the other end of the spectrum, and expect your code to live long
//  and grow bigger and bigger, you need something that can reduce added complexity.
//  Here, the final "Type erasure" or "Multiple inheritance" soltions are likely better.
// If you are somewhere in-between, you may choose the "Inheritance" solution.
//  This is what I mean when I write "it depends...".
//
//## FURTHER MATERIAL
// -) What are regular types, and why should I care? - "Regular, Revisited" by Victor Ciura
// -) Why should I care about value semantics? - "Value Semantics: Safety, Independence,
//      Projection & Future of Programming" by Dave Abrahams
// -) Advice on type-safe programming: "Typically C++, But why?" by Björn Fahller
// -) Just generally recommended: "Coping with other people's C++ code" by Laura Savino


