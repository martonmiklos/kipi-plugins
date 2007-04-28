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
#ifndef THEME_H
#define THEME_H   

// Qt
#include <qstring.h>
#include <qvaluelist.h>

// KDE
#include <ksharedptr.h>

namespace KIPIHTMLExport {

class Theme : public KShared {
public:
	typedef KSharedPtr<Theme> Ptr;
	typedef QValueList<Ptr> List;

	~Theme();
	QString name() const;
	QString comment() const;

	QString authorName() const;
	QString authorUrl() const;

	/**
	 * Theme path, on hard disk
	 */
	QString path() const;

	/**
	 * Theme directory on hard disk
	 */
	QString directory() const;

	static const List& getList();
	static Ptr findByPath(const QString& path);

private:
	Theme();
	struct Private;
	Private* d;
};

} // namespace

#endif /* THEME_H */
