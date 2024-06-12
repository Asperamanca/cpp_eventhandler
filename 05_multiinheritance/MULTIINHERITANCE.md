# Multiple Inheritance
Our event handlers can finally choose exactly which types of events they want to handler. And we achieve this through multiple inheritance - a base class (CInputRule) which also handles the bookkeeping for the input rule, and one interface class per event type. Our input rules now inherit from the base class and any combination of event handler interfaces.
We also did some improvements getting the events into CInputRuleManager.
## Improvements
### Event handlers can now implement exactly the event types they need
No more "dummy function implementations" to satisfy the base class!
### We simplified event handling in CWindowEventFilter and the interface of CInputRuleManager
All events are now treated the same inside CWindowEventFilter, and they are passed to CInputRuleManager using the same function.
## Dependencies
We separated inputrule.cpp and eventhandler.h, and also created CEventInfo (eventinfo.cpp) to simplify passing event information to the input rules.
### Initialization
![05_dependency_multiinheritance_init_result](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/f414141d-a22d-48b3-931e-506e7af52ea0)

### Runtime
![05_dependency_multiinheritance_runtime_result](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/d84a65e0-13bd-46d8-bbf5-ee5a0fa0e84d)

## Issues
### CInputRule is a weird base class, with some attached dangers
CInputRule is used as a base class, but works like a concrete class. It can be instantiated on it's own, that that could lead to dangerous copy operations with "slice" the object, almost certainly causing follow-up bugs.
### We duplicate the loop over input rules for every event type
That shouldn't be necessary. It's one more place in the code we must not forget when extending the event types.
### We dynamic_cast our event handlers a lot
For each event, we do one dynamic_cast per event handler (less if an event handler consumes the event). This might not scale so well.
## Next
[Type erasure](../06_typeerasure/TYPEERASURE.md)
