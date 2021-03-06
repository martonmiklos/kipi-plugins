/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2006-04-04
 * Description : A KIPI plugin to generate HTML image galleries
 *
 * Copyright (C) 2006-2010 by Aurelien Gateau <aurelien dot gateau at free.fr>
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

// vim: set tabstop=4 shiftwidth=4 noexpandtab:

#ifndef PLUGINHTMLEXPORT_H
#define PLUGINHTMLEXPORT_H

// Qt includes

#include <QVariant>

// Libkipi includes

#include <KIPI/Plugin>

class QAction;

using namespace KIPI;

namespace KIPIHTMLExport
{

/**
 * Implementation of the Plugin abstract class for HTMLExport.
 */
class Plugin_HTMLExport : public Plugin
{
    Q_OBJECT

public:

    Plugin_HTMLExport(QObject* const parent, const QVariantList& args);
    virtual ~Plugin_HTMLExport();

    void setup(QWidget* const widget);

private Q_SLOTS:

    void slotActivate();

private:

    void setupActions();

private:

    struct Private;
    Private* d;
};

} // namespace KIPIHTMLExport

#endif // PLUGINHTMLEXPORT_H
