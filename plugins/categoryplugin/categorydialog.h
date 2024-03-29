/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2014 by Eric MAEKER, MD (France) <eric.maeker@gmail.com>      *
 *  All rights reserved.                                                   *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program (COPYING.FREEMEDFORMS file).                   *
 *  If not, see <http://www.gnu.org/licenses/>.                            *
 ***************************************************************************/
/***************************************************************************
 *  Main developer: Eric MAEKER, <eric.maeker@gmail.com>                   *
 *  Contributors:                                                          *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 ***************************************************************************/
#ifndef CATEGORYPLUGIN_CATEGORYDIALOG_H
#define CATEGORYPLUGIN_CATEGORYDIALOG_H

#include <categoryplugin/category_exporter.h>

#include <QDialog>
#include <QAbstractTableModel>

namespace Category {
class ICategoryModelHelper;

namespace Internal {
class CategoryDialogPrivate;
}

class CATEGORY_EXPORT CategoryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CategoryDialog(QWidget *parent = 0);
    void setCategoryModel(ICategoryModelHelper *model, const int labelColumn);

public Q_SLOTS:
    void editItem(const QModelIndex &current, const QModelIndex &previous);

    void done(int r);

private:
    Internal::CategoryDialogPrivate *d;
};

}  // End namespace PMH

#endif // CATEGORYPLUGIN_CATEGORYDIALOG_H
