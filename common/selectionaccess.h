#ifndef SELECTIONACCESS_H
#define SELECTIONACCESS_H

#include <QQuickItem>

enum class ESelectionState : int
{
    NotSelectable = 0,
    NotSelected,
    Selected
};

// Selected and deselects items via a "bSelected" bool property
// Only items with such a property are considered 'selectable'
// DESIGN NOTE:
//  Design of this class is not a focus of this example. In a production implementation, I would probably keep the "selection" information
//  on the C++ side and only mirror it to QML via properties (or at least do some caching on the C++ side)
class CSelectionAccess
{
public:
    static bool isSelectable(QQuickItem* const pItem);
    // true: Item selected, false: item exists, but not selected, empty: item does not exist, or is not selectable
    static ESelectionState getSelectionState(QQuickItem* const pItem);

    static QList<QQuickItem*> getSelectedItemsInTree(QQuickItem* const pStartItem);

    // pItem must be selectable, otherwise nothing happens
    static void setSelected(QQuickItem* const pItem, bool bSelected);
    static void setListSelected(QList<QQuickItem*> itemList, bool bSelected);
    // pItem must be selectable, otherwise nothing happens
    static void toggleSelected(QQuickItem* const pItem);

    // Changes selection for pStartItem and all children
    static void setTreeSelected(QQuickItem* const pStartItem, bool bSelected);

    // Changes selection for any items that are fully contained within sceneRect and pStartItem or children
    static void setTreeSelectedByRect(const QRectF& sceneRect, QQuickItem* const pStartItem, bool bSelected);

    static constexpr const char* const sm_pSelectionPropertyName = "bSelected";
private:
    static QList<QQuickItem*> getItemTree(QQuickItem* const pStartItem);
    static void setSelectedUnchecked(QQuickItem* const pItem, bool bSelected);

};

#endif // SELECTIONACCESS_H
