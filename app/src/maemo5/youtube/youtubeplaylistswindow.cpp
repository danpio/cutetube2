/*
 * Copyright (C) 2015 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "youtubeplaylistswindow.h"
#include "imagecache.h"
#include "listview.h"
#include "playlistdelegate.h"
#include "settings.h"
#include "youtubeplaylistwindow.h"
#include <QLabel>
#include <QActionGroup>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QMenuBar>

YouTubePlaylistsWindow::YouTubePlaylistsWindow(StackedWindow *parent) :
    StackedWindow(parent),
    m_model(new YouTubePlaylistModel(this)),
    m_cache(new ImageCache),
    m_view(new ListView(this)),
    m_delegate(new PlaylistDelegate(m_cache, YouTubePlaylistModel::DateRole, YouTubePlaylistModel::ThumbnailUrlRole,
                                    YouTubePlaylistModel::TitleRole, YouTubePlaylistModel::UsernameRole,
                                    YouTubePlaylistModel::VideoCountRole, m_view)),
    m_viewGroup(new QActionGroup(this)),
    m_listAction(new QAction(tr("List"), this)),
    m_gridAction(new QAction(tr("Grid"), this)),
    m_reloadAction(new QAction(tr("Reload"), this)),
    m_label(new QLabel(QString("<p align='center'; style='font-size: 40px; color: %1'>%2</p>")
                              .arg(palette().color(QPalette::Mid).name()).arg(tr("No playlists found")), this))
{
    setWindowTitle(tr("Playlists"));
    setCentralWidget(new QWidget);
    
    m_view->setModel(m_model);
    m_view->setItemDelegate(m_delegate);
    m_listAction->setCheckable(true);
    m_listAction->setChecked(true);
    m_gridAction->setCheckable(true);
    m_viewGroup->setExclusive(true);
    m_viewGroup->addAction(m_listAction);
    m_viewGroup->addAction(m_gridAction);
    m_reloadAction->setEnabled(false);
    
    m_label->hide();
    
    m_layout = new QVBoxLayout(centralWidget());
    m_layout->addWidget(m_view);
    m_layout->addWidget(m_label);
    m_layout->setContentsMargins(0, 0, 0, 0);
    
    menuBar()->addAction(m_listAction);
    menuBar()->addAction(m_gridAction);
    menuBar()->addAction(m_reloadAction);
    
    connect(m_model, SIGNAL(statusChanged(QYouTube::ResourcesRequest::Status)), this,
            SLOT(onModelStatusChanged(QYouTube::ResourcesRequest::Status)));
    connect(m_cache, SIGNAL(imageReady()), this, SLOT(onImageReady()));
    connect(m_view, SIGNAL(activated(QModelIndex)), this, SLOT(showPlaylist(QModelIndex)));
    connect(m_listAction, SIGNAL(triggered()), this, SLOT(enableListMode()));
    connect(m_gridAction, SIGNAL(triggered()), this, SLOT(enableGridMode()));
    connect(m_reloadAction, SIGNAL(triggered()), m_model, SLOT(reload()));
    
    if (Settings::instance()->defaultViewMode() == "grid") {
        m_gridAction->trigger();
    }
}

YouTubePlaylistsWindow::~YouTubePlaylistsWindow() {
    delete m_cache;
    m_cache = 0;
}

void YouTubePlaylistsWindow::list(const QString &resourcePath, const QStringList &part, const QVariantMap &filters,
                                  const QVariantMap &params) {
    m_model->list(resourcePath, part, filters, params);
}

void YouTubePlaylistsWindow::enableGridMode() {
    m_delegate->setGridMode(true);
    m_view->setFlow(QListView::LeftToRight);
    m_view->setWrapping(true);
    Settings::instance()->setDefaultViewMode("grid");
}

void YouTubePlaylistsWindow::enableListMode() {
    m_delegate->setGridMode(false);
    m_view->setFlow(QListView::TopToBottom);
    m_view->setWrapping(false);
    Settings::instance()->setDefaultViewMode("list");
}

void YouTubePlaylistsWindow::showPlaylist(const QModelIndex &index) {
    if (const YouTubePlaylist *playlist = m_model->get(index.row())) {
        YouTubePlaylistWindow *window = new YouTubePlaylistWindow(playlist, this);
        window->show();
    }
}

void YouTubePlaylistsWindow::onImageReady() {
    m_view->viewport()->update(m_view->viewport()->rect());
}

void YouTubePlaylistsWindow::onModelStatusChanged(QYouTube::ResourcesRequest::Status status) {
    switch (status) {
    case QYouTube::ResourcesRequest::Loading:
        showProgressIndicator();
        m_label->hide();
        m_view->show();
        m_reloadAction->setEnabled(false);
        return;
    case QYouTube::ResourcesRequest::Failed:
        QMessageBox::critical(this, tr("Error"), m_model->errorString());
        break;
    default:
        break;
    }
    
    hideProgressIndicator();
    m_reloadAction->setEnabled(true);
    
    if (m_model->rowCount() == 0) {
        m_view->hide();
        m_label->show();
    }
}
