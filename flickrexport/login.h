/* ============================================================
 * File  : flickrlogin.h
 * Author: Vardhman Jain <vardhman @ gmail.com>
 * Date  : 2005-07-07
 * Description : 
 * 
 * Copyright 2005 by Vardhman Jain <vardhman @ gmail.com>

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

#ifndef FLICKRLOGIN_H
#define FLICKRLOGIN_H

#include <qdialog.h>

class QLabel;
class QLineEdit;

namespace KIPIFlickrExportPlugin
{

class FlickrLogin : public QDialog
{
public:
    
    FlickrLogin(QWidget* parent, const QString& header,
                 const QString& _name=QString(),
                 const QString& _passwd=QString());
    ~FlickrLogin();

    QString name()     const;
    QString password() const;
    
private:

    QLabel*    m_headerLabel;
    QLineEdit* m_nameEdit;
    QLineEdit* m_passwdEdit;
};

}

#endif 
