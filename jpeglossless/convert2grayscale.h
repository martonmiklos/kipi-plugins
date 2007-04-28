/* ============================================================
 * Author: Gilles Caulier <caulier dot gilles at kdemail dot net>
 * Date  : 2003-10-14
 * Description : batch images grayscale conversion
 *
 * Copyright 2003-2006 by Gilles Caulier <caulier dot gilles at kdemail dot net>
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

#ifndef convert2grayscale_H
#define convert2grayscale_H

// Qt includes.

#include <qobject.h>

class QString;

class KProcess;

namespace KIPIJPEGLossLessPlugin
{

class ImageGrayScale : public QObject
{
    Q_OBJECT

public:

    ImageGrayScale();
    ~ImageGrayScale();

    bool image2GrayScale(const QString& src, const QString& TmpFolder, QString& err);

private slots:

    void slotReadStderr(KProcess*, char*, int);

private:

    bool image2GrayScaleJPEG(const QString& src, const QString& dest, QString& err);
    
    bool image2GrayScaleImageMagick(const QString& src, const QString& dest, QString& err);

private:

    QString m_stdErr;
};

}  // NameSpace KIPIJPEGLossLessPlugin

#endif /* convert2grayscale_H */
