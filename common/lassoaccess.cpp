#include "lassoaccess.h"
#include <QQmlContext>
#include <QQmlEngine>


QQuickItem* CLassoAccess::getLassoItem(QQuickItem* const pStartItem)
{
    if ( ! pStartItem)
    {
        return {};
    }
    auto* const pContext = QQmlEngine::contextForObject(pStartItem);
    if ( ! pContext)
    {
        return {};
    }
    return qobject_cast<QQuickItem*>(pContext->objectForName("lasso"));
    // Sufficient for this example. In real life, we'd have to traverse through child contexts
}

