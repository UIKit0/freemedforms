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
 * \class Form::FormCollection
 * \brief Holds a book of forms (a specific mode, the central form, duplicates or not...).
 * The form books are created and managed by the Form::FormManager object. Books contain
 * all forms available for a specific mode.
 * \sa Form::FormManager
 */

#include "formcollection.h"
#include "iformitem.h"

#include <translationutils/constants.h>

#include <QDebug>

using namespace Form;
using namespace Internal;
using namespace Trans::ConstantTranslations;

namespace Form {
namespace Internal {
/*!
 * \class Form::Internal::FormCollectionPrivate
 * \brief Private implementation of the Form::FormCollection class.
 */
class FormCollectionPrivate
{
public:
    FormCollectionPrivate(FormCollection *parent) :
        q(parent)
    {
    }

    ~FormCollectionPrivate()
    {
    }

public:
    QList<Form::FormMain *> _emptyRootForms;

private:
    FormCollection *q;
};
}  // namespace Internal
} // end namespace Form

/*! Constructor of the Form::FormCollection class */
FormCollection::FormCollection(QObject *parent) :
    QObject(parent),
    d(new FormCollectionPrivate(this))
{
}

/*! Destructor of the Form::FormCollection class */
FormCollection::~FormCollection()
{
    if (d)
        delete d;
    d = 0;
}

/*! Initializes the object with the default values. Return true if initialization was completed. */
bool FormCollection::initialize()
{
    return true;
}
