/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2008-10-31
 * Description : a widget to display simple settings
 *
 * Copyright (C) 2008 by Andi Clemens <andi dot clemens at gmx dot net>
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

#include "settingstab.h"
#include "settingstab.moc"

// Qt includes.

#include <QGridLayout>
#include <QPushButton>
#include <QStackedWidget>

// KDE includes.

#include <klocale.h>

// Local includes.

#include "advancedsettings.h"
#include "simplesettings.h"
#include "removalsettings.h"
#include "storagesettingsbox.h"

namespace KIPIRemoveRedEyesPlugin
{

class SettingsTabPriv
{

public:

    SettingsTabPriv()
    {
        storageSettingsBox  = 0;
        settings            = 0;

        advancedSettings    = 0;
        simpleSettings      = 0;

        settingsSwitcherBtn = 0;
        settingsStack       = 0;
    }

    bool                        simpleCorrectionMode;

    QPushButton*                settingsSwitcherBtn;
    QStackedWidget*             settingsStack;

    AdvancedSettings*           advancedSettings;
    RemovalSettings*            settings;
    SimpleSettings*             simpleSettings;
    StorageSettingsBox*         storageSettingsBox;
};

SettingsTab::SettingsTab(QWidget* parent)
              : QWidget(parent),
                d(new SettingsTabPriv)
{
    d->settings             = new RemovalSettings;
    d->simpleCorrectionMode = true;
    d->settingsSwitcherBtn  = new QPushButton;
    d->storageSettingsBox = new StorageSettingsBox;

    // settings stack widget ----------------------------------------------------------

    d->simpleSettings       = new SimpleSettings;
    d->advancedSettings     = new AdvancedSettings;

    d->settingsStack = new QStackedWidget;
    d->settingsStack->insertWidget(Simple, d->simpleSettings);
    d->settingsStack->insertWidget(Advanced, d->advancedSettings);
    d->settingsStack->setCurrentIndex(Simple);
    setSettingsMode(Simple);

    // Set layouts --------------------------------------------------------------

    QGridLayout* settingsTabLayout = new QGridLayout;
    settingsTabLayout->addWidget(d->settingsStack,          0, 0, 1, 1);
    settingsTabLayout->setRowStretch(1, 10);

    QGridLayout* storageLayout = new QGridLayout;
    storageLayout->addWidget(d->storageSettingsBox, 0, 0, 1, 1);
    storageLayout->setRowStretch(1, 10);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addLayout(settingsTabLayout,        0, 0, 1, 2);
    mainLayout->addLayout(storageLayout,            0, 2, 1, 2);
    mainLayout->addWidget(d->settingsSwitcherBtn,   2, 0, 1, 1);
    mainLayout->setRowStretch(1, 10);
    setLayout(mainLayout);

    connect(d->settingsSwitcherBtn, SIGNAL(clicked()),
            this, SLOT(settingsModeChanged()));
}

SettingsTab::~SettingsTab()
{
//    delete d->settings;
    delete d;
}

void SettingsTab::prepareSettings()
{
    d->settings->storageMode            = d->storageSettingsBox->storageMode();
    d->settings->subfolderName          = d->storageSettingsBox->subfolder();
    d->settings->prefixName             = d->storageSettingsBox->prefix();
}

void SettingsTab::loadSettings(RemovalSettings* newSettings)
{
    d->settings = new RemovalSettings(*newSettings);
    d->simpleSettings->loadSettings(d->settings);
    d->advancedSettings->loadSettings(d->settings);
    applySettings();
}

void SettingsTab::applySettings()
{
    d->storageSettingsBox->setPrefix(d->settings->prefixName);
    d->storageSettingsBox->setSubfolder(d->settings->subfolderName);
    d->storageSettingsBox->setStorageMode(d->settings->storageMode);
}

RemovalSettings* SettingsTab::readSettings()
{
    if (d->simpleCorrectionMode)
        d->settings = new RemovalSettings(*(d->simpleSettings->readSettings()));
    else
        d->settings = new RemovalSettings(*(d->advancedSettings->readSettings()));

    prepareSettings();
    return d->settings;
}

RemovalSettings* SettingsTab::readSettingsForSave()
{
    d->settings = d->advancedSettings->readSettings();
    d->settings->simpleMode = d->simpleSettings->simpleMode();
    prepareSettings();
    return d->settings;
}

void SettingsTab::setSettingsMode(SettingsMode mode)
{
    switch (mode)
    {
        case Simple:
            d->settingsSwitcherBtn->setText(i18n("Advanced Mode"));
            d->settingsStack->setCurrentIndex(Simple);
            d->simpleCorrectionMode = true;
            break;

        case Advanced:
            d->settingsSwitcherBtn->setText(i18n("Simple Mode"));
            d->settingsStack->setCurrentIndex(Advanced);
            d->simpleCorrectionMode = false;
            break;
    }
}

void SettingsTab::settingsModeChanged()
{
    switch (d->settingsStack->currentIndex())
    {
        case Simple:
            setSettingsMode(Advanced);
            break;

        case Advanced:
            setSettingsMode(Simple);
            break;
    }
}

void SettingsTab::updateSettings()
{
    if (d->simpleCorrectionMode)
        d->settings = d->simpleSettings->readSettings();
    else
        d->settings = d->advancedSettings->readSettings();
}

} // namespace KIPIRemoveRedEyesPlugin
