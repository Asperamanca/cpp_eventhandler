#include "selectionaccess.h"
#include <ranges>

bool CSelectionAccess::isSelectable(QQuickItem* const pItem)
{
    if ( ! pItem)
    {
        return false;
    }

    const auto vSelected = pItem->property(sm_pSelectionPropertyName);
    return vSelected.metaType().id() == qMetaTypeId<bool>();
}


ESelectionState CSelectionAccess::getSelectionState(QQuickItem* const pItem)
{
    if ( ! pItem)
    {
        return ESelectionState::NotSelectable;
    }

    const auto vSelected = pItem->property(sm_pSelectionPropertyName);
    if (vSelected.metaType().id() == qMetaTypeId<bool>())
    {
        if (vSelected.toBool())
        {
            return ESelectionState::Selected;
        }
        return ESelectionState::NotSelected;
    }
    return ESelectionState::NotSelectable;
}

QList<QQuickItem*> CSelectionAccess::getSelectedItemsInTree(QQuickItem* const pStartItem)
{
    const auto itemTree = getItemTree(pStartItem);
    auto view = std::views::filter(itemTree,[](QQuickItem* const pItem)->bool
                                   { return (CSelectionAccess::getSelectionState(pItem) == ESelectionState::Selected); } );
    return {view.begin(), view.end()};
}

void CSelectionAccess::setSelected(QQuickItem* const pItem, bool bSelected)
{
    if ( ! isSelectable(pItem))
    {
        return;
    }
    setSelectedUnchecked(pItem, bSelected);
}

void CSelectionAccess::setListSelected(QList<QQuickItem*> itemList, bool bSelected)
{
    std::ranges::for_each(itemList,[&bSelected](QQuickItem* const pItem){setSelected(pItem, bSelected);});
}

void CSelectionAccess::toggleSelected(QQuickItem* const pItem)
{
    const auto eSelectionState = getSelectionState(pItem);
    switch(eSelectionState)
    {
        using enum ESelectionState;
        case NotSelectable:
        {
            break;
        }
        case NotSelected:
        {
            setSelectedUnchecked(pItem, true);
            break;
        }
        case Selected:
        {
            setSelectedUnchecked(pItem, false);
            break;
        }
    }
}

void CSelectionAccess::setTreeSelected(QQuickItem* const pStartItem, bool bSelected)
{
    if ( ! pStartItem)
    {
        return;
    }
    setSelected(pStartItem, bSelected);
    std::ranges::for_each(pStartItem->childItems(),[&bSelected](QQuickItem* const pItem)
                          { CSelectionAccess::setTreeSelected(pItem, bSelected); });
}

void CSelectionAccess::setTreeSelectedByRect(const QRectF& sceneRect, QQuickItem* const pStartItem, bool bSelected)
{
    // This is a quick hack, and not performant
    // Design considerations:
    //  1) Create a generic function that can conditionally select items, so we can specify different conditions without duplicating code
    //  2) Create some kind of caching so we don't have to walk the scene all the time
    if ( ! pStartItem)
    {
        return;
    }
    if (sceneRect.contains(pStartItem->mapRectToScene(pStartItem->boundingRect())))
    {
        setSelected(pStartItem, bSelected);
    }
    std::ranges::for_each(pStartItem->childItems(),[&sceneRect,&bSelected](QQuickItem* const pItem)
                          { CSelectionAccess::setTreeSelectedByRect(sceneRect, pItem, bSelected);} );
}

QList<QQuickItem*> CSelectionAccess::getItemTree(QQuickItem* const pStartItem)
{
    if ( ! pStartItem)
    {
        return {};
    }

    if (pStartItem->childItems().isEmpty())
    {
        return {pStartItem};
    }

    auto list = pStartItem->childItems();
    list.append(pStartItem);
    return list;
}

void CSelectionAccess::setSelectedUnchecked(QQuickItem* const pItem, bool bSelected)
{
    pItem->setProperty(sm_pSelectionPropertyName, QVariant::fromValue<bool>(bSelected));
}
