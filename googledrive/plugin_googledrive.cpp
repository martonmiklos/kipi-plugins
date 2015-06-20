/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2013-11-18
 * Description : a kipi plugin to export images to Google-Drive web service
 *
 * Copyright (C) 2013 by Pankaj Kumar <me at panks dot me>
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

// To disable warnings under MSVC2008 about POSIX methods().
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include "plugin_googledrive.moc"

// C ANSI includes

extern "C"
{
#include <unistd.h>
}

// KDE includes

#include <kdebug.h>
#include <kconfig.h>
#include <kapplication.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kgenericfactory.h>
#include <klibloader.h>
#include <kstandarddirs.h>
#include <kwindowsystem.h>

// LibKIPI includes

#include <libkipi/interface.h>

// Local includes

#include "gdwindow.h"

namespace KIPIGoogleDrivePlugin
{

K_PLUGIN_FACTORY(GoogleDriveFactory, registerPlugin<Plugin_GoogleDrive>(); )
K_EXPORT_PLUGIN(GoogleDriveFactory("kipiplugin_googledrive"))

Plugin_GoogleDrive::Plugin_GoogleDrive(QObject* const parent,const QVariantList& /*args*/)
    : Plugin(GoogleDriveFactory::componentData(), parent, "Google Drive Export")
{
    kDebug(AREA_CODE_LOADING) << "Plugin_GoogleDrive/PicasaWeb Plugin Loaded";

    KIconLoader::global()->addAppDir("kipiplugin_googledrive");
    setUiBaseName("kipiplugin_googledriveui.rc");
    setupXML();
}

Plugin_GoogleDrive::~Plugin_GoogleDrive()
{
}

void Plugin_GoogleDrive::setup(QWidget* const widget)
{
    m_dlgGDriveExport = 0;
    m_dlgPicasaExport = 0;
    m_dlgPicasaImport = 0;

    Plugin::setup(widget);

    if(!interface())
    {
        kDebug() << "kipi interface is null";
        return;
    }

    setupActions();
}

void Plugin_GoogleDrive::setupActions()
{
    setDefaultCategory(ExportPlugin);// uncomment if import feature is added to google drive
    m_actionGDriveExport = new KAction(this);
    m_actionGDriveExport->setText(i18n("Export to &Google Drive..."));
    m_actionGDriveExport->setIcon(KIcon("kipi-googledrive"));
    m_actionGDriveExport->setShortcut(KShortcut(Qt::ALT+Qt::SHIFT+Qt::CTRL+Qt::Key_G));

    connect(m_actionGDriveExport,SIGNAL(triggered(bool)),
            this,SLOT(slotGDriveExport()));

    addAction("googledriveexport",m_actionGDriveExport);
    
    m_actionPicasaExport = new KAction(this);
    m_actionPicasaExport->setText(i18n("Export to &PicasaWeb..."));
    m_actionPicasaExport->setIcon(KIcon("kipi-picasa"));
    m_actionPicasaExport->setShortcut(KShortcut(Qt::ALT+Qt::SHIFT+Qt::Key_P));

    connect(m_actionPicasaExport, SIGNAL(triggered(bool)),
            this, SLOT(slotPicasaExport()));

    addAction("picasawebexport", m_actionPicasaExport);

    m_actionPicasaImport = new KAction(this);
    m_actionPicasaImport->setText(i18n("Import from &PicasaWeb..."));
    m_actionPicasaImport->setIcon(KIcon("kipi-picasa"));
    m_actionPicasaImport->setShortcut(KShortcut(Qt::ALT+Qt::SHIFT+Qt::CTRL+Qt::Key_P));

    connect(m_actionPicasaImport, SIGNAL(triggered(bool)),
            this, SLOT(slotPicasaImport()) );

    addAction("picasawebimport", m_actionPicasaImport, ImportPlugin);
}

void Plugin_GoogleDrive::slotGDriveExport()
{
    KStandardDirs dir;
    QString tmp = dir.saveLocation("tmp",QString("kipi-gd-") + QString::number(getpid()) + QString("/"));

    if(!m_dlgGDriveExport)
    {
        m_dlgGDriveExport = new GDWindow(tmp,kapp->activeWindow(),QString("googledriveexport"));
    }
    else
    {
        if(m_dlgGDriveExport->isMinimized())
        {
            KWindowSystem::unminimizeWindow(m_dlgGDriveExport->winId());
        }

        KWindowSystem::activateWindow(m_dlgGDriveExport->winId());
    }

    m_dlgGDriveExport->reactivate();
}

void Plugin_GoogleDrive::slotPicasaExport()
{
    KStandardDirs dir;
    QString tmp = dir.saveLocation("tmp",QString("kipi-gd-") + QString::number(getpid()) + QString("/"));

    if(!m_dlgPicasaExport)
    {
        m_dlgPicasaExport = new GDWindow(tmp,kapp->activeWindow(),QString("picasawebexport"));
    }
    else
    {
        if(m_dlgPicasaExport->isMinimized())
        {
            KWindowSystem::unminimizeWindow(m_dlgPicasaExport->winId());
        }

        KWindowSystem::activateWindow(m_dlgPicasaExport->winId());
    }

    m_dlgPicasaExport->reactivate();
}

void Plugin_GoogleDrive::slotPicasaImport()
{
    KStandardDirs dir;
    QString tmp = dir.saveLocation("tmp",QString("kipi-gd-") + QString::number(getpid()) + QString("/"));

    if(!m_dlgPicasaImport)
    {
        m_dlgPicasaImport = new GDWindow(tmp,kapp->activeWindow(),QString("picasawebimport"));
    }
    else
    {
        if(m_dlgPicasaImport->isMinimized())
        {
            KWindowSystem::unminimizeWindow(m_dlgPicasaImport->winId());
        }

        KWindowSystem::activateWindow(m_dlgPicasaImport->winId());
    }

    m_dlgPicasaImport->reactivate();
}

} // namespace KIPIGoogleDrivePlugin
