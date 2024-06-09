#include "dragmoveeventhandler.h"
#include <selectionaccess.h>
#include <QSinglePointEvent>
#include <QQuickItem>
#include "inputinfo.h"

// Again, the actual logic of the event handler is unchanged
//  Now that we look at this switch, we start thinking that this could
//  probably be solved in a more elegant fashion...but I have to stop somewhere.
bool CDragMoveEventHandler::handleSinglePointEvent(const QSinglePointEvent& event, const CInputInfo& inputInfo)
{
    switch(event.type())
    {
        case QEvent::MouseButtonPress:
        {
            if ((event.button() == Qt::LeftButton) && inputInfo.getItemUnderCursor())
            {
                m_DragStartPos = event.scenePosition();
                m_bDragPossible = true;
            }
        }
        case QEvent::MouseMove:
        {
            if (m_bDragPossible)
            {
                const auto currentPos = event.scenePosition();
                if ( (event.buttons().testFlag(Qt::LeftButton)) &&
                    ( ! m_DragOperation.isValid()) )
                {
                    if (QLineF(m_DragStartPos, currentPos).length() >= sm_ItemDragTreshold)
                    {
                        CSelectionAccess::setSelected(inputInfo.getItemUnderCursor(), true);
                        auto selectedItems = CSelectionAccess::getSelectedItemsInTree(inputInfo.getViewRootObject());
                        m_DragOperation = CItemDragOperation{m_DragStartPos,selectedItems};
                    }
                }
                m_DragOperation.moveItems(currentPos);
                return m_DragOperation.isValid();
            }
            break;
        }
        case QEvent::MouseButtonRelease:
        {
            if (event.button() == Qt::LeftButton)
            {
                m_bDragPossible = false;
            }
            if (m_DragOperation.isValid())
            {
                m_DragOperation={};
                return true;
            }
            break;
        }
        default: break;
    }
    return false;
}

