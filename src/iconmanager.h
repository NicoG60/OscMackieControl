#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QtAwesome.h>

#define _ima IconManager::instance()

class IconManager : public QtAwesome
{
private:
    IconManager();

public:
    static IconManager& instance();
};

#endif // ICONMANAGER_H
