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

#include "dailymotiondownloaddialog.h"
#include "categorynamemodel.h"
#include "resources.h"
#include "settings.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

DailymotionDownloadDialog::DailymotionDownloadDialog(QWidget *parent) :
    QDialog(parent),
    m_streamModel(new DailymotionStreamModel(this)),
    m_subtitleModel(new DailymotionSubtitleModel(this)),
    m_categoryModel(new CategoryNameModel(this)),
    m_streamSelector(new QComboBox(this)),
    m_subtitleSelector(new QComboBox(this)),
    m_categorySelector(new QComboBox(this)),
    m_subtitleCheckBox(new QCheckBox(tr("Download &subtitles"), this)),
    m_commandCheckBox(new QCheckBox(tr("&Override global custom command"), this)),
    m_commandEdit(new QLineEdit(this)),
    m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this)),
    m_layout(new QFormLayout(this))
{
    setWindowTitle(tr("Download video"));

    m_streamSelector->setModel(m_streamModel);

    m_subtitleSelector->setModel(m_subtitleModel);

    m_categorySelector->setModel(m_categoryModel);

    m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    m_layout->addRow(tr("Video &format:"), m_streamSelector);
    m_layout->addRow(m_subtitleCheckBox);
    m_layout->addRow(tr("Subtitles &language:"), m_subtitleSelector);
    m_layout->addRow(tr("&Category:"), m_categorySelector);
    m_layout->addRow(tr("&Custom command:"), m_commandEdit);
    m_layout->addRow(m_commandCheckBox);
    m_layout->addRow(m_buttonBox);

    connect(m_streamModel, SIGNAL(statusChanged(QDailymotion::StreamsRequest::Status)),
            this, SLOT(onStreamModelStatusChanged(QDailymotion::StreamsRequest::Status)));
    connect(m_subtitleModel, SIGNAL(statusChanged(QDailymotion::ResourcesRequest::Status)),
            this, SLOT(onSubtitleModelStatusChanged(QDailymotion::ResourcesRequest::Status)));
    connect(m_subtitleCheckBox, SIGNAL(toggled(bool)), this, SLOT(setSubtitlesEnabled(bool)));
    connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

QString DailymotionDownloadDialog::videoId() const {
    return m_videoId;
}

QString DailymotionDownloadDialog::streamId() const {
    return m_streamSelector->itemData(m_streamSelector->currentIndex()).toMap().value("id").toString();
}

QString DailymotionDownloadDialog::subtitlesLanguage() const {
    return m_subtitleCheckBox->isChecked() ? m_subtitleSelector->currentText() : QString();
}

QString DailymotionDownloadDialog::category() const {
    return m_categorySelector->currentText();
}

QString DailymotionDownloadDialog::customCommand() const {
    return m_commandEdit->text();
}

bool DailymotionDownloadDialog::customCommandOverrideEnabled() const {
    return m_commandCheckBox->isChecked();
}

void DailymotionDownloadDialog::accept() {
    Settings::setDefaultDownloadFormat(Resources::DAILYMOTION, m_streamSelector->currentText());
    Settings::setSubtitlesEnabled(m_subtitleCheckBox->isChecked());
    Settings::setSubtitlesLanguage(subtitlesLanguage());
    Settings::setDefaultCategory(category());
    QDialog::accept();
}

void DailymotionDownloadDialog::list(const QString &videoId) {
    m_videoId = videoId;
    m_streamModel->list(videoId);

    if (m_subtitleCheckBox->isChecked()) {
        m_subtitleModel->list(videoId);
    }
    else {
        m_subtitleCheckBox->setChecked(Settings::subtitlesEnabled());
    }
}

void DailymotionDownloadDialog::setSubtitlesEnabled(bool enabled) {
    if (enabled) {
        m_subtitleModel->list(m_videoId);
    }
    else {
        m_subtitleModel->cancel();
    }
}

void DailymotionDownloadDialog::onStreamModelStatusChanged(QDailymotion::StreamsRequest::Status status) {
    switch (status) {
    case QDailymotion::StreamsRequest::Loading:
        m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        break;
    case QDailymotion::StreamsRequest::Ready:
        m_streamSelector->setCurrentIndex(qMax(0, m_streamModel->match("name",
                                          Settings::defaultDownloadFormat(Resources::DAILYMOTION))));
        m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_subtitleModel->status()
                                                              != QDailymotion::ResourcesRequest::Loading);
        break;
    case QDailymotion::StreamsRequest::Failed:
        m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        QMessageBox::critical(this, tr("Error"), m_streamModel->errorString());
        break;
    default:
        break;
    }
}

void DailymotionDownloadDialog::onSubtitleModelStatusChanged(QDailymotion::ResourcesRequest::Status status) {
    switch (status) {    
    case QDailymotion::ResourcesRequest::Loading:
        m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        return;
    case QDailymotion::ResourcesRequest::Ready:
        m_subtitleSelector->setCurrentIndex(qMax(0, m_streamModel->match("name", Settings::subtitlesLanguage())));
        break;
    case QDailymotion::ResourcesRequest::Failed:
        QMessageBox::critical(this, tr("Error"), m_subtitleModel->errorString());
        break;
    default:
        break;
    }

    m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_streamModel->status()
                                                          == QDailymotion::StreamsRequest::Ready);
}
