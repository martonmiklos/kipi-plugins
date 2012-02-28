/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2012-02-12
 * Description : a kipi plugin to export images to the Imgur web service
 *
 * Copyright (C) 2012-2012 by Marius Orcsik <marius at habarnam dot ro>
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

#ifndef IMGURWIDGET_H
#define IMGURWIDGET_H

// Qt includes

#include <QWidget>
#include <QProgressBar>
#include <QLabel>

// KDE includes

#include <kdialog.h>

// LibKIPI includes
#include "libkipi/interface.h"

// Local includes

#include "imgurtalker.h"
#include "imgurimageslist.h"

namespace KIPI
{
class Interface;
}

namespace KIPIImgurExportPlugin
{
class ImgurWidget : public QWidget
{
    Q_OBJECT

public:
    ImgurWidget(KIPI::Interface* interface, QWidget* parent = 0);
    ~ImgurWidget();

    ImgurImagesList* imagesList() const;
    QProgressBar* progressBar() const;

Q_SIGNALS:
    void signalAddItems(KUrl::List list);
    void signalImageListChanged ();

private:
    ImgurImagesList     *m_imagesList;
    QLabel              *m_headerLbl;
    QProgressBar        *m_progressBar;

public Q_SLOTS:
    void slotAddItems(KUrl::List list);
    void slotImageListChanged ();
};
} // namespace KIPIImgurExportPlugin

#endif /* IMGURWIDGET_H */
