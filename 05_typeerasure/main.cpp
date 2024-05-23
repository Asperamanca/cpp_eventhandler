#include <QGuiApplication>
#include <QQuickView>
#include <commandlineparser.h>
#include "windoweventfilter.h"
#include "inputrulemanagerfactory.h"

//## 05 - TYPE ERASURE
//
// Our previous version was a huge improvement, and if you expect your codebase
//  to stay small-to-mid-sized, it may be a sweet spot of flexibility and simplicity.
// However, if you expect to get a large number of input rules and event types,
//  and maybe also more functionality within an input rule, it doesn't scale that well
//  (and also requires a lot of dummy implementations of virtual functions).
//
// We specifically identified three issues:
//  1) The "if/else if" ladded by event type in CWindowEventFilter
//      somehow does not look ideal
//  2) We can't mix and match which event handler function to implement,
//      we always have to implement them all
//  3) CAbstractInputRule could become a heavyweight, if we plan to add more
//      shared functionality to the input rules
// Let's see what we can do about them.
//
// main.cpp is essentially unchanged, but we did a lot of structural changes elsewhere:
// Before, each type of input rule (e.g. CDragMoveInputRule) inherited from IInputRule.
//  We only avoided code duplication for enable/disable through a shared base class (CAbstractInputRule).
// Let's try a different approach: We create one interface for event handling only,
//  and implement the enable/disable stuff in a class that is not part of an inheritance hierarchy.
// We further split things up by creating separate interface classes for separate events.
//  This will allow us to mix and match event function as we need.
//
// Let's head over to eventhandler.h and take a look...
//  (and return to the end of this file for a wrap-up)

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
//  best to use the simplest approach possible. Here, I would probably go for
//  "Functions" or maybe even "The big switch".
// If you are at the other end of the spectrum, and expect your code to live long
//  and grow bigger and bigger, you need something that can reduce added complexity.
//  Here, the final "Type erasure" soltion is likely better.
// If you are somewhere in-between, you may choose the "Inheritance" solution.
//  This is what I mean when I write "it depends...".
//
//## FURTHER MATERIAL
// -) What are regular types, and why should I care? - "Regular, Revisited" by Victor Ciura
// -) Why should I care about value semantics? - "Value Semantics: Safety, Independence,
//      Projection & Future of Programming" by Dave Abrahams
// -) Advice on type-safe programming: "Typically C++, But why?" by Björn Fahller
// -) Just generally recommended: "Coping with other people's C++ code" by Laura Savino


