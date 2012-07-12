/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2004-01-01
 * Description : Wall Paper kipi-plugin
 *
 * Copyright (C) 2004      by Gregory Kokanosky <gregory dot kokanosky at free.fr>
 * Copyright (C) 2004-2012 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2012      by Varun Herale <varun dot herale at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "plugin_wallpaper.moc"

// QT includes

#include <QDBusInterface>
#include <QDBusReply>

// KDE includes

#include <klocale.h>
#include <kactioncollection.h>
#include <kactionmenu.h>
#include <kgenericfactory.h>
#include <klibloader.h>
#include <kdebug.h>
#include <kapplication.h>
#include <kmessagebox.h>

// LibKIPI includes

#include <libkipi/interface.h>
#include <libkipi/imagecollection.h>

namespace KIPIWallPaperPlugin
{

K_PLUGIN_FACTORY( WallPaperFactory, registerPlugin<Plugin_WallPaper>(); )
K_EXPORT_PLUGIN ( WallPaperFactory("kipiplugin_wallpaper") )

class Plugin_WallPaper::Private
{

public:

    Private()
    {
        actionBackground = 0;
        interface        = 0;
    }

    KActionMenu* actionBackground;
    Interface*   interface;
};

Plugin_WallPaper::Plugin_WallPaper(QObject* const parent, const QVariantList&)
    : Plugin(WallPaperFactory::componentData(), parent, "WallPaper"),
      d(new Private)
{
    kDebug(AREA_CODE_LOADING) << "Plugin_WallPaper plugin loaded";
}

Plugin_WallPaper::~Plugin_WallPaper()
{
    delete d->actionBackground;
    delete d;
}

void Plugin_WallPaper::setup(QWidget* const widget)
{
    Plugin::setup(widget);

    d->actionBackground = new KActionMenu(KIcon("image-jpeg"), i18n("&Set as Background"), actionCollection());
    d->actionBackground->setObjectName("images2desktop");

    KAction* wallpaper = actionCollection()->addAction("images2desktop_setwallpaper");
    wallpaper->setText(i18n("Set as wallpaper"));

    connect(wallpaper, SIGNAL(triggered(bool)),
            this, SLOT(slotSetWallpaper()));

    d->actionBackground->addAction(wallpaper);

    addAction( d->actionBackground );

    d->interface = dynamic_cast<Interface*>( parent() );

    if ( !d->interface )
    {
        kError() << "Kipi interface is null!";
        return;
    }

    ImageCollection selection = d->interface->currentSelection();
    d->actionBackground->setEnabled( selection.isValid() && !selection.images().isEmpty() );

    connect(d->interface, SIGNAL(selectionChanged(bool)),
            d->actionBackground, SLOT(setEnabled(bool)));
 }

void Plugin_WallPaper::slotSetWallpaper()
{
   ImageCollection images = d->interface->currentSelection();

   if ( !images.isValid() )
       return;

   KUrl url                      = images.images()[0];
   QString path                  = url.path();
   QDBusInterface* dbusInterface = new QDBusInterface("org.kde.plasma-desktop", "/App", "local.PlasmaApp");
   QDBusReply<void> reply        = dbusInterface->call("setWallpaperImage", path);

   if ( !reply.isValid() )
       KMessageBox::information(0, i18n("Background cannot be changed. You do not have the correct version "
                                        "of kde-workspace."), i18n("Change Background"));

   delete dbusInterface;
}

Category Plugin_WallPaper::category(KAction* const action) const
{
    if ( action == d->actionBackground )
       return ImagesPlugin;

    kWarning() << "Unrecognized action for plugin category identification";
    return ImagesPlugin; // no warning from compiler, please
}

} // namespace KIPIWallPaperPlugin
