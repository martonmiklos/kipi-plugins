/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2011-05-23
 * Description : a plugin to create panorama by fusion of several images.
 * Acknowledge : based on the expoblending plugin
 *
 * Copyright (C) 2011 by Benjamin Girault <benjamin dot girault at gmail dot com>
 * Copyright (C) 2009-2011 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef ACTIONS_H
#define ACTIONS_H

// Qt includes

#include <QString>
#include <QImage>
#include <QMetaType>
#include <QMap>

// KDE includes

#include <kurl.h>

// Local includes

namespace KIPIPanoramaPlugin
{

enum Action
{
    NONE = 0,
    PREPROCESS,
    OPTIMIZE
};

class ItemPreprocessedUrls
{
public:

    ItemPreprocessedUrls() {}

    ItemPreprocessedUrls(const KUrl& preprocessed, const KUrl& preview)
        : preprocessedUrl(preprocessed), previewUrl(preview) {}

    virtual ~ItemPreprocessedUrls() {}

public:

    KUrl preprocessedUrl;              // Can be an original file or an aligned version, depending of user choice.
    KUrl previewUrl;                   // The JPEG preview version, accordingly of preprocessedUrl constent.
};

typedef QMap<KUrl, ItemPreprocessedUrls> ItemUrlsMap;   // Map between original Url and processed temp Urls.

// ----------------------------------------------------------------------------------------------------------

class ActionData
{
public:

    ActionData()
        : starting(false), success(false) {}

public:

    bool           starting;
    bool           success;

    QString        message;

    QImage         image;

    KUrl::List     inUrls;
    KUrl::List     outUrls;

    ItemUrlsMap    preProcessedUrlsMap;

    Action         action;
};

}  // namespace KIPIExpoBlendingPlugin

Q_DECLARE_METATYPE(KIPIPanoramaPlugin::ActionData)
Q_DECLARE_METATYPE(KIPIPanoramaPlugin::ItemPreprocessedUrls)

#endif /* ACTIONS_H */
