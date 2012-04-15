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
 *  Main Developers : Eric Maeker <eric.maeker@gmail.com>,                *
 *                    Guillaume Denry <guillaume.denry@gmail.com>          *
 *  Contributors :                                                         *
 *      NAME <MAIL@ADDRESS.COM>                                            *
 ***************************************************************************/
#ifndef MASK_STRING_H
#define MASK_STRING_H

#include <QString>

#include <padtoolsplugin/pad_fragment.h>

namespace PadTools {
class PadDocument;

class PadString : public PadFragment
{
public:
    PadString(const QString &string = "") : PadFragment(), _string(string) {}

	const QString &string() const { return _string; }
	void setValue(const QString &string) { _string = string; }

	void debug(int indent = 0) const;

//    void run(QMap<QString,QVariant> &);
//    void run(QMap<QString,QVariant> &tokens, QTextDocument *source, QTextDocument *out);
    void run(QMap<QString,QVariant> &tokens, PadDocument *document);

private:
	QString _string;
};

}  // PadTools

#endif
