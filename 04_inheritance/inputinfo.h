#ifndef INPUTINFO_H
#define INPUTINFO_H

#include <QQuickItem>
#include <QPointer>

//## Instead of passing n parameters, we simply pass a custom type
//  with any number of public members. This has mainly two benefits:
// 1) When we need to add more parameters later, we do not need to change
//    any function signature
// 2) Because of the form of this type (which is called an aggregate),
//    we can use a special, very readable way to initialize it:
//    Aggregate initialization with designated initializers
//    This way of initialization makes it harder to mix up parameter of the same type
//    To see it in action, head back to CWindowEventFilter::eventFilter
class CInputInfo
{
public:
    QPointer<QQuickItem> m_pItemUnderCursor{};
    QPointer<QQuickItem> m_pViewRootObject{};
};

// Fun fact: What exactly an aggregate IS changes in almost every C++ version
//  If you want the current lore, see https://en.cppreference.com/w/cpp/language/aggregate_initialization
// I also recommend "A Tour of C++ Recognized User Type Categories" by Nina Ranns
//  (several recorded conference talks available online)

#endif // INPUTINFO_H
