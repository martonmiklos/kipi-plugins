/* ============================================================
 * Authors: Gilles Caulier <caulier dot gilles at kdemail dot net>
 * Date   : 2004-05-16
 * Description : a plugin to synchronize metadata pictures 
 *               with a GPS device.
 *
 * Copyright 2006 by Gilles Caulier
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

// Qt includes.

#include <qlayout.h>
#include <qlabel.h>
#include <qvgroupbox.h>
#include <qgrid.h>
#include <qpushbutton.h>
#include <qfileinfo.h>
#include <qwhatsthis.h>

// KDE includes.

#include <kdebug.h>
#include <klocale.h>
#include <kapplication.h>
#include <kaboutdata.h>
#include <khelpmenu.h>
#include <ksqueezedtextlabel.h>
#include <kiconloader.h>
#include <kpopupmenu.h>
#include <kstandarddirs.h>
#include <klistview.h>
#include <kfiledialog.h>
#include <kconfig.h>
#include <kmessagebox.h>
#include <kglobalsettings.h>
#include <knuminput.h>
#include <kseparator.h>

// Local includes.

#include "gpslistviewitem.h"
#include "pluginsversion.h"
#include "gpssyncdialog.h"
#include "gpssyncdialog.moc"

namespace KIPIGPSSyncPlugin
{

class GPSSyncDialogPriv
{
public:

    GPSSyncDialogPriv()
    {
        listView       = 0;
        interface      = 0;
        maxGapInput    = 0;
        gpxFileName    = 0;
        gpxPointsLabel = 0;
    }

    QLabel             *gpxPointsLabel;

    KListView          *listView;

    KIntSpinBox        *maxGapInput;

    KSqueezedTextLabel *gpxFileName;

    KIPI::Interface    *interface;

    GPSDataParser       gpxParser;
};

GPSSyncDialog::GPSSyncDialog( KIPI::Interface* interface, QWidget* parent)
             : KDialogBase(Plain, i18n("GPS Sync"), 
                           Help|User1|Apply|Close, Close, 
                           parent, 0, true, true )
{
    d = new GPSSyncDialogPriv;
    d->interface = interface;

    setButtonText( User1, i18n("Load GPX File..."));
    enableButton(Apply, false);

    QGridLayout *mainLayout = new QGridLayout(plainPage(), 3, 1, 0, marginHint());

    //---------------------------------------------

    QFrame *headerFrame = new QFrame( plainPage() );
    headerFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    QHBoxLayout* layout = new QHBoxLayout( headerFrame );
    layout->setMargin( 2 ); // to make sure the frame gets displayed
    layout->setSpacing( 0 );
    QLabel *pixmapLabelLeft = new QLabel( headerFrame, "pixmapLabelLeft" );
    pixmapLabelLeft->setScaledContents( false );
    layout->addWidget( pixmapLabelLeft );
    QLabel *labelTitle = new QLabel( i18n("Syncronize Picture Metadata with a GPS Device"),
                                     headerFrame, "labelTitle" );
    layout->addWidget( labelTitle );
    layout->setStretchFactor( labelTitle, 1 );

    QString directory;
    KGlobal::dirs()->addResourceType("kipi_banner_left", KGlobal::dirs()->kde_default("data") + "kipi/data");
    directory = KGlobal::dirs()->findResourceDir("kipi_banner_left", "banner_left.png");

    pixmapLabelLeft->setPaletteBackgroundColor( QColor(201, 208, 255) );
    pixmapLabelLeft->setPixmap( QPixmap( directory + "banner_left.png" ) );
    labelTitle->setPaletteBackgroundColor( QColor(201, 208, 255) );

    // --------------------------------------------------------------

    d->listView = new KListView(plainPage());
    d->listView->addColumn( i18n("Thumbnail") );
    d->listView->addColumn( i18n("Filename") );
    d->listView->addColumn( i18n("Altitude") );
    d->listView->addColumn( i18n("Latitude") );
    d->listView->addColumn( i18n("Longitude") );
    d->listView->addColumn( i18n("Date") );
    d->listView->addColumn( i18n("Extrapoled") );
    d->listView->setResizeMode(QListView::AllColumns);
    d->listView->setAllColumnsShowFocus(true);
    d->listView->setSorting(-1);
    d->listView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    d->listView->setSelectionMode(QListView::Single);
    d->listView->setMinimumWidth(450);

    // ---------------------------------------------------------------

    QWidget *settingsBox = new QGroupBox(0, Qt::Vertical, i18n("Settings"), plainPage());
    QGridLayout* settingsBoxLayout = new QGridLayout(settingsBox->layout(), 5, 1,
                                                     KDialog::spacingHint());

    QLabel *gpxFileLabel = new QLabel(i18n("Current GPX file:"), settingsBox);
    d->gpxFileName       = new KSqueezedTextLabel(i18n("No GPX file"), settingsBox);
    d->gpxPointsLabel    = new QLabel(settingsBox);
    KSeparator *line     = new KSeparator(Horizontal, settingsBox);

    QLabel *maxGapLabel = new QLabel(i18n("Max gap time:"), settingsBox);
    d->maxGapInput      = new KIntSpinBox(0, 2000, 1, 30, 10, settingsBox);
    QWhatsThis::add(d->maxGapInput, i18n("<p>Set here the maximum distance in "
                    "seconds from a GPS point that a photo will be matched."));

    settingsBoxLayout->addMultiCellWidget(gpxFileLabel, 0, 0, 0, 1);     
    settingsBoxLayout->addMultiCellWidget(d->gpxFileName, 1, 1, 0, 1);     
    settingsBoxLayout->addMultiCellWidget(d->gpxPointsLabel, 2, 2, 0, 1);     
    settingsBoxLayout->addMultiCellWidget(line, 3, 3, 0, 1);     
    settingsBoxLayout->addMultiCellWidget(maxGapLabel, 4, 4, 0, 0); 
    settingsBoxLayout->addMultiCellWidget(d->maxGapInput, 4, 4, 1, 1); 

    // ---------------------------------------------------------------

    mainLayout->addMultiCellWidget(headerFrame, 0, 0, 0, 2);
    mainLayout->addMultiCellWidget(d->listView, 1, 3, 0, 1);
    mainLayout->addMultiCellWidget(settingsBox, 1, 1, 2, 2);
    mainLayout->setColStretch(1, 10);
    mainLayout->setRowStretch(3, 10);

    // ---------------------------------------------------------------
    // About data and help button.

    KAboutData* about = new KAboutData("kipiplugins",
                                       I18N_NOOP("GPS Sync"),
                                       kipiplugins_version,
                                       I18N_NOOP("A Plugin to synchronize pictures metadata with a GPS device"),
                                       KAboutData::License_GPL,
                                       "(c) 2006, Gilles Caulier",
                                       0,
                                       "http://extragear.kde.org/apps/kipi");

    about->addAuthor("Gilles Caulier", I18N_NOOP("Author and Maintainer"),
                     "caulier dot gilles at kdemail dot net");

    KHelpMenu* helpMenu = new KHelpMenu(this, about, false);
    helpMenu->menu()->removeItemAt(0);
    helpMenu->menu()->insertItem(i18n("GPS Sync Handbook"),
                                 this, SLOT(slotHelp()), 0, -1, 0);
    actionButton(Help)->setPopup( helpMenu->menu() );

    readSettings();
}

GPSSyncDialog::~GPSSyncDialog()
{
    delete d;
}

void GPSSyncDialog::setImages( const KURL::List& images )
{
    // We only add all JPEG file only on the list because Exiv2 can't 
    // update metadata on others file formats.

    for( KURL::List::ConstIterator it = images.begin(); it != images.end(); ++it )
    {
        QFileInfo fi((*it).path());
        QString ext = fi.extension().upper();

        if (ext == QString("JPG") || ext == QString("JPEG") || ext == QString("JPE"))
            new GPSListViewItem(d->listView, d->listView->lastItem(), *it);
    }
}

// Load GPX data file.
void GPSSyncDialog::slotUser1()
{
    KURL loadGPXFile = KFileDialog::getOpenURL(KGlobalSettings::documentPath(),
                                               i18n("%1|GPS Exchange Format").arg("*.gpx"), this,
                                               QString(i18n("Select GPX File to Load")) );
    if( loadGPXFile.isEmpty() )
       return;

    enableButton(Apply, false);
    d->gpxParser.clear();
    bool ret = d->gpxParser.loadGPXFile(loadGPXFile);

    if (!ret)
    {
        KMessageBox::error(this, i18n("Cannot parse %1 GPX file!")
                           .arg(loadGPXFile.fileName()), i18n("GPS Sync"));    
        enableButton(Apply, false);
        return;
    }

    if (d->gpxParser.numPoints() <= 0)
    {
        KMessageBox::sorry(this, i18n("The %1 GPX file do not have a date-time track to use!")
                           .arg(loadGPXFile.fileName()), i18n("GPS Sync"));    
        enableButton(Apply, false);
        return;
    }

    d->gpxFileName->setText(loadGPXFile.fileName());
    d->gpxPointsLabel->setText(i18n("Points parsed: %1").arg(d->gpxParser.numPoints()));
    enableButton(Apply, true);
    matchGPSAndPhoto();
}

void GPSSyncDialog::slotHelp()
{
    KApplication::kApplication()->invokeHelp("gpssync", "kipi-plugins");
}

void GPSSyncDialog::closeEvent(QCloseEvent *e)
{
    if (!e) return;
    saveSettings();
    e->accept();
}

void GPSSyncDialog::slotClose()
{
    saveSettings();
    KDialogBase::slotClose();
}

void GPSSyncDialog::readSettings()
{
    KConfig config("kipirc");
    config.setGroup("GPS Sync Settings");
    d->maxGapInput->setValue(config.readNumEntry("Max Gap Time", 30));

    resize(configDialogSize(config, QString("GPS Sync Dialog")));
}

void GPSSyncDialog::saveSettings()
{
    KConfig config("kipirc");
    config.setGroup("GPS Sync Settings");
    config.writeEntry("Max Gap Time", d->maxGapInput->value() );

    saveDialogSize(config, QString("GPS Sync Dialog"));
    config.sync();
}

void GPSSyncDialog::matchGPSAndPhoto()
{
    int itemsUpdated = 0;

    QListViewItemIterator it( d->listView );
    while ( it.current() ) 
    {
        GPSListViewItem *item = (GPSListViewItem*) it.current();
        double alt =0.0, lat=0.0, lng = 0.0;
        if (d->gpxParser.parseDates(item->getDateTime(), d->maxGapInput->value(), alt, lat, lng))
        {
            item->setGPSInfo(alt, lat, lng);
            itemsUpdated++;
        }
        ++it;
    }

    if (itemsUpdated == 0)
    {
        KMessageBox::sorry(this, i18n("Cannot find pictures to correlate with GPX file data."),
                           i18n("GPS Sync"));    
        enableButton(Apply, false);
        return;
    }

    KMessageBox::information(this, i18n("GPS data of %1 picture(s) have been updated on "
                             "the list using the GPX data file.\n"
                       "Press Apply button to update picture(s) metadata.")
                       .arg(itemsUpdated), i18n("GPS Sync"));    
    enableButton(Apply, true);
}

void GPSSyncDialog::slotApply()
{
    QListViewItemIterator it( d->listView );
    while ( it.current() ) 
    {
        GPSListViewItem *item = (GPSListViewItem*) it.current();
        d->listView->setSelected(item, true);
        d->listView->ensureItemVisible(item);
        item->writeGPSInfoToFile();
        ++it;
        kapp->processEvents();
    }
}

}  // NameSpace KIPIGPSSyncPlugin
