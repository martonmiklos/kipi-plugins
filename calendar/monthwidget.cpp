/* ============================================================
 * File  : monthwidget.cpp
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Date  : 2003-11-03
 * Description :
 *
 * Copyright 2003 by Renchi Raju

 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include <qdatetime.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qevent.h>
#include <qdragobject.h>
#include <qstrlist.h>

#include <kurl.h>
#include <kurldrag.h>
#include <kiconloader.h>
#include <kfiledialog.h>
#include <kimageio.h>
#include <klocale.h>
#include <kglobal.h>

#include "monthwidget.h"
#include "calsettings.h"
#include <libkipi/thumbnailjob.h>

namespace DKCalendar
{

MonthWidget::MonthWidget( KIPI::Interface* interface, QWidget *parent, int month)
    : QFrame(parent), interface_( interface )
{
    setAcceptDrops(true);
    month_     = month;
    imagePath_ = QString("");
    pixmap_    = new QPixmap(SmallIcon("file_broken",
                                       KIcon::SizeMedium,
                                       KIcon::DisabledState));
    setFixedSize(QSize(70,90));
    setFrameStyle(QFrame::Panel|QFrame::Raised);
}

MonthWidget::~MonthWidget()
{
    if (pixmap_) delete pixmap_;
}

QString MonthWidget::imagePath()
{
    return imagePath_;
}

void MonthWidget::drawContents(QPainter *p)
{
    QString name = KGlobal::locale()->monthName(month_, true);
    QRect cr;

    cr = contentsRect();
    cr.setBottom(70);
    p->drawPixmap(cr.width()/2 - pixmap_->width()/2,
                  cr.height()/2 - pixmap_->height()/2,
                  *pixmap_);

    cr = contentsRect();
    cr.setTop(70);
    p->drawText(cr,Qt::AlignHCenter,name);
}

void MonthWidget::dragEnterEvent(QDragEnterEvent* event)
{
    event->accept(QUriDrag::canDecode(event));
}

void MonthWidget::dropEvent(QDropEvent* event)
{
    KURL::List srcURLs;
    KURLDrag::decode(event, srcURLs);

    KIPI::ThumbnailJob* thumbJob_ =
        new KIPI::ThumbnailJob(srcURLs.first(),64);
    connect(thumbJob_, SIGNAL(signalThumbnail(const KURL&, const QPixmap&)),
            SLOT(slotGotThumbnaiL(const KURL&, const QPixmap&)));
}

void MonthWidget::slotGotThumbnaiL(const KURL& url, const QPixmap& pix)
{
    imagePath_ = url.path();

    CalSettings::instance()->setImage(month_,imagePath_);

    delete pixmap_;
    pixmap_ = new QPixmap(pix);
    update();
}

void MonthWidget::mouseReleaseEvent(QMouseEvent* e)
{
    if (!contentsRect().contains(e->pos())) return;

    if (e->button() == Qt::LeftButton) {
        KURL url =
            KFileDialog::getOpenURL( interface_->currentAlbum()->root().path(),
                                    KImageIO::pattern( KImageIO::Reading),
                                    this,
                                    i18n("Select Image"));
        if (url.isValid()) {
            KIPI::ThumbnailJob* thumbJob_ =
                new KIPI::ThumbnailJob(url,64);
            connect(thumbJob_, SIGNAL(signalThumbnail(const KURL&, const QPixmap&)),
                    SLOT(slotGotThumbnaiL(const KURL&, const QPixmap&)));
        }
    }
    else if (e->button() == Qt::RightButton) {
        imagePath_ = QString("");
        CalSettings::instance()->setImage(month_,imagePath_);
        delete pixmap_;
        pixmap_    = new QPixmap(SmallIcon("file_broken",
                                           KIcon::SizeMedium,
                                           KIcon::DisabledState));
        update();
    }
}

}

