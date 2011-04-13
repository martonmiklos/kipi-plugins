/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2011-04-12
 * Description : A KIPI Plugin to export albums to rajce.net
 *
 * Copyright (C) 2011 by Lukas Krejci <krejci.l at centrum dot cz>
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

#include "newalbumdialog.h"
#include <klocalizedstring.h>
#include <QFormLayout>
#include <KLineEdit>
#include <KTextEdit>
#include <QCheckBox>

using namespace KIPIRajceExportPlugin;

NewAlbumDialog::NewAlbumDialog(QWidget* parent): KDialog(parent)
{
    setCaption(i18n("New Album"));
    setButtons(Ok | Cancel);
    setDefaultButton(Cancel);
    setModal(false);

    QWidget * mainWidget = new QWidget(this);
    setMainWidget(mainWidget);

    _albumName = new KLineEdit;
    _albumDescription = new KTextEdit;
    _albumVisible = new QCheckBox;

    _albumVisible->setChecked(true);

    QFormLayout * layout = new QFormLayout();
    layout->setMargin(10);
    layout->setRowWrapPolicy(QFormLayout::WrapLongRows);
    layout->addRow(i18n("Name"), _albumName);
    layout->addRow(i18n("Description"), _albumDescription);
    layout->addRow(i18n("Public"), _albumVisible);

    mainWidget->setLayout(layout);
}

QString KIPIRajceExportPlugin::NewAlbumDialog::albumDescription()
{
    return _albumDescription->toPlainText();
}

QString NewAlbumDialog::albumName()
{
    return _albumName->text();
}

bool NewAlbumDialog::albumVisible()
{
    return _albumVisible->isChecked();
}
