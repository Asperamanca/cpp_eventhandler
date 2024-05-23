#ifndef WINDOWEVENTFILTER_H
#define WINDOWEVENTFILTER_H

#include <QObject>
#include <QQuickView>
#include <itemdragoperation.h>
#include <lassoselectionoperation.h>

class CWindowEventFilter : public QObject
{
public:
    explicit CWindowEventFilter(QObject* pParent = nullptr);
    ~CWindowEventFilter() override;

    void setView(QQuickView* const pView);
    void setEditAllowed(const bool bArg);
    void setLassoAllowed(const bool bArg);

    bool eventFilter(QObject* pWatched, QEvent* pEvent) override;

private:
    //## As you can see, we added some more private functions
    // We'll explore them in the cpp file
    // The rest of the header is the same, minus comments
    bool handleLassoEvent(QSinglePointEvent& event, QQuickItem* pItemUnderCursor);
    bool handleLassoEvent(QKeyEvent& event);
    bool handleDragMoveEvent(QSinglePointEvent& event, QQuickItem* pItemUnderCursor);
    bool handleSelectionEvent(QSinglePointEvent& event, QQuickItem* pItemUnderCursor);
    bool handleSelectionEvent(QKeyEvent& event);

    QQuickItem* getViewRootObject();
    QQuickItem* getLassoItem();

private:
    // Used across object lifetime
    QPointer<QQuickView> m_pView{};
    QPointer<QQuickItem> m_pLasso{};
    bool m_bEditAllowed = false;
    bool m_bLassoAllowed = false;

    // Used for one event sequence
    QPointer<QQuickItem> m_pLeftPressedItem{};
    QPointF m_DragStartPos{};
    CItemDragOperation m_DragOperation{};
    CLassoSelectionOperation m_LassoSelectionOperation{};

    //## ISSUE: We have members that work on different features in the same place
    //  With just 6 such members, things are still in control, but as the list
    //  grows, things will become hard to follow
    // The term here is "low cohesion".
    // Cohesion is a software design metric that measures how tightly
    //   the parts of a symbol (in this case a class) are bound together.
    //
    // For example, we have m_DragStartPos, which is a member of the whole class,
    //  but only ever used in a single function.
    // A sure sign we can extract some code: We want to keep
    //  the parts together that fit together, and separate
    //  the parts that don't depend on each other.

    static constexpr qreal sm_ItemDragTreshold = 5.0;
};

#endif // WINDOWEVENTFILTER_H
