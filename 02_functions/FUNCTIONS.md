# Functions
We still have all the code in one file, but separated into functions: One function per topic.
## Improvements
### Order or priority of features is now much more obvious
We call functions for different features in turn. This reveals the order or priority of the features (which was there before, but buried unter lines and lines of code).
### CWindowEventFilter::eventFilter is much easier to read
The function is much shorter.
## Dependencies
![02_dependency_functions](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/b787ccc5-8ddc-4406-aea8-f8b7f2ee89d0)

Not much changed, but we see that each function only uses one or at most two grey functionality classes.
## Issues
### Class members for different concerns
Most class members in CWindowEventFilter are only used in one or two functions. This is a sign of low cohesion.
## Next
[Classes](../03_classes/CLASSES.md)
