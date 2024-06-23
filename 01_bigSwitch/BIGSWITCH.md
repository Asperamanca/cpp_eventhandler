# Big Switch
We start with implementing everything in a big switch.
## Isn't that unrealistic?
No, I have seen plenty such code. When you start small, a switch is a pretty natural way to start. And I have seen plenty of code (mine included) where there never seemed to be time or will to refactor the growing switch into something more suitable.
## Dependency diagram
![01_dependency_bigswitch](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/fe5a87c6-007d-49e1-91e3-bf3711aab7db)

The dependencies are pretty simple: main.cpp uses windoweventfilter.cpp, and from there we use everything else. The grey boxes are code that's outside of the scope of our refactoring. But it's still code that's needed to implement the application's behavior, so we need to know who depends on it.
As you can see, a single file depends on basically everything.
## Issues
### Long function
CWindowEventFilter::eventFilter is already a long function, and we haven't even implemented much functionality!
### Mixing concerns
Inside CWindowEventFilter::eventFilter, we mix lines of code which handle different functionality: A few lines handle lasso selection, then a few lines handle drag-move,...
This makes it hard to read and understand a single feature. E.g. if you are looking for a bug in drag-move, you always have to skip the lines not relevant to it
## Next
[Functions](../02_functions/FUNCTIONS.md)
