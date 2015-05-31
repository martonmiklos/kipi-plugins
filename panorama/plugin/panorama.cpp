/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2011-05-23
 * Description : a plugin to create panorama by fusion of several images.
 * Acknowledge : based on the expoblending plugin
 *
 * Copyright (C) 2011-2015 by Benjamin Girault <benjamin dot girault at gmail dot com>
 * Copyright (C) 2009-2013 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

// Qt Includes


#include <QApplication>
#include <QCommandLineParser>
#include <QUrl>
#include <QIcon>

// KDE includes

#include <kmessagebox.h>
#include <klocale.h>
#include <kconfig.h>
#include <kglobal.h>

// Local includes

#include "kpversion.h"
#include "kpmetadata.h"
#include "aboutdata.h"
#include "manager/manager.h"

using namespace KIPIPanoramaPlugin;
using namespace KIPIPlugins;

int main(int argc, char* argv[])
{
    KLocalizedString::setApplicationDomain("kipiplugin_panorama");
    QApplication app(argc, argv);

    PanoramaAboutData aboutData;
//     aboutData->setApplicationData();
//     aboutData->setAppName("panorama");
//     aboutData->setCatalogName("kipiplugin_panorama");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("files", i18n("File(s) to open"), "+[file(s)]");
    aboutData.setupCommandLine(&parser);
    aboutData.setProgramLogo(QIcon::fromTheme("kipi-panorama"));

    parser.process(app);
    aboutData.processCommandLine(&parser);

    KPMetadata::initializeExiv2();


    QList<QUrl> urlList;
    const QStringList args = parser.positionalArguments();

    for (auto& arg : args)
//     for(int i = 0; i < args->count(); ++i)
    {
//         urlList.append(args->url(i));
       urlList.append(QUrl(arg));
    }

    Manager mngr;

    mngr.setItemsList(urlList);
    mngr.setIface(0);
    mngr.run();

    app.exec();
    KPMetadata::cleanupExiv2();

    return 0;
}
