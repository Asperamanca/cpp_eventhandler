#ifndef LASSOSELECTIONOPERATION_H
#define LASSOSELECTIONOPERATION_H

#include <QQuickItem>
#include <QPointer>

class CLassoSelectionOperation
{
public:
    CLassoSelectionOperation() = default;
    CLassoSelectionOperation(QQuickItem* const pRootItem, QPointF selectionStartPos);

    bool isValid() const;
    QPointF getStartPos() const;
    void updateSelection(const QPointF& currentPos);
    void complete();
    void cancel();

private:
    QPointer<QQuickItem> m_pRootItem{};
    QPointF m_StartPos{};
    QPointF m_CurrentPos{};

};

#endif // LASSOSELECTIONOPERATION_H
