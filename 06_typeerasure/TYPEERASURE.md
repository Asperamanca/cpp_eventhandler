# Type Erasure
TODO: Update
As far as the big structure is concerned, this is a "detail improvement". But we use quite a few coding tricks to make future extension easier and smoother. The additions are quite complex code - but the complexity is contained in a small area of code.
## Improvements
### We no longer need dummies for event functions we don't need
We can now precisely choose which types of event we want to handle, and which not.
### We can manage input rules as values, not pointers
This makes the code safer in general, and eliminates some nullptr checks
### Simplified the interface between CWindowEventHandler and CInputRuleManager
All events are now passed anonymously to CInputRuleManager, the distinction by event type (and therefore branching of code) is now done at the latest possible time, in CInputInfo
## Dependencies
Structurally, this is very similar to our previous refinement.
### Initialization
![06_dependency_typeerasure_init](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/9fa785c4-8c46-42f6-9ad4-24fb3650fdfe)

The only noticable change is that the new input rule (inputrule.cpp) now needs the event handlers (eventhandler.h), where before our input rule base class in abstractinputrule.cpp needed inputrule.h. What the diagram does not show: The dependency from abstractinputrule to inputrule was inheritance (Is-A), whereas the new dependency from inputrule to eventhandler is composition (Has-A). This enables us to manage instances of the new CInputRule object by value.
### Runtime
![06_dependency_typeerasure_runtime](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/a7a170d1-421d-4196-86fc-98a66180e439)

In addition to the change mentioned above, we also created CEventInfo (eventinfo.cpp) to simplify passing event information to the input rules.
## Where to go from here
I honestly cannot think of a further refinement that would provide a significant improvement to the example. Of course, in the real world, requirements would eventually change, making further refinements necessary. But that's another story...
If you can think of another improvement, please provide feedback.
You may want to continue to the [wrap-up](../WRAPUP.md).
