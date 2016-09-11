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

#ifndef PLUGIN_GYAZO_H
#define PLUGIN_GYAZO_H

// Qt includes

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QVariant>

// LibKIPI includes

#include <libkipi/plugin.h>

class KAction;

using namespace KIPI;

namespace KIPIGyazoPlugin
{

class Plugin_Gyazo : public Plugin
{
    Q_OBJECT

public:

    Plugin_Gyazo(QObject* const parent, const QVariantList& args);
    ~Plugin_Gyazo();

    void setup(QWidget* const);

public Q_SLOTS:
    void slotExport();

private slots:
    void replyFinished(QNetworkReply*reply);

private:
    void setupActions();

private:
    KAction*   m_actionExport;
    QNetworkRequest m_postRequest;
    QNetworkAccessManager *m_manager;
};

} // namespace KIPIGyazoPlugin

#endif // PLUGIN_GYAZO_H
