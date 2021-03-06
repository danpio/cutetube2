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

#ifndef YOUTUBEVIEW_H
#define YOUTUBEVIEW_H

#include <QWidget>

class YouTubeUser;
class YouTubeVideo;
class YouTubePlaylist;
class YouTubeNavModel;
class ListView;
class QModelIndex;
class QVBoxLayout;

class YouTubeView : public QWidget
{
    Q_OBJECT
    
public:
    explicit YouTubeView(QWidget *parent = 0);
    
public Q_SLOTS:
    void search(const QString &query, const QString &type, const QString &order);
    void showResource(const QString &type, const QString &id);
    
private:
    void showAccounts();
    void showCategories();
    void showFavourites();
    void showLatestVideos();
    void showLikes();
    void showPlaylists();
    void showSearchDialog();
    void showSubscriptions();
    void showUploads();

private Q_SLOTS:
    void onItemActivated(const QModelIndex &index);
    
    void onCommentAdded();
    
    void onUserSubscribed(YouTubeUser *user);
    void onUserUnsubscribed(YouTubeUser *user);
    
    void onVideoFavourited(YouTubeVideo *video);
    void onVideoUnfavourited(YouTubeVideo *video);
    void onVideoLiked(YouTubeVideo *video);
    void onVideoDisliked(YouTubeVideo *video);
    void onVideoAddedToPlaylist(YouTubeVideo *video, YouTubePlaylist *playlist);
    
private:
    YouTubeNavModel *m_model;
    
    ListView *m_view;
    QVBoxLayout *m_layout;
};

#endif // YOUTUBEVIEW_H
