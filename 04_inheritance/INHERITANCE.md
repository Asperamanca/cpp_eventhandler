# Inheritance
This is our first big, structural change. We separate the interface of an input rule from it's implementation. This way, whoever calls the event handler function of an input rule no longer needs to know about the exact implementation. 
We also offload the job of managing the input rules to a separate class (CInputRuleManager), and make the interface for calling an event handler function simpler by passing a single, custom type (CInputInfo).
And we offload the job of creating the input rules, and define their order (or priority) to yet another class (CInputRuleManagerFactory)
## Improvements
### CWindowsEventFilter no longer needs to know about specific input rules
Thanks to inheritance and the CInputRule interface class, neither CWindowsEventFilter nor the new CInputRuleManager needs to know anything about the implementation details of each input rule
### Input rules are called in correct order without "if / else if" ladders
Now, rules are simply inserted into a container in order, and then called from that container in order.
### Passing parameters to event handler functions is now safer
Thanks to CInputInfo, we have more readable code and less danger of mixing up parameters of the same type.
## Dependencies
This refinement also brings a new level of separation to our dependency diagram: We can now look at two parts of the program separately:
- Initialization: The phase where we set up and prepare our data structures
- Runtime: where we use those data structures to get things done
This is good news: It has become much easier to re-use parts of the runtime in a different application, and just replace the initialization part. This could also apply to unit tests.
### Initialization
![04_dependency_inheritance_init](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/5b65759a-ef81-4c16-b5ae-45b782fdfc9f)

The blue dashed arrows show the path of creating our instances. 
### Runtime
![04_dependency_inheritance_runtime](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/cd95f3fc-7353-4e6f-b730-58e1fc3fb508)

At runtime, our dependency diagram is (almost) completely separated into a left half (main, windoweventfilter, and inputrulemanager) and a right half (the input rules with abstractinputrule). They are only connected by their common dependency on inputrule.h (CInputRule) and inputinfo (CInputInfo). And both of those are extremely simple files with few dependencies.
## Issues
### The input rule (base class and derived class combined) still has more than one job
We have separated concerns somewhat between CAbstractInputRule (taking care of input rule bookkeeping) and the various input rule implementations (taking care of events). But in the final class, those things are again baked together, and hard to separate in the future. In general, it is typically not good to use inheritance to avoid code duplication (rather use composition), but that's what we did here.
### We have to implement dummy functions for events we don't need
This can become confusing and introduces dead code.
