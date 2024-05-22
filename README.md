# cpp_eventhandler
Demonstrates how to successively refactor code from Big Switch via Classes to Type Erasure
# Motivation
Over the years, I have written and then refactored a lot of code. More than once, I have stumbled trying to improve a piece of code, because I could not find the right abstraction and the right approach (or I simply didn't know it, then).

There are a lot of books about code design and design patterns, and they have been helpful. They often start with guiding principles (such as SOLID - see "Clean C++" by Stephan Roth, as well as the updated "Clean C++ 20" if you want to know more). Then they either describe how to achieve each principle separately, using examples. Or they go through software design patterns, explain how they work, when to apply them, etc. (see "C++ Software Design" by Klaus Iglberger for an excellent book of this kind).

All of that is very helpful, and helped me become a better programmer. But when you write real code, you rarely just use one design principle, or just use one pattern. You mix and match, combine, taking the best of each to form just the abstraction that fits the problem.

This code sample is an attempt to fill that gap: I start with a very simple, almost naive implementation (still, something I have seen in production code - and probably also written myself in the past). In each step, I point out software design issues, and solve them in the next refinement. At the end of the last refinement, I discuss tradeoffs, because the last refinement is not necessarily
 the best, it depends on various factors.
# What the code does
The code I refactor and refine is the central GUI event handler for a simple application (that is, the code that responds, in our limited example, to arriving mouse and keyboard events). The application itself does the same in all refinements: It shows a window with a few colored rectangles. The user can do three things:
 1) "Click selection": Select and deselect items using Click (single-selection), Control-Click (toggle selection, which also allows multi-selection) and Control-A (select all)
 2) "Lasso selection": Select items using a lasso (click into the background, then drag to form a lasso). This operation can be canceled using the "Escape" key.
 3) "Drag-Move": Move single or multiple items by click-dragging them.

Both Lasso and Drag-Move can be turned on and off. In this example, I do this via command line arguments "--edit" (allows drag-move) and "--lasso" (allows lasso). In a real-world use case, assue we could turn some features on or off at runtime (e.g. depending on some mode the user can switch via UI).
# How to read the code
When reading the code, there are two parts:
1) Code inside the "common" folder as well as files with extension ".qml" only exist to make the program work. They are not part of the refactoring process, and I wrote then as simple as possible to get the job done. I also would not be surprised if you found a few bugs in that part of the program, and the implementation is certainly not optimal.  
2) The code to look at closely is inside the folders starting with two digits ("01_..."). This is where I want to show different approaches solving the same problem, and what tradeoffs they bring.
# How to run the code
To run the examples, you need Qt 6.7.0 or newer.
