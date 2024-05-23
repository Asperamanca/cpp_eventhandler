#ifndef LASSOINPUTRULE_H
#define LASSOINPUTRULE_H

#include <QPointer>
#include <lassoselectionoperation.h>

class QSinglePointEvent;
class QQuickItem;

class CLassoInputRule
{
public:
    void setViewRootObject(QQuickItem* const pViewRootObject);
    void setLassoAllowed(const bool bArg);

    bool handleLassoEvent(QSinglePointEvent& event, QQuickItem* pItemUnderCursor, QQuickItem* const pViewRootObject);
    bool handleLassoEvent(QKeyEvent& event, QQuickItem* const pViewRootObject);

private:
    QQuickItem* getLassoItem(QQuickItem* const pViewRootObject);

private:
    //## IMPROVEMENT: All members of this class now serve the
    //  job of implementing a lasso selection feature
    bool m_bLassoAllowed = false;
    QPointer<QQuickItem> m_pLasso{};

    // Members used for one event sequence
    CLassoSelectionOperation m_LassoSelectionOperation{};
};

#endif // LASSOINPUTRULE_H
