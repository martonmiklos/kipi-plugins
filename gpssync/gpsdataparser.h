/* ============================================================
 * Authors: Gilles Caulier <caulier dot gilles at kdemail dot net>
 * Date   : 2006-09-19
 * Description : GPS data file parser. 
 *               (GPX format http://www.topografix.com/gpx.asp).
 * 
 * Copyright 2006 by Gilles Caulier <caulier dot gilles at kdemail dot net>
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

#ifndef GPSDATAPARSER_H
#define GPSDATAPARSER_H

// Qt includes.

#include <qdatetime.h>
#include <qmap.h>

// KDE includes.

#include <kurl.h>

// Local includes.

#include "gpsdatacontainer.h"

namespace KIPIGPSSyncPlugin
{

class GPSDataParser
{
	
public:

    GPSDataParser();
    ~GPSDataParser(){};	

    bool loadGPXFile(const KURL& url);    

    void clear();
    int  numPoints();
    bool matchDate(QDateTime photoDateTime, int maxGapTime, int timeZone, 
                   bool interpolate, int interpolationDstTime, 
                   GPSDataContainer& gpsData);

private:

    // Methods used to perform interpolation.
    QDateTime findNextDate(QDateTime dateTime, int secs);
    QDateTime findPrevDate(QDateTime dateTime, int secs);

protected: 

    typedef QMap<QDateTime, GPSDataContainer> GPSDataMap; 

    GPSDataMap m_GPSDataMap;
};

} // NameSpace KIPIGPSSyncPlugin

#endif  // GPSDATAPARSER_H
