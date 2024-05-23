#ifndef WINDOWEVENTFILTER_H
#define WINDOWEVENTFILTER_H

#include <QObject>
#include <QQuickView>
#include <itemdragoperation.h>
#include <lassoselectionoperation.h>

//## This class receives events and can filter them
// NAMING: Class types start with uppercase "C"
class CWindowEventFilter : public QObject
{
// QT: QObject is an important base class in the Qt framework
//  It represents an object with identity (hence it cannot be copied)
//  It allows Qt features such as signals and slots (a kind of publisher/observer pattern implementation)
//  and also reflection on properties declared within the QObject
// Most of the time, a QObject will have a macro Q_OBJECT inside the class declaration
//  However, this is not an example about Qt, and we don't need the capabilities provided
//  by the macro, so I omit it.
// The reason CWindowEventFilter must be a QObject is that the function 'installEventFilter'
//  which we use later requires the target object to be derived from QObject
public:
    explicit CWindowEventFilter(QObject* pParent = nullptr);
    ~CWindowEventFilter() override;

    // In order to do something useful, I need the view I am attached to
    void setView(QQuickView* const pView);

    // Configure which features are enabled
    void setEditAllowed(const bool bArg);
    void setLassoAllowed(const bool bArg);

    //## This function gets called whenever our window receives an event
    bool eventFilter(QObject* pWatched, QEvent* pEvent) override;

private:
    // Some helper functions
    QQuickItem* getViewRootObject();
    QQuickItem* getLassoItem();

private:
    // Some members used across object lifetime
    // NAMING: Class members start with 'm_', Pointer are prefixed with 'p'
    QPointer<QQuickView> m_pView{};
    QPointer<QQuickItem> m_pLasso{};
    // NAMING: bool variables are prefixed with 'b'
    bool m_bEditAllowed = false;
    bool m_bLassoAllowed = false;

    // Some members used for one event sequence
    QPointer<QQuickItem> m_pLeftPressedItem{};
    QPointF m_DragStartPos{};
    CItemDragOperation m_DragOperation{};
    CLassoSelectionOperation m_LassoSelectionOperation{};

    // A constant to specify how far the user needs to drag an object
    //  before a drag operation can start
    // NAMING: Statics start with 's'
    static constexpr qreal sm_ItemDragTreshold = 5.0;
};

#endif // WINDOWEVENTFILTER_H
