#include <QGuiApplication>
#include <QQuickView>
#include <commandlineparser.h>
#include "windoweventfilter.h"

// REFACTORING AN EVENT HANDLER - FROM BIG SWITCH TO FLEXIBLE TYPE-ERASED INTERFACES
//## INTRODUCTION
// MOTIVATION
// Over the years, I have written and then refactored a lot of code. More than
//  once, I have stumbled trying to improve a piece of code, because I could not
//  find the right abstraction and the right approach (or I simply didn't know it,
//  then).
// There are a lot of books about code design and design patterns, and they have
//  been helpful. They often start with guiding principles (such as SOLID - see
//  "Clean C++" by Stephan Roth, as well as the updated "Clean C++ 20" if you want
//  to know more). Then they either describe how to achieve each principle
//  separately, using examples. Or they go through software design patterns,
//  explain how they work, when to apply them, etc. (see "C++ Software Design" by
//  Klaus Iglberger for an excellent book of this kind).
// All of that is very helpful, and helped me become a better programmer. But when
//  you write real code, you rarely just use one design principle, or just use one
//  pattern. You mix and match, combine, taking the best of each to form just the
//  abstraction that fits the problem.
// This code sample is an attempt to fill that gap: I start with a very simple,
//  almost naive implementation (still, something I have seen in production code -
//  and probably also written myself in the past). In each step, I point out
//  software design issues, and solve them in the next refinement. At the end of
//  the last refinement, I discuss tradeoffs, because the last refinement is not
//  necessarily the best, it depends on various factors.

// WHAT THE CODE DOES
// The code I refactor and refine is the central GUI event handler for a simple
//  application (that is, the code that responds, in our limited example, to
//  arriving mouse and keyboard events). The application itself does the same in
//  all refinements: It shows a window with a few colored rectangles. The user can
//  do three things:
// 1) "Click selection": Select and deselect items using Click
//      (single-selection), Control-Click (toggle selection, which also allows
//      multi-selection) and Control-A (select all)
// 2) "Lasso selection": Select items using a lasso (click into the background,
//      then drag to form a lasso). This operation can be canceled using the
//      "Escape" key.
// 3) "Drag-Move": Move single or multiple items by click-dragging them.
// Both Lasso and Drag-Move can be turned on and off. In this example, I do this
//  via command line arguments "--edit" (allows drag-move) and "--lasso" (allows
//  lasso). In a real-world use case, assue we could turn some features on or off
//  at runtime (e.g. depending on some mode the user can switch via UI).

// REFINEMENT STEPS
// 1) "The big switch": This is where we start. Everything is in a single function in a big switch
// 2) "Functions": Everything is still in a single file, but at least separated into functions
// 3) "Classes": Things get split up into multiple files, but I don't yet use
//      any abstractions to avoid code duplication
// 4) "Inheritance": I use classic inheritance to make some central code more generic and less repetitive
// 5) "Type erasure": I add flexibility for future extension of the code using
//      a combination of type erasure, multiple inheritance and C++20 concepts

// HOW TO READ THE CODE
// When reading the code, there are two parts:
//  a) Code inside the "common" folder as well as files with extension ".qml" only exist
//      to make the program work. They are not part of the refactoring process, and I wrote then
//      as simple as possible to get the job done. I also would not be surprised if you found
//      a few bugs in that part of the program, and the implementation is certainly not optimal.
//  b) The code to look at closely is inside the folders starting with two digits ("01_...")
//      This is where I want to show different approaches solving the same problem,
//      and what tradeoffs they bring.

// HOW TO RUN THE CODE
// To build the examples, you need a C++20-enabled compiler and Qt 6.7.0 or newer.
//  Nevertheless, this is NOT an example about Qt, it's just the framework I happen
//  to use. (Still, I make a few remarks about certain usages of Qt types, but
//  that's side notes and not essential).
// When you run the code, notice that you HAVE to add command line switches
//  "--edit --lasso" to see all features in action!

// A WORD ON QT
// (I hope) you don't need to understand QML or Qt Quick in order to follow my code.
//  But here is a little primer to make it easier for you:
//  QML is a declarative language in a way similar to HTML.
//  You can basically state: "There is a rectangle at position x/y which has size w/h and color 'red'"
//  The objects created in such a way always (logically) form a tree:
// -) There is a single root object attached to the view (=the windows)
// -) This root object may have children, and they in turn may have children.
//      If I start at the root object, I can recursively walk the whole tree to find every visual object.
//
// CAVEATS
// -) For readability, I don't use namespaces. In a real project, this could easily cause naming conflicts
// -) I am not a cmake expert. Things work, but please don't take my cmake files as an example on
//      "how to do it right"

// A word on DOCUMENTATION of these examples:
// -) Always start reading in main.cpp.
//      The text there will then tell you where to continue reading.
// -) You can always find comments that guide you through the example by searching for "//##"
// -) Comments without any special prefix explain what I am doing, and why
// -) NAMING: In a few places, I explain my scheme of naming things, in order to make the code easier to read
// -) ISSUE: Such comments mark an issue that we want to address in a following refinement of the code
// -) IMPROVEMENT: In such comments, I mention thins we improved in the latest code refinements
// -) QT: Notes about things related to the Qt framework

// So let's start with the most basic implementation
//## 01 - BIG SWITCH
// Before we come to the switch itself, let's take a look at the main function:

int main(int argc, char *argv[])
{
    // QGuiApplication helps me parse the command line,
    //  and also runs the event loop (see end of main function)
    QGuiApplication app(argc, argv);

    CCommandLineParser cmdLineParser(app);
    // CWindowEventFilter is the class that will receive (and respond to) all events
    CWindowEventFilter eventFilter;
    // Enable / disable some features using the command line
    eventFilter.setEditAllowed(cmdLineParser.isEditAllowed());
    eventFilter.setLassoAllowed(cmdLineParser.isLassoAllowed());

    // Prepare a window
    QQuickView view;
    // Tell eventFilter from which view to capture events
    eventFilter.setView(&view);
    // Load QML file for visualization
    view.loadFromModule("mehQml","Main");
    // Show the window
    view.show();

    // Run the event loop, so we can receive events
    return app.exec();
}

// Continue reading in "windoweventfilter.h" and cpp...

