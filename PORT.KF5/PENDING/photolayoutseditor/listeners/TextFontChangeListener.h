/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2011-09-01
 * Description : a plugin to create photo layouts by fusion of several images.
 * Acknowledge : based on the expoblending plugin
 *
 * Copyright (C) 2011 by Łukasz Spas <lukasz dot spas at gmail dot com>
 * Copyright (C) 2009-2015 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef TEXTFONTCHANGELISTENER_H
#define TEXTFONTCHANGELISTENER_H

#include <QObject>

class QtProperty;

namespace KIPIPhotoLayoutsEditor
{
    class TextItem;

    class TextFontChangeListener : public QObject
    {
            Q_OBJECT

            TextItem * m_item;

        public:

            explicit TextFontChangeListener(TextItem * item, QObject * parent = 0);

        public slots:

            void propertyChanged(QtProperty * property);
    };
}

#endif // TEXTFONTCHANGELISTENER_H
