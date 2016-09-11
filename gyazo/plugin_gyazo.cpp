/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2016-09-11
 * Description : a kipi plugin to import/export images to Gyazo web service
 *
 * Copyright (C) 2016 by Miklos Marton <martonmiklosqdev at gmail dot com>
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

#include <QDebug>
#include <QFile>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QDesktopServices>

#include "plugin_gyazo.moc"

// C ANSI includes

extern "C"
{
#include <unistd.h>
}

// KDE includes

#include <kdebug.h>
#include <kapplication.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kstandarddirs.h>
#include <kwindowsystem.h>
#include <kgenericfactory.h>

// LibKIPI includes

#include <libkipi/interface.h>
#include <libkipi/imagecollection.h>

// Local includes

namespace KIPIGyazoPlugin
{

K_PLUGIN_FACTORY( GyazoFactory, registerPlugin<Plugin_Gyazo>(); )
K_EXPORT_PLUGIN ( GyazoFactory("kipiplugin_gyazo") )

Plugin_Gyazo::Plugin_Gyazo(QObject* const parent, const QVariantList& /*args*/)
    : Plugin(GyazoFactory::componentData(), parent, "Gyazo Export")
{
    kDebug(AREA_CODE_LOADING) << "Plugin_Gyazo plugin loaded";

    KIconLoader::global()->addAppDir("kipiplugin_gyazo");

    setUiBaseName("kipiplugin_gyazoui.rc");
    setupXML();
}

Plugin_Gyazo::~Plugin_Gyazo()
{
}

void Plugin_Gyazo::setup(QWidget* const widget)
{
    Plugin::setup(widget);

    if (!interface())
    {
        kDebug() << "Kipi interface is null!";
        return;
    }
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    setupActions();
}

void Plugin_Gyazo::setupActions()
{
    setDefaultCategory(ExportPlugin);

    m_actionExport = new KAction(this);
    m_actionExport->setText(i18n("Export to &Gyazo..."));
    m_actionExport->setIcon(KIcon("kipi-gyazo"));
    m_actionExport->setShortcut(KShortcut(Qt::ALT+Qt::SHIFT+Qt::Key_G));

    connect(m_actionExport, SIGNAL(triggered(bool)),
            this, SLOT(slotExport()) );

    addAction("gyazoexport", m_actionExport);
}

void Plugin_Gyazo::slotExport()
{
    // API is implemented after the Linux Gyazo client:
    // https://github.com/gyazo/Gyazo-for-Linux/blob/master/src/gyazo.rb
    KUrl::List images = interface()->currentSelection().images();
    if (images.isEmpty())
        return;
    
    QFile *file = new QFile(images.first().toLocalFile());
    if (!file->open(QFile::ReadOnly))
    {
        delete file;
        return;
    }

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->setBoundary("----BOUNDARYBOUNDARY----");

    QString metadata = QString("{" \
                               " app: %1," \
                               " title: %2," \
                               " url: %3," \
                               " note: \"%4\"" \
                               "}").arg("app").arg("title").arg("note").arg("note");
    QHttpPart metaDataPart;
    metaDataPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"metadata\""));
    metaDataPart.setBody(metadata.toLocal8Bit());
    multiPart->append(metaDataPart);

    QHttpPart idPart;
    idPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"id\""));
    idPart.setBody("id");
    multiPart->append(idPart);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"imagedata\"; filename=\"gyazo.com\""));
    imagePart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

    multiPart->append(imagePart);

    m_postRequest.setUrl(QUrl("https://upload.gyazo.com/upload.cgi"));
    m_postRequest.setRawHeader("User-Agent", "Gyazo/1.2");

    QNetworkReply *reply = m_manager->post(m_postRequest, multiPart);
    multiPart->setParent(reply); // delete the multiPart with the reply
}

void Plugin_Gyazo::replyFinished(QNetworkReply *reply)
{
    QString url = reply->readAll();
    if (!url.isEmpty())
    {
        QDesktopServices::openUrl(QUrl(url));
    }
}

} // namespace KIPIGyazoPlugin
