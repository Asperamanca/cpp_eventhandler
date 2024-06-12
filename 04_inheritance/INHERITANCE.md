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
![04_dependency_inheritance_init_result](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/585dfa4f-bbb0-4f79-b92b-725b9ee464ec)

The blue dashed arrows show the path of creating our instances. 
### Runtime
![04_dependency_inheritance_runtime_result](https://github.com/Asperamanca/cpp_eventhandler/assets/59048940/7d145b3c-9155-4ea8-9234-928c8b8def1a)


At runtime, our dependency diagram is (almost) completely separated into a left half (main, windoweventfilter, and inputrulemanager) and a right half (the input rules). They are only connected by their common dependency on inputrule.cpp (CInputRule) and inputinfo.cpp (CInputInfo). And both of those are extremely simple files with few dependencies.
## Issues
### The CWindowEventFilter still needs to know about all the event types
As it is, the CWindowEventFilter handles each type of event separately, and passes it to the CInputRuleManager via a separate interface. Can't we simplify that?
### We have to implement dummy functions for events we don't need
This can become confusing and introduces dead code.
## Next
[Multiple inheritance](../05_multiinheritance/MULTIINHERITANCE.md)
