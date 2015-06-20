/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2013-11-18
 * Description : a kipi plugin to export images to Google-Drive web service
 *
 * Copyright (C) 2013 by Pankaj Kumar <me at panks dot me>
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

#include "gdwidget.moc"

// Qt includes

#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

// KDE includes

#include <klocale.h>
#include <kdialog.h>
#include <kcombobox.h>
#include <kpushbutton.h>

// LibKIPI includes

#include <libkipi/interface.h>
#include <libkipi/uploadwidget.h>
#include <libkipi/imagecollection.h>

// Local includes

#include "kpimageslist.h"
#include "kpprogresswidget.h"

namespace KIPIGoogleDrivePlugin
{

GoogleDriveWidget::GoogleDriveWidget(QWidget* const parent, KIPI::Interface* const iface, const QString& serviceName):QWidget(parent)
{
    m_serviceName = serviceName;
    setObjectName("Google Drive Widget");

    QHBoxLayout* const mainLayout =new QHBoxLayout(this);

    //----------------------------------------------------------

    m_imgList = new KIPIPlugins::KPImagesList(this);
    m_imgList->setControlButtonsPlacement(KIPIPlugins::KPImagesList::ControlButtonsBelow);
    m_imgList->setAllowRAW(true);
    m_imgList->loadImagesFromCurrentSelection();
    m_imgList->listView()->setWhatsThis(i18n("This is the list of images to upload to your Google Drive account."));

    QWidget* const settingsBox           = new QWidget(this);
    QVBoxLayout* const settingsBoxLayout = new QVBoxLayout(settingsBox);

    m_headerLbl = new QLabel(this);
    m_headerLbl->setWhatsThis(i18n("This is a clickable link to open Google Drive in a browser."));
    m_headerLbl->setOpenExternalLinks(true);
    m_headerLbl->setFocusPolicy(Qt::NoFocus);

    //------------------------------------------------------------

    QGroupBox* const accountBox   = new QGroupBox(i18n("Account"),settingsBox);
    accountBox->setWhatsThis(i18n("This is the Google Drive account that is currently logged in."));
    QGridLayout* accountBoxLayout = new QGridLayout(accountBox);

    QLabel* const userNameLbl = new QLabel(i18nc("account settings","Name:"),accountBox);
    m_userNameDisplayLbl      = new QLabel(accountBox);

    m_changeUserBtn           = new KPushButton(KGuiItem(i18n("Change Account"), "switch-system-user",
                                                         i18n("Change Google Drive account for transfer")), accountBox);

    accountBoxLayout->addWidget(userNameLbl,          0,0,1,2);
    accountBoxLayout->addWidget(m_userNameDisplayLbl, 0,2,1,2);
    accountBoxLayout->addWidget(m_changeUserBtn,      1,0,1,4);
    accountBoxLayout->setSpacing(KDialog::spacingHint());
    accountBoxLayout->setMargin(KDialog::spacingHint());

    //-------------------------------------------------------------

    QGroupBox* const albBox            = new QGroupBox(i18n("Destination"),settingsBox);
    albBox->setWhatsThis(i18n("This is the Google Drive folder to which selected photos will be uploaded."));
    QGridLayout* const albumsBoxLayout = new QGridLayout(albBox);

    QLabel* const albLbl = new QLabel(i18n("Album:"),albBox);
    m_albumsCoB          = new KComboBox(albBox);
    m_albumsCoB->setEditable(false);

    m_newAlbumBtn = new KPushButton(KGuiItem(i18n("New Album"),"list-add",
                                             i18n("Create new Google Drive folder")),accountBox);
    m_reloadAlbumsBtn = new KPushButton(KGuiItem(i18nc("album list","Reload"),"view-refresh",
                                                 i18n("Reload album list")),accountBox);

    albumsBoxLayout->addWidget(albLbl,            0, 0, 1, 1);
    albumsBoxLayout->addWidget(m_albumsCoB,       0, 1, 1, 4);
    albumsBoxLayout->addWidget(m_newAlbumBtn,     1, 3, 1, 1);
    albumsBoxLayout->addWidget(m_reloadAlbumsBtn, 1, 4, 1, 1);
    
    //----------------------------------------------------------
    
    QGroupBox* const sizeBox         = new QGroupBox(i18n("Max Dimension"), settingsBox);
    sizeBox->setWhatsThis(i18n("This is the maximum dimension of the images. Images larger than this will be scaled down."));
    QVBoxLayout* const sizeBoxLayout = new QVBoxLayout(sizeBox);
    m_dlDimensionCoB                 = new KComboBox(sizeBox);
    m_dlDimensionCoB->addItem(i18n("Original Size"), "d");
    m_dlDimensionCoB->addItem(i18n("1600 px"), "1600");
    m_dlDimensionCoB->addItem(i18n("1440 px"), "1440");
    m_dlDimensionCoB->addItem(i18n("1280 px"), "1280");
    m_dlDimensionCoB->addItem(i18n("1152 px"), "1152");
    m_dlDimensionCoB->addItem(i18n("1024 px"), "1024");
    m_dlDimensionCoB->setCurrentIndex(0);
    sizeBoxLayout->addWidget(m_dlDimensionCoB);
    
    // ------------------------------------------------------------------------

    QGroupBox* const uploadBox         = new QGroupBox(i18n("Destination"), settingsBox);
    uploadBox->setWhatsThis(i18n("This is the location where Picasaweb images will be downloaded."));
    QVBoxLayout* const uploadBoxLayout = new QVBoxLayout(uploadBox);
    m_uploadWidget                     = iface->uploadWidget(uploadBox);
    uploadBoxLayout->addWidget(m_uploadWidget);

    //-----------------------------------------------------------

    QGroupBox* const optionsBox         = new QGroupBox(i18n("Options"),settingsBox);
    optionsBox->setWhatsThis(i18n("These are the options that would be applied to photos before upload."));
    QGridLayout *const optionsBoxLayout = new QGridLayout(optionsBox);

    m_resizeChB = new QCheckBox(optionsBox);
    m_resizeChB->setText(i18n("Resize photos before uploading"));
    m_resizeChB->setChecked(false);

    m_dimensionSpB = new QSpinBox(optionsBox);
    m_dimensionSpB->setMinimum(0);
    m_dimensionSpB->setMaximum(5000);
    m_dimensionSpB->setSingleStep(10);
    m_dimensionSpB->setValue(1600);
    m_dimensionSpB->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_dimensionSpB->setEnabled(false);

    QLabel* const dimensionLbl = new QLabel(i18n("Maximum Dimension:"),optionsBox);

    m_imageQualitySpB          = new QSpinBox(optionsBox);
    m_imageQualitySpB->setMinimum(0);
    m_imageQualitySpB->setMaximum(100);
    m_imageQualitySpB->setSingleStep(1);
    m_imageQualitySpB->setValue(90);
    m_imageQualitySpB->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QLabel* const imageQualityLbl = new QLabel(i18n("JPEG Quality:"),optionsBox);

    optionsBoxLayout->addWidget(m_resizeChB,      0, 0, 1, 5);
    optionsBoxLayout->addWidget(imageQualityLbl,  1, 1, 1, 1);
    optionsBoxLayout->addWidget(m_imageQualitySpB,1, 2, 1, 1);
    optionsBoxLayout->addWidget(dimensionLbl,     2, 1, 1, 1);
    optionsBoxLayout->addWidget(m_dimensionSpB,   2, 2, 1, 1);
    optionsBoxLayout->setRowStretch(3,10);
    optionsBoxLayout->setSpacing(KDialog::spacingHint());
    optionsBoxLayout->setMargin(KDialog::spacingHint());

    m_progressBar = new KIPIPlugins::KPProgressWidget(settingsBox);
    m_progressBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_progressBar->hide();

    //------------------------------------------------------

    settingsBoxLayout->addWidget(m_headerLbl);
    settingsBoxLayout->addWidget(accountBox);
    settingsBoxLayout->addWidget(sizeBox);
    settingsBoxLayout->addWidget(uploadBox);
    settingsBoxLayout->addWidget(albBox);
    settingsBoxLayout->addWidget(optionsBox);
    settingsBoxLayout->addWidget(m_progressBar);
    settingsBoxLayout->setSpacing(KDialog::spacingHint());
    settingsBoxLayout->setMargin(KDialog::spacingHint());

    //--------------------------------------------------------

    mainLayout->addWidget(m_imgList);
    mainLayout->addWidget(settingsBox);
    mainLayout->setSpacing(KDialog::spacingHint());
    mainLayout->setMargin(0);

    updateLabels();

    //-------------------------------------------------------

    connect(m_resizeChB,SIGNAL(clicked()),
            this,SLOT(slotResizeChecked()));

    //connect(m_imgList,SIGNAL(signalImageListChanged()),
            //this,SLOT(slotImageListChanged()));
    if (QString::compare(m_serviceName, QString("picasawebimport"), Qt::CaseInsensitive) == 0)
    {
        m_imgList->hide();
        m_newAlbumBtn->hide();
        optionsBox->hide();
    }
    else
    {
        uploadBox->hide();
        sizeBox->hide();
    }
}

GoogleDriveWidget::~GoogleDriveWidget()
{
}

void GoogleDriveWidget::updateLabels(const QString& name, const QString& url)
{

    if(QString::compare(m_serviceName, QString("googledriveexport"), Qt::CaseInsensitive) == 0)
    {
        QString web("http://www.drive.google.com");

        if (!url.isEmpty())
            web = url;

        m_headerLbl->setText(QString("<b><h2><a href='%1'>"
                                     "<font color=\"#9ACD32\">Google Drive</font>"
                                     "</a></h2></b>").arg(web));
    }
    else
    {
        m_headerLbl->setText(QString("<b><h2><a href='http://picasaweb.google.com/%1'>"
                             "<font color=\"#9ACD32\">Picasaweb</font>"
                             "</a></h2></b>").arg(name));                                //TODO Change this from name to unique username because in URL it needs unique username and not name.
    }


    if (name.isEmpty())
    {
        m_userNameDisplayLbl->clear();
    }
    else
    {
        m_userNameDisplayLbl->setText( QString("<b>%1</b>").arg(name) );
    }
}

KIPIPlugins::KPImagesList* GoogleDriveWidget::imagesList() const
{
    return m_imgList;
}

void GoogleDriveWidget::slotResizeChecked()
{
    m_dimensionSpB->setEnabled(m_resizeChB->isChecked());
    m_imageQualitySpB->setEnabled(m_resizeChB->isChecked());
}

KIPIPlugins::KPProgressWidget* GoogleDriveWidget::progressBar() const
{
    return m_progressBar;
}

} // namespace KIPIGoogleDrivePlugin
