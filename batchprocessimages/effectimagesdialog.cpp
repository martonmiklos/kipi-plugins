//////////////////////////////////////////////////////////////////////////////
//
//    EFFECTIMAGESDIALOG.CPP
//
//    Copyright (C) 2003-2004 Gilles CAULIER <caulier dot gilles at free.fr>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//////////////////////////////////////////////////////////////////////////////


// Include files for Qt

#include <qgroupbox.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qwhatsthis.h>
#include <qcheckbox.h>
#include <qpushbutton.h>

// Include files for KDE

#include <klocale.h>
#include <kconfig.h>
#include <kmessagebox.h>
#include <knuminput.h>
#include <kprocess.h>

// Local includes

#include "effectoptionsdialog.h"
#include "outputdialog.h"
#include "imagepreview.h"
#include "effectimagesdialog.h"

namespace KIPIBatchProcessImagesPlugin
{

//////////////////////////////////// CONSTRUCTOR ////////////////////////////////////////////

EffectImagesDialog::EffectImagesDialog( KURL::List urlList, KIPI::Interface* interface, QWidget *parent )
                  : BatchProcessImagesDialog( urlList, interface, parent )
{
    m_nbItem = m_selectedImageFiles.count();

    setCaption(i18n("Batch FX transforming Images options"));
    setHelp("effectimages", "kipi-plugins");

    //---------------------------------------------

    groupBox1->setTitle( i18n("FX transforming images options") );

    m_labelType->setText( i18n("FX type:") );

    m_Type->insertItem(i18n("Adaptative threshold"));
    m_Type->insertItem(i18n("Charcoal"));
    m_Type->insertItem(i18n("Detect edges"));
    m_Type->insertItem(i18n("Emboss"));
    m_Type->insertItem(i18n("Implode"));
    m_Type->insertItem(i18n("Paint"));
    m_Type->insertItem(i18n("Shade light"));
    m_Type->insertItem(i18n("Solarize"));
    m_Type->insertItem(i18n("Spread"));
    m_Type->insertItem(i18n("Swirl"));
    m_Type->insertItem(i18n("Wave"));
    m_Type->setCurrentText(i18n("Emboss"));
    whatsThis = i18n("<p>Select here the FX type for your images:<p>"
                     "<b>Adaptative threshold</b>: perform local adaptive thresholding. The algorithm "
                     "selects an individual threshold for each pixel based on the range of intensity "
                     "values in its local neighborhood. This allows for thresholding of an image whose "
                     "global intensity histogram doesn't contain distinctive peaks.<p>"
                     "<b>Charcoal</b>: simulate a charcoal drawing.<p>"
                     "<b>Detect edges</b>: detect edges within an image.<p>"
                     "<b>Emboss</b>: returns a grayscale image with a three-dimensional effect. The "
                     "algorithm convolve the image with a Gaussian operator of the given radius and "
                     "standard deviation.<p>"
                     "<b>Implode</b>: implode image pixels about the center.<p>"
                     "<b>Paint</b>: applies a special effect filter that simulates an oil painting.<p>"
                     "<b>Shade light</b>: shines a distant light on an image to create a three-dimensional "
                     "effect.<p>"
                     "<b>Solarize</b>: negate all pixels above the threshold level. This algorithm produces a "
                     "solarization effect seen when exposing a photographic film to light during the development "
                     "process.<p>"
                     "<b>Spread</b>: this is a special effects method that randomly displaces each pixel in a "
                     "block defined by the radius parameter.<p>"
                     "<b>Swirl</b>: swirls the pixels about the center of the image. <p>"
                     "<b>Wave</b>: creates a \"ripple\" effect in the image by shifting the pixels vertically "
                     "along a sine wave.<p>");

    QWhatsThis::add( m_Type, whatsThis );

    //---------------------------------------------

    readSettings();
    listImageFiles();
}


//////////////////////////////////// DESTRUCTOR /////////////////////////////////////////////

EffectImagesDialog::~EffectImagesDialog()
{
}


//////////////////////////////////////// SLOTS //////////////////////////////////////////////

void EffectImagesDialog::slotAbout( void )
{
    KMessageBox::about(this, i18n("A KIPI plugin for batch images FX transformations\n\n"
                                  "Author: Gilles Caulier\n\n"
                                  "Email: caulier dot gilles at free.fr\n\n"
                                  "This plugin use the \"convert\" program from \"ImageMagick\" package.\n"),
                                  i18n("About KIPI batch images FX transformations"));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

void EffectImagesDialog::slotOptionsClicked(void)
{
    QString Type = m_Type->currentText();
    EffectOptionsDialog *optionsDialog = new EffectOptionsDialog(this, Type);

    if ( Type == i18n("Adaptative threshold") )
       {
       optionsDialog->m_latWidth->setValue(m_latWidth);
       optionsDialog->m_latHeight->setValue(m_latHeight);
       optionsDialog->m_latOffset->setValue(m_latOffset);
       }

    if ( Type == i18n("Charcoal") )
       {
       optionsDialog->m_charcoalRadius->setValue(m_charcoalRadius);
       optionsDialog->m_charcoalDeviation->setValue(m_charcoalDeviation);
       }

    if ( Type == i18n("Detect edges") )
       optionsDialog->m_edgeRadius->setValue(m_edgeRadius);

    if ( Type == i18n("Emboss") )
       {
       optionsDialog->m_embossRadius->setValue(m_embossRadius);
       optionsDialog->m_embossDeviation->setValue(m_embossDeviation);
       }

    if ( Type == i18n("Implode") )
       optionsDialog->m_implodeFactor->setValue(m_implodeFactor);

    if ( Type == i18n("Paint") )
       optionsDialog->m_paintRadius->setValue(m_paintRadius);

    if ( Type == i18n("Shade light") )
       {
       optionsDialog->m_shadeAzimuth->setValue(m_shadeAzimuth);
       optionsDialog->m_shadeElevation->setValue(m_shadeElevation);
       }

    if ( Type == i18n("Solarize") )
       optionsDialog->m_solarizeFactor->setValue(m_solarizeFactor);

    if ( Type == i18n("Spread") )
       optionsDialog->m_spreadRadius->setValue(m_spreadRadius);

    if ( Type == i18n("Swirl") )
       optionsDialog->m_swirlDegrees->setValue(m_swirlDegrees);

    if ( Type == i18n("Wave") )
       {
       optionsDialog->m_waveAmplitude->setValue(m_waveAmplitude);
       optionsDialog->m_waveLenght->setValue(m_waveLenght);
       }

    if ( optionsDialog->exec() == KMessageBox::Ok )
       {
       if ( Type == i18n("Adaptative threshold") )
          {
          m_latWidth = optionsDialog->m_latWidth->value();
          m_latHeight = optionsDialog->m_latHeight->value();
          m_latOffset = optionsDialog->m_latOffset->value();
          }

       if ( Type == i18n("Charcoal") )
          {
          m_charcoalRadius = optionsDialog->m_charcoalRadius->value();
          m_charcoalDeviation = optionsDialog->m_charcoalDeviation->value();
          }

       if ( Type == i18n("Detect edges") )
          m_edgeRadius = optionsDialog->m_edgeRadius->value();

       if ( Type == i18n("Emboss") )
          {
          m_embossRadius = optionsDialog->m_embossRadius->value();
          m_embossDeviation = optionsDialog->m_embossDeviation->value();
          }

       if ( Type == i18n("Implode") )
          m_implodeFactor = optionsDialog->m_implodeFactor->value();

       if ( Type == i18n("Paint") )
          m_paintRadius = optionsDialog->m_paintRadius->value();

       if ( Type == i18n("Shade light") )
          {
          m_shadeAzimuth = optionsDialog->m_shadeAzimuth->value();
          m_shadeElevation = optionsDialog->m_shadeElevation->value();
          }

       if ( Type == i18n("Solarize") )
          m_solarizeFactor = optionsDialog->m_solarizeFactor->value();

       if ( Type == i18n("Spread") )
          m_spreadRadius = optionsDialog->m_spreadRadius->value();

       if ( Type == i18n("Swirl") )
          m_swirlDegrees = optionsDialog->m_swirlDegrees->value();

       if ( Type == i18n("Wave") )
          {
          m_waveAmplitude = optionsDialog->m_waveAmplitude->value();
          m_waveLenght = optionsDialog->m_waveLenght->value();
          }
       }

    delete optionsDialog;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

void EffectImagesDialog::readSettings(void)
{
m_config = new KConfig("kipirc");
    m_config->setGroup("EffectImages Settings");

    m_Type->setCurrentText(m_config->readEntry("EffectType", i18n("Normalize")));
    m_latWidth = m_config->readNumEntry("LatWidth", 50);
    m_latHeight = m_config->readNumEntry("LatHeight", 50);
    m_latOffset = m_config->readNumEntry("LatOffset", 1);
    m_charcoalRadius = m_config->readNumEntry("CharcoalRadius", 3);
    m_charcoalDeviation = m_config->readNumEntry("CharcoalDeviation", 3);
    m_edgeRadius = m_config->readNumEntry("EdgeRadius", 3);
    m_embossRadius = m_config->readNumEntry("EmbossRadius", 3);
    m_embossDeviation = m_config->readNumEntry("EmbossDeviation", 3);
    m_implodeFactor = m_config->readNumEntry("ImplodeFactor", 1);
    m_paintRadius = m_config->readNumEntry("PaintRadius", 3);
    m_shadeAzimuth = m_config->readNumEntry("ShadeAzimuth", 40);
    m_shadeElevation = m_config->readNumEntry("ShadeElevation", 40);
    m_solarizeFactor = m_config->readNumEntry("SolarizeFactor", 10);
    m_spreadRadius = m_config->readNumEntry("SpreadRadius", 3);
    m_swirlDegrees = m_config->readNumEntry("SwirlDegrees", 45);
    m_waveAmplitude = m_config->readNumEntry("WaveAmplitude", 50);
    m_waveLenght = m_config->readNumEntry("WaveLenght", 100);

    if (m_config->readEntry("SmallPreview", "true") == "true")
        m_smallPreview->setChecked( true );
    else
        m_smallPreview->setChecked( false );

    m_overWriteMode->setCurrentItem(m_config->readNumEntry("OverWriteMode", 2));  // 'Rename' per default...

    if (m_config->readEntry("RemoveOriginal", "false") == "true")
        m_removeOriginal->setChecked( true );
    else
        m_removeOriginal->setChecked( false );

    delete m_config;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

void EffectImagesDialog::saveSettings(void)
{
    // Write all settings in configuration file.

    m_config = new KConfig("kipirc");
    m_config->setGroup("EffectImages Settings");
    m_config->writeEntry("EffectType", m_Type->currentText());

    m_config->writeEntry("LatWidth", m_latWidth);
    m_config->writeEntry("LatHeight", m_latHeight);
    m_config->writeEntry("LatOffset", m_latOffset);
    m_config->writeEntry("CharcoalRadius", m_charcoalRadius);
    m_config->writeEntry("CharcoalDeviation", m_charcoalDeviation);
    m_config->writeEntry("EdgeRadius", m_edgeRadius);
    m_config->writeEntry("EmbossRadius", m_embossRadius);
    m_config->writeEntry("EmbossDeviation", m_embossDeviation);
    m_config->writeEntry("ImplodeFactor", m_implodeFactor);
    m_config->writeEntry("PaintRadius", m_paintRadius);
    m_config->writeEntry("ShadeAzimuth", m_shadeAzimuth);
    m_config->writeEntry("ShadeElevation", m_shadeElevation);
    m_config->writeEntry("SolarizeFactor", m_solarizeFactor);
    m_config->writeEntry("SpreadRadius", m_spreadRadius);
    m_config->writeEntry("SwirlDegrees", m_swirlDegrees);
    m_config->writeEntry("WaveAmplitude", m_waveAmplitude);
    m_config->writeEntry("WaveLenght", m_waveLenght);

    m_config->writeEntry("SmallPreview", m_smallPreview->isChecked());
    m_config->writeEntry("OverWriteMode", m_overWriteMode->currentItem());
    m_config->writeEntry("RemoveOriginal", m_removeOriginal->isChecked());

    m_config->sync();

    delete m_config;
}


////////////////////////////////////////////// FONCTIONS ////////////////////////////////////////////

QString EffectImagesDialog::makeProcess(KProcess* proc, BatchProcessImagesItem *item,
                                        const QString& albumDest)
{
    *proc << "convert";

    if ( albumDest.isNull() && m_smallPreview->isChecked() )    // Preview mode and small preview enabled !
       {
       *m_PreviewProc << "-crop" << "300x300+0+0";
       m_previewOutput.append( " -crop 300x300+0+0 ");
       }

    if (m_Type->currentText() == i18n("Adaptative threshold"))
       {
       *proc << "-lat";
       QString Temp, Temp2;
       Temp2 = Temp.setNum( m_latWidth ) + "x";
       Temp2.append ( Temp.setNum( m_latHeight ) + "+" );
       Temp2.append ( Temp.setNum( m_latOffset ) );
       *proc << Temp2;
       }

    if (m_Type->currentText() == i18n("Charcoal"))
       {
       *proc << "-charcoal";
       QString Temp, Temp2;
       Temp2 = Temp.setNum( m_charcoalRadius ) + "x";
       Temp2.append ( Temp.setNum( m_charcoalDeviation ) );
       *proc << Temp2;
       }

    if (m_Type->currentText() == i18n("Detect edges"))
       {
       *proc << "-edge";
       QString Temp, Temp2;
       Temp2 = Temp.setNum( m_edgeRadius );
       *proc << Temp2;
       }

    if (m_Type->currentText() == i18n("Emboss"))
       {
       *proc << "-emboss";
       QString Temp, Temp2;
       Temp2 = Temp.setNum( m_embossRadius ) + "x";
       Temp2.append ( Temp.setNum( m_embossDeviation ) );
       *proc << Temp2;
       }

    if (m_Type->currentText() == i18n("Implode"))
       {
       *proc << "-implode";
       QString Temp, Temp2;
       Temp2 = Temp.setNum( m_implodeFactor );
       *proc << Temp2;
       }

    if (m_Type->currentText() == i18n("Paint"))
       {
       *proc << "-paint";
       QString Temp, Temp2;
       Temp2 = Temp.setNum( m_paintRadius );
       *proc << Temp2;
       }

    if (m_Type->currentText() == i18n("Shade light"))
       {
       *proc << "-shade";
       QString Temp, Temp2;
       Temp2 = Temp.setNum( m_shadeAzimuth ) + "x";
       Temp2.append ( Temp.setNum( m_shadeElevation ) );
       *proc << Temp2;
       }

    if (m_Type->currentText() == i18n("Solarize"))
       {
       *proc << "-solarize";
       QString Temp, Temp2;
       Temp2 = Temp.setNum( m_solarizeFactor );
       *proc << Temp2;
       }

    if (m_Type->currentText() == i18n("Spread"))
       {
       *proc << "-spread";
       QString Temp, Temp2;
       Temp2 = Temp.setNum( m_spreadRadius );
       *proc << Temp2;
       }

   if (m_Type->currentText() == i18n("Swirl"))
       {
       *proc << "-swirl";
       QString Temp, Temp2;
       Temp2 = Temp.setNum( m_swirlDegrees );
       *proc << Temp2;
       }

    if (m_Type->currentText() == i18n("Wave"))
       {
       *proc << "-wave";
       QString Temp, Temp2;
       Temp2 = Temp.setNum( m_waveAmplitude ) + "x";
       Temp2.append ( Temp.setNum( m_waveLenght ) );
       *proc << Temp2;
       }

    *proc << "-verbose";

    *proc << item->pathSrc();

    if ( !albumDest.isNull() )   // No preview mode !
       {
       *proc << albumDest + "/" + item->nameDest();
       }

    return(extractArguments(proc));
}

}  // NameSpace KIPIBatchProcessImagesPlugin

#include "effectimagesdialog.moc"
