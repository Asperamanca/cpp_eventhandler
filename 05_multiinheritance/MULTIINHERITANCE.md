# Multiple Inheritance

## Improvements
### Event handlers can now implement exactly the event types they need
No more "dummy function implementations" to satisfy the base class!
### We simplified event handling in CWindowEventFilter and the interface of CInputRuleManager
All events are now treated the same inside CWindowEventFilter, and they are passed to CInputRuleManager using the same function.
## Dependencies

## Issues
### CInputRule is a weird base class, with some attached dangers
CInputRule is used as a base class, but works like a concrete class. It can be instantiated on it's own, that that could lead to dangerous copy operations with "slice" the object, almost certainly causing follow-up bugs.
### We duplicate the loop over input rules for every event type
That shouldn't be necessary. It's one more place in the code we must not forget when extending the event types.
### We dynamic_cast our event handlers a lot
For each event, we do one dynamic_cast per event handler (less if an event handler consumes the event). This might not scale so well.
