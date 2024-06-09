#ifndef WINDOWEVENTFILTER_H
#define WINDOWEVENTFILTER_H

#include <QObject>
#include <QQuickView>
#include <memory>
#include "inputrulemanager.h"

// Nothing changed here, so let's head over to the cpp file
class CWindowEventFilter : public QObject
{
public:
    explicit CWindowEventFilter(std::unique_ptr<CInputRuleManager> pInputRuleManager,
                                QObject* pParent = nullptr);
    ~CWindowEventFilter() override;

    void setView(QQuickView* const pView);

    bool eventFilter(QObject* pWatched, QEvent* pEvent) override;

private:
    QQuickItem* getViewRootObject();

private:
    QPointer<QQuickView> m_pView{};
    std::unique_ptr<CInputRuleManager> m_pInputRuleManager;


};

#endif // WINDOWEVENTFILTER_H
