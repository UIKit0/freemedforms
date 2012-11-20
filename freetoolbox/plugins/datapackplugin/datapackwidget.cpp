/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2012 by Eric MAEKER, MD (France) <eric.maeker@gmail.com>      *
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
 *   Main developers: Eric MAEKER, <eric.maeker@gmail.com>                 *
 *   Contributors:                                                         *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 ***************************************************************************/
/*!
 * \class DataPackPlugin::Internal::DataPackWidget
 * Widget of the datapack page.
 *
 * \sa DataPackPlugin::Internal::DataPackPage
 */

#include "datapackwidget.h"

#include <translationutils/constants.h>

#include <QDebug>

#include "ui_datapackwidget.h"

using namespace DataPackPlugin;
using namespace Internal;
using namespace Trans::ConstantTranslations;

namespace DataPackPlugin {
namespace Internal {
class DataPackWidgetPrivate
{
public:
    DataPackWidgetPrivate(DataPackWidget *parent) :
        q(parent)
    {
        ui = new Ui::DataPackWidget;
    }
    
    ~DataPackWidgetPrivate()
    {
        delete ui;
    }
    
public:
    Ui::DataPackWidget *ui;

private:
    DataPackWidget *q;
};
}  // namespace Internal
} // end namespace DataPackPlugin

/*! Constructor of the DataPackPlugin::Internal::DataPackWidget class */
DataPackWidget::DataPackWidget(QWidget *parent) :
    QWidget(parent),
    d(new DataPackWidgetPrivate(this))
{
    d->ui->setupUi(this);
}

/*! Destructor of the DataPackPlugin::Internal::DataPackWidget class */
DataPackWidget::~DataPackWidget()
{
    if (d)
        delete d;
    d = 0;
}

/*! Initializes the object with the default values. Return true if initialization was completed. */
bool DataPackWidget::initialize()
{
    return true;
}
