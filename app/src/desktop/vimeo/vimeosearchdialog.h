/*
 * Copyright (C) 2016 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VIMEOSEARCHDIALOG_H
#define VIMEOSEARCHDIALOG_H

#include <QDialog>

class VimeoSearchTypeModel;
class QComboBox;
class QDialogButtonBox;
class QFormLayout;
class QLineEdit;

class VimeoSearchDialog : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(QString query READ query WRITE setQuery)
    Q_PROPERTY(QString order READ order)
    Q_PROPERTY(QString type READ type)

public:
    explicit VimeoSearchDialog(QWidget *parent = 0);

    QString query() const;

    QString type() const;

    QString order() const;

public Q_SLOTS:
    virtual void accept();
    
    void setQuery(const QString &query);

private Q_SLOTS:
    void onQueryChanged(const QString &query);

private:
    VimeoSearchTypeModel *m_typeModel;
    
    QLineEdit *m_queryEdit;

    QComboBox *m_typeSelector;

    QDialogButtonBox *m_buttonBox;

    QFormLayout *m_layout;
};

#endif // VIMEOSEARCHDIALOG_H
