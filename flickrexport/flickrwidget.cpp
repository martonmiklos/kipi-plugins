/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2005-07-07
 * Description : a kipi plugin to export images to Flickr web service
 *
 * Copyright (C) 2005-2008 by Vardhman Jain <vardhman at gmail dot com>
 * Copyright (C) 2008 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

// Qt includes.

#include <qpushbutton.h>
#include <qlabel.h>
#include <qframe.h>
#include <qheader.h>
#include <qlistview.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qsplitter.h>
#include <qwhatsthis.h>
#include <qlineedit.h>

// KDE includes.

#include <kdialog.h>
#include <kactivelabel.h>
#include <klocale.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <kseparator.h>
#include <kiconloader.h>
#include <kapplication.h>

// Local includes.

#include "flickrwidget.h"
#include "flickrwidget.moc"

namespace KIPIFlickrExportPlugin
{

FlickrWidget::FlickrWidget(QWidget* parent)
            : QWidget(parent)
{
    QVBoxLayout* flickrWidgetLayout = new QVBoxLayout(this, 5, 5);

    m_photoView               = 0; //new KHTMLPart(splitter);
    KSeparator *line          = new KSeparator(Horizontal, this);
    QSplitter* splitter       = new QSplitter(this);
    m_tagView                 = new QListView(splitter);
    KActiveLabel *headerLabel = new KActiveLabel(this);
    headerLabel->setFocusPolicy(NoFocus);
    headerLabel->setLinkUnderline(false);
    headerLabel->setText(i18n("<qt><b><h2><a href='http://www.flickr.com'>"
                              "<font color=\"#0065DE\">flick</font>"
                              "<font color=\"#FF0084\">r</font></a>"
                              " Export"
                              "</h2></b></qt>"));

    m_tagView->hide();
    //m_tagView->addColumn(i18n("Albums"));
    //m_tagView->setResizeMode(QListView::AllColumns);
    //m_tagView->header()->setLabel(0, i18n( "Albums"));

    // -------------------------------------------------------------------

    QGroupBox* leftPannelBox         = new QGroupBox(i18n("Upload Options"), splitter);
    leftPannelBox->setColumnLayout(0, Qt::Vertical);
    leftPannelBox->layout()->setSpacing(KDialog::spacingHint());
    leftPannelBox->layout()->setMargin(0);
    QVBoxLayout* leftPannelBoxLayout = new QVBoxLayout(leftPannelBox->layout());

    //m_newAlbumBtn = new QPushButton(leftPannelBox, "m_newAlbumBtn");
    //m_newAlbumBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //m_newAlbumBtn->setText(i18n("&New Album"));

    QGridLayout* tagsLayout  = new QGridLayout(1, 1);
    QLabel* tagsLabel        = new QLabel(i18n("Added Tags: "), leftPannelBox);
    m_tagsLineEdit           = new QLineEdit(leftPannelBox);
    m_exportHostTagsCheckBox = new QCheckBox(leftPannelBox);
    m_exportHostTagsCheckBox->setText(i18n("Use Host Application Tags"));

    tagsLayout->addWidget(tagsLabel,               0, 0);
    tagsLayout->addWidget(m_tagsLineEdit,          0, 1);
    tagsLayout->addWidget(m_exportHostTagsCheckBox, 1, 1);

    // ------------------------------------------------------------------------

    QGroupBox* optionsBox         = new QGroupBox(i18n("Override Default Options"), leftPannelBox);
    optionsBox->setColumnLayout(0, Qt::Vertical);
    optionsBox->layout()->setSpacing(KDialog::spacingHint());
    optionsBox->layout()->setMargin(KDialog::spacingHint());
    QGridLayout* optionsBoxLayout = new QGridLayout(optionsBox->layout(), 5, 3);

    m_publicCheckBox = new QCheckBox(optionsBox);
    m_publicCheckBox->setText(i18n("As in accessible for people", "Public (anyone can see them)"));

    m_familyCheckBox = new QCheckBox(optionsBox);
    m_familyCheckBox->setText(i18n("Visible to Family"));

    m_friendsCheckBox = new QCheckBox(optionsBox);
    m_friendsCheckBox->setText(i18n("Visible to Friends"));

    m_resizeCheckBox = new QCheckBox(optionsBox);
    m_resizeCheckBox->setText(i18n("Resize photos before uploading"));
    m_resizeCheckBox->setChecked(false);

    m_dimensionSpinBox = new QSpinBox(0, 5000, 10, optionsBox);
    m_dimensionSpinBox->setValue(600);
    m_dimensionSpinBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_dimensionSpinBox->setEnabled(false);

    QLabel* resizeLabel = new QLabel(i18n("Maximum dimension (pixels):"), optionsBox);

    m_imageQualitySpinBox = new QSpinBox(0, 100, 1, optionsBox);
    m_imageQualitySpinBox->setValue(85);
    m_imageQualitySpinBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // NOTE: The term Compression factor may be to technical to write in the label
    QLabel* imageQualityLabel = new QLabel(i18n("JPEG Image Quality (higher is better):"), optionsBox);

    optionsBoxLayout->addMultiCellWidget(m_publicCheckBox,      0, 0, 0, 3);
    optionsBoxLayout->addMultiCellWidget(m_familyCheckBox,      1, 1, 0, 3);
    optionsBoxLayout->addMultiCellWidget(m_friendsCheckBox,     2, 2, 0, 3);
    optionsBoxLayout->addMultiCellWidget(imageQualityLabel,     3, 3, 0, 2);
    optionsBoxLayout->addMultiCellWidget(m_imageQualitySpinBox, 3, 3, 3, 3);
    optionsBoxLayout->addMultiCellWidget(m_resizeCheckBox,      4, 4, 0, 3);
    optionsBoxLayout->addMultiCellWidget(resizeLabel,           5, 5, 1, 2);
    optionsBoxLayout->addMultiCellWidget(m_dimensionSpinBox,    5, 5, 3, 3);
    optionsBoxLayout->setColSpacing(0, KDialog::spacingHint());
    optionsBoxLayout->setColStretch(1, 10);
    optionsBoxLayout->setSpacing(KDialog::spacingHint());
    optionsBoxLayout->setMargin(0);

    // ------------------------------------------------------------------------

    QGroupBox* loginDetailsBox = new QGroupBox(i18n("Login"), leftPannelBox);
    loginDetailsBox->setColumnLayout(0, Qt::Vertical);
    loginDetailsBox->layout()->setSpacing(KDialog::spacingHint());
    loginDetailsBox->layout()->setMargin(KDialog::spacingHint());
    QGridLayout* loginDetailsBoxLayout = new QGridLayout(loginDetailsBox->layout(), 1, 2);

    QLabel *userNameLabel  = new QLabel(i18n("User Name: "), loginDetailsBox);
    m_userNameDisplayLabel = new QLabel(loginDetailsBox);
    m_changeUserButton     = new QPushButton(loginDetailsBox);
    m_changeUserButton->setText(i18n("Use a different account"));
    m_changeUserButton->setIconSet(SmallIcon("switchuser"));

    loginDetailsBoxLayout->addMultiCellWidget(userNameLabel,          0, 0, 0, 0);
    loginDetailsBoxLayout->addMultiCellWidget(m_userNameDisplayLabel, 0, 0, 1, 2);
    loginDetailsBoxLayout->addMultiCellWidget(m_changeUserButton,     1, 1, 0, 2);
    loginDetailsBoxLayout->setColStretch(2, 10);
    loginDetailsBoxLayout->setSpacing(KDialog::spacingHint());
    loginDetailsBoxLayout->setMargin(0);

    // ------------------------------------------------------------------

    m_fileSrcButtonGroup = new QButtonGroup(i18n("Files List"), splitter);
    m_fileSrcButtonGroup->setRadioButtonExclusive(true);
    m_fileSrcButtonGroup->setColumnLayout(0, Qt::Vertical);
    m_fileSrcButtonGroup->layout()->setSpacing(KDialog::spacingHint());
    m_fileSrcButtonGroup->layout()->setMargin(KDialog::spacingHint());

    m_currentSelectionButton = new QRadioButton(m_fileSrcButtonGroup);
    m_currentSelectionButton->setText(i18n("Use Current Selection"));
    m_currentSelectionButton->setChecked(true);

    m_selectImagesButton = new QRadioButton(m_fileSrcButtonGroup);
    m_selectImagesButton->setText(i18n("Custom Selection"));
    m_selectImagesButton->setEnabled(true);

    m_addPhotoButton = new QPushButton(m_fileSrcButtonGroup);
    m_addPhotoButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_addPhotoButton->setText(i18n("&Add Photos"));
    m_addPhotoButton->setIconSet(SmallIcon("add"));

    QVBoxLayout* m_fileSrcButtonGroupLayout = new QVBoxLayout(m_fileSrcButtonGroup->layout());
    m_fileSrcButtonGroupLayout->setAlignment(Qt::AlignTop);
    m_fileSrcButtonGroupLayout->addWidget(m_currentSelectionButton);
    m_fileSrcButtonGroupLayout->addWidget(m_selectImagesButton);
    m_fileSrcButtonGroupLayout->addWidget(m_addPhotoButton);

    // ------------------------------------------------------------------------

    leftPannelBoxLayout->addLayout(tagsLayout);
    leftPannelBoxLayout->addWidget(optionsBox);
    leftPannelBoxLayout->addWidget(loginDetailsBox);
    leftPannelBoxLayout->setSpacing(KDialog::spacingHint());
    leftPannelBoxLayout->setMargin(KDialog::spacingHint());

    flickrWidgetLayout->addWidget(headerLabel);
    flickrWidgetLayout->addWidget(line);
    flickrWidgetLayout->addWidget(splitter, 5);
    flickrWidgetLayout->setSpacing(KDialog::spacingHint());
    flickrWidgetLayout->setMargin(0);

    // ------------------------------------------------------------------------

    connect(m_selectImagesButton, SIGNAL(clicked()),
            this, SLOT(slotSelectionChecked()));

    connect(m_resizeCheckBox, SIGNAL(clicked()),
            this, SLOT(slotResizeChecked()));

    // ------------------------------------------------------------------------

    setName("FlickrWidget");
    resize( QSize(600, 400).expandedTo(minimumSizeHint()) );
    clearWState(WState_Polished);
}

FlickrWidget::~FlickrWidget()
{
}

void FlickrWidget::slotSelectionChecked()
{
    kdDebug() << "Slot Selection Checked" << endl;
    m_addPhotoButton->setEnabled(m_selectImagesButton->isChecked());
}

void FlickrWidget::slotResizeChecked()
{
    m_dimensionSpinBox->setEnabled(m_resizeCheckBox->isChecked());
}

} // namespace KIPIFlickrExportPlugin
