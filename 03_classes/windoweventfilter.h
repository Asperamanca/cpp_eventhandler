#ifndef WINDOWEVENTFILTER_H
#define WINDOWEVENTFILTER_H

#include <QObject>
#include <QQuickView>
#include "dragmoveinputrule.h"
#include "lassoinputrule.h"
#include "selectioninputrule.h"


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
    //## IMPROVEMENT: The private functions which handled
    //  different jobs are gone
    QQuickItem* getViewRootObject();

private:
    // Used across object lifetime
    QPointer<QQuickView> m_pView{};

    //## IMPROVEMENT: We are down to one class member per job
    CDragMoveInputRule m_DragMoveRule;
    CLassoInputRule m_LassoRule;
    CSelectionInputRule m_SelectionRule;

    //## ISSUE: The input rules are hard-wired into the central CWindowEventFilter class
    //  There is no easy way to customize what input rules I want (e.g. for testing)
};

#endif // WINDOWEVENTFILTER_H
