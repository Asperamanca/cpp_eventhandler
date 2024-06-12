# Multiple Inheritance
Our event handlers can finally choose exactly which types of events they want to handler. And we achieve this through multiple inheritance - a base class (CInputRule) which also handles the bookkeeping for the input rule, and one interface class per event type. Our input rules now inherit from the base class and any combination of event handler interfaces.
We also did some improvements getting the events into CInputRuleManager.
## Improvements
### Event handlers can now implement exactly the event types they need
No more "dummy function implementations" to satisfy the base class!
### We simplified event handling in CWindowEventFilter and the interface of CInputRuleManager
All events are now treated the same inside CWindowEventFilter, and they are passed to CInputRuleManager using the same function.
## Dependencies
The only change visible on the dependency level is the separation of inputrule.cpp and eventhandler.h
### Initialization
![05_dependency_multiinheritance_init_result](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/8a65fb6e-4729-4d24-a99f-2139e39deb5c)

### Runtime
![05_dependency_multiinheritance_runtime_result](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/a6870249-7843-45b5-8383-74bd1aec5b05)

## Issues
### CInputRule is a weird base class, with some attached dangers
CInputRule is used as a base class, but works like a concrete class. It can be instantiated on it's own, that that could lead to dangerous copy operations with "slice" the object, almost certainly causing follow-up bugs.
### We duplicate the loop over input rules for every event type
That shouldn't be necessary. It's one more place in the code we must not forget when extending the event types.
### We dynamic_cast our event handlers a lot
For each event, we do one dynamic_cast per event handler (less if an event handler consumes the event). This might not scale so well.
