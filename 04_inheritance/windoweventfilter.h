#ifndef WINDOWEVENTFILTER_H
#define WINDOWEVENTFILTER_H

#include <QObject>
#include <QQuickView>
#include <memory>
#include "inputrulemanager.h"
//## IMPROVEMENT: We don't need to include any of the event handlers / input rules here

class CWindowEventFilter : public QObject
{
public:
    // We take CInputRuleManager as a unique pointer, meaning that we take ownership
    explicit CWindowEventFilter(std::unique_ptr<CInputRuleManager> pInputRuleManager,
                                QObject* pParent = nullptr);
    ~CWindowEventFilter() override;

    void setView(QQuickView* const pView);

    bool eventFilter(QObject* pWatched, QEvent* pEvent) override;

private:
    QQuickItem* getViewRootObject();

private:
    QPointer<QQuickView> m_pView{};
    // Because it's a unique pointer, the object behind m_pInputRuleManager
    //  wil be deleted when this instance of CWindowEventFilter is destroyed
    std::unique_ptr<CInputRuleManager> m_pInputRuleManager;


};

#endif // WINDOWEVENTFILTER_H
