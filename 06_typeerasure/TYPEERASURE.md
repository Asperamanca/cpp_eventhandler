# Type Erasure
As far as the big structure is concerned, this is a "detail improvement". But we use quite a few coding tricks to make future extension easier and smoother. The additions are quite complex code - but the complexity is contained in a small area of code.
## Improvements
### We no longer need dummies for event functions we don't need
We can now precisely choose which types of event we want to handle, and which not.
### We can manage input rules as values, not pointers
This makes the code safer in general, and eliminates some nullptr checks
### Simplified the interface between CWindowEventHandler and CInputRuleManager
All events are now passed anonymously to CInputRuleManager, the distinction by event type (and therefore branching of code) is now done at the latest possible time, in CInputInfo
## Dependencies
We have renamed "input rules" to "event handlers", and cleaned up their dependencies on both inputrule.cpp and eventhandler.h. They now only depend on eventhandler.h only. Also, inputrulemanager.cpp no longer depends on eventhandler.h directly (only through inputrule.cpp, which now has a private companion file)
### Initialization
![06_dependency_typeerasure_init_result](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/b96a500b-1034-4a44-9e04-d3ff18a4c270)

### Runtime
![06_dependency_typeerasure_runtime_result](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/84cfce74-6557-4a0a-9cb4-6597e402000d)

The inputrule.cpp is no longer a base class, and because of the new design, we can now use it as a value (and are no longer forced to handle it via pointer). This simplifies managing the input rules a bit.
## Where to go from here
I honestly cannot think of a further refinement that would provide a significant improvement to the example. Of course, in the real world, requirements would eventually change, making further refinements necessary. But that's another story...
If you can think of another improvement, please provide feedback.
You may want to continue to the [wrap-up](../WRAPUP.md).
