// vim: set tabstop=4 shiftwidth=4 noexpandtab:
/*
A KIPI plugin to generate HTML image galleries
Copyright 2006 by Aurelien Gateau <aurelien dot gateau at free.fr>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Cambridge, MA 02110-1301, USA.

*/

// Self
#include "theme.h"

// KDE
#include <kdesktopfile.h>
#include <kstandarddirs.h>
#include <kurl.h>


namespace KIPIHTMLExport {


static const char* AUTHOR_GROUP="X-HTMLExport Author";

static Theme::List sList;


struct Theme::Private {
	KDesktopFile* mDesktopFile;
	KURL mURL;

	void init(const QString& desktopFileName) {
		mDesktopFile=new KDesktopFile(desktopFileName, true /*read only*/);
		mURL.setPath(desktopFileName);
	}
};


Theme::Theme() {
	d=new Private;
}


Theme::~Theme() {
	delete d->mDesktopFile;
	delete d;
}


const Theme::List& Theme::getList() {
	if (sList.isEmpty()) {
		QStringList list=KGlobal::instance()->dirs()->findAllResources("data", "kipiplugin_htmlexport/themes/*/*.desktop");
		QStringList::Iterator it=list.begin(), end=list.end();
		for (;it!=end; ++it) {
			Theme* theme=new Theme;
			theme->d->init(*it);
			sList << Theme::Ptr(theme);
		}
	}
	return sList;
}


Theme::Ptr Theme::findByPath(const QString& path) {
	const Theme::List& lst=getList();
	Theme::List::ConstIterator it=lst.begin(), end=lst.end();
	for (; it!=end; ++it) {
		if ((*it)->path()==path) {
			return *it;
		}
	}
	return 0;
}


QString Theme::name() const {
	return d->mDesktopFile->readName();
}


QString Theme::comment() const {
	return d->mDesktopFile->readComment();
}


QString Theme::path() const {
	return d->mURL.path();
}


QString Theme::directory() const {
	return d->mURL.directory();
}


QString Theme::authorName() const {
	KConfigGroupSaver saver(d->mDesktopFile, AUTHOR_GROUP);
	return d->mDesktopFile->readEntry("Name");
}


QString Theme::authorUrl() const {
	KConfigGroupSaver saver(d->mDesktopFile, AUTHOR_GROUP);
	return d->mDesktopFile->readEntry("Url");
}


} // namespace
