//////////////////////////////////////////////////////////////////////////////
//
//    BATCHPROCESSIMAGESLIST.H
//
//    Copyright (C) 2004 Gilles CAULIER <caulier dot gilles at gmail dot com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin Street, Fifth Floor, Cambridge, MA 02110-1301, USA.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BATCHPROCESSIMAGE_H
#define BATCHPROCESSIMAGE_H

// Include files for Qt

#include <qobject.h>

// Include files for KDE

#include <klistview.h>

namespace KIPIBatchProcessImagesPlugin
{

class BatchProcessImagesList : public KListView
{
Q_OBJECT

public:
    BatchProcessImagesList(QWidget *parent=0, const char *name=0);

signals:
    void addedDropItems(QStringList filesPath);

protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent* e);
    bool acceptDrag(QDropEvent* e) const;
    void contentsDropEvent(QDropEvent* e);
    void droppedImagesItems(QDropEvent *e);
};

}  // NameSpace KIPIBatchProcessImagesPlugin

#endif
