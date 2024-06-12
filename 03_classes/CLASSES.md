# Classes
The principle is the same as in "Functions", but now we use one file per topic, not just one function
## Improvements
### Members of classes now serve the job of the class
Within each of the now separate classes, we only use the class members relevant to the topic at hand.
### CWindowEventFilter got simpler
CWindowEventFilter was in danger of becoming too big a class. Now it's simpler and smaller.
## Dependencies
![03_dependency_classes](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/6071e4cc-4abb-4af6-9637-3504c680d55a)

No structural change. If you count, event the number of arrows is identical. Basically just the color of the boxes has changed.

## Issues
### Input rules are hard-wired into CWindowEventFilter
CWindowEventFilter is tightly coupled to the input rules. It knows all their details, is even responsible for calling them in the correct order. Adding new rules will make the ugly if/else if ladder event uglier.
Also, this makes it hard to customize what input rules to use, e.g. for testing.
### Event handler functions get two parameters of the same type
This is dangerous, because we could easily mix them up.
## Next
[Inheritance](../04_inheritance/INHERITANCE.md)
