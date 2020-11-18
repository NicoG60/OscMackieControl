#include "iconmanager.h"

IconManager::IconManager() :
    QtAwesome()
{
    initFontAwesome();
}

IconManager& IconManager::instance()
{
    static IconManager inst;
    return inst;
}
