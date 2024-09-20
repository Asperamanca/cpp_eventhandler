# Wrap-Up
You have now seen a code refinement from single function with a big switch to separate functions, separate classes, abstraction via base class and finally a flexible approach using type erasure.
The order suggests that the first implementation is the worst, and the last implementation is the best. But it's not that simple. Because, as so often the answer is "it depends...".
In every refinement, we actually increased the total amount of code. In most refinedments, we added new files. This is a source of complexity that should be taken into account.
## It...depends?
So when I say "it depends..." you fairly ask "...on what?"
Let me try to answer in this way:
In each refinement, the code (as it is) has become more complex IN TOTAL. At the same time, we have often reduced complexity LOCALLY. And also, our changes will reduce the ADDITIONAL complexity required to add new event handlers and event handler types.
- For a very simple example that's not expected to change anymore, it's probably best to use the simplest approach possible. Here, I would probably go for "Functions".
- If you are at the other end of the spectrum, and expect your code to live live and grow bigger and bigger, you need something that can reduce added complexity. Here, the final "Type erasure" or maybe "Multiple inheritance" soltions are likely better.
- If you are somewhere in-between, you may choose the "Inheritance" solution.

This is what I mean when I write "it depends...".

## Material listed in code
- [Aggregate Initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization)
- ["A Tour of C++ Recognized User Type Categories" by Nina Ranns](https://www.youtube.com/watch?v=pdoUnvTwnr4)
- [Rule of three](https://en.cppreference.com/w/cpp/language/rule_of_three)
- [Argument dependent lookup and hidden friends](https://www.modernescpp.com/index.php/argument-dependent-lookup-and-hidden-friends/)
- [Klaus Iglberger - C++ Software Design](https://www.oreilly.com/library/view/c-software-design/9781098113155/)

## Further material
- What are regular types, and why should I care? - ["Regular, Revisited" by Victor Ciura](https://www.youtube.com/watch?v=9ilirLg9TzM)
- Why should I care about value semantics? - ["Value Semantics: Safety, Independence, Projection & Future of Programming" by Dave Abrahams](https://www.youtube.com/watch?v=QthAU-t3PQ4)
- Advice on type-safe programming: ["Typically C++, But why?" by Björn Fahller](https://www.youtube.com/watch?v=ZJKWNBcPHaQ)
- Just generally recommended: ["Coping with other people's C++ code" by Laura Savino](https://www.youtube.com/watch?v=qyz6sOVON68)
