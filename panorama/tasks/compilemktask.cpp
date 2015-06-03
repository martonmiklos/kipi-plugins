/* ============================================================
 * 
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2012-03-15
 * Description : a plugin to create panorama by fusion of several images.
 *
 * Copyright (C) 2012-2015 by Benjamin Girault <benjamin dot girault at gmail dot com>
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

#include "compilemktask.h"

// KDE includes

#include <klocale.h>

// Local includes

#include <kipiplugins_debug.h>

namespace KIPIPanoramaPlugin
{

CompileMKTask::CompileMKTask(const KUrl& workDir,
                             const KUrl& mkUrl, const KUrl& /*panoUrl*/,
                             const QString& nonaPath, const QString& enblendPath,
                             const QString& makePath, bool preview)
    : Task(preview ? STITCHPREVIEW : STITCH, workDir),
      /*panoUrl(&panoUrl),*/ mkUrl(&mkUrl), nonaPath(nonaPath),
      enblendPath(enblendPath), makePath(makePath), process(0)
{}

CompileMKTask::~CompileMKTask()
{
    if (process)
    {
        delete process;
        process = 0;
    }
}

void CompileMKTask::requestAbort()
{
    process->kill();
}

void CompileMKTask::run(ThreadWeaver::JobPointer, ThreadWeaver::Thread*)
{
    process = new KProcess();
    process->clearProgram();
    process->setWorkingDirectory(tmpDir.toLocalFile());
    process->setOutputChannelMode(KProcess::MergedChannels);
    process->setProcessEnvironment(QProcessEnvironment::systemEnvironment());

    QStringList args;
    args << makePath;
    args << "-f";
    args << mkUrl->toLocalFile();
    args << QString("ENBLEND='%1'").arg(enblendPath);
    args << QString("NONA='%1'").arg(nonaPath);

    process->setProgram(args);

    qCDebug(KIPIPLUGINS_LOG) << "make command line: " << process->program();

    process->start();

    if (!process->waitForFinished(-1) || process->exitCode() != 0)
    {
        errString = getProcessError(*process);
        successFlag = false;
        return;
    }
    qCDebug(KIPIPLUGINS_LOG) << "make's output:" << endl << process->readAll();

    successFlag = true;
    return;
}

}  // namespace KIPIPanoramaPlugin