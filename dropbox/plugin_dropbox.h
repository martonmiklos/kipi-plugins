/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2008-12-26
 * Description : a kipi plugin to import/export images to Dropbox web service
 *
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef PLUGIN_DROPBOX_H
#define PLUGIN_DROPBOX_H

//Qt includes

#include <QVariant>

//Libkipi includes

#include <libkipi/plugin.h>

class KAction;

namespace KIPIDropboxPlugin{
    class DBWindow;
}

class KJob;

using namespace KIPI;

namespace KIPIDropboxPlugin
{

class Plugin_Dropbox:public Plugin
{
    Q_OBJECT

public:
    Plugin_Dropbox(QObject* const parent, const QVariantList& args);
    ~Plugin_Dropbox();

    void setup(QWidget* const);
    //virtual KJob* exportFiles(const QString& album);

public Q_SLOTS:
    void slotExport();

private:
    void setupActions();

private:
    KAction* m_actionExport;
    DBWindow* m_dlgExport;

};

}//namespace KIPIDropboxPlugin

#endif //PLUGIN_DROPBOX_H

