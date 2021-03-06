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

#include "youtubevideo.h"
#include "logger.h"
#include "resources.h"
#include "youtube.h"
#include <QDateTime>

YouTubeVideo::YouTubeVideo(QObject *parent) :
    CTVideo(parent),
    m_request(0),
    m_disliked(false),
    m_dislikeCount(0),
    m_favourite(false),
    m_favouriteCount(0),
    m_liked(false),
    m_likeCount(0)
{
    setHasSubtitles(true);
    setService(Resources::YOUTUBE);
    connect(YouTube::instance(), SIGNAL(videoDisliked(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
    connect(YouTube::instance(), SIGNAL(videoLiked(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
    connect(YouTube::instance(), SIGNAL(videoFavourited(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
    connect(YouTube::instance(), SIGNAL(videoUnfavourited(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
}

YouTubeVideo::YouTubeVideo(const QString &id, QObject *parent) :
    CTVideo(parent),
    m_request(0),
    m_disliked(false),
    m_dislikeCount(0),
    m_favourite(false),
    m_favouriteCount(0),
    m_liked(false),
    m_likeCount(0)
{
    setHasSubtitles(true);
    setService(Resources::YOUTUBE);
    loadVideo(id);
    connect(YouTube::instance(), SIGNAL(videoDisliked(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
    connect(YouTube::instance(), SIGNAL(videoLiked(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
    connect(YouTube::instance(), SIGNAL(videoFavourited(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
    connect(YouTube::instance(), SIGNAL(videoUnfavourited(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
}

YouTubeVideo::YouTubeVideo(const QVariantMap &video, QObject *parent) :
    CTVideo(parent),
    m_request(0),
    m_disliked(false),
    m_dislikeCount(0),
    m_favourite(false),
    m_favouriteCount(0),
    m_liked(false),
    m_likeCount(0)
{
    setHasSubtitles(true);
    setService(Resources::YOUTUBE);
    loadVideo(video);
    connect(YouTube::instance(), SIGNAL(videoDisliked(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
    connect(YouTube::instance(), SIGNAL(videoLiked(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
    connect(YouTube::instance(), SIGNAL(videoFavourited(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
    connect(YouTube::instance(), SIGNAL(videoUnfavourited(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
}

YouTubeVideo::YouTubeVideo(const YouTubeVideo *video, QObject *parent) :
    CTVideo(video, parent),
    m_request(0),
    m_disliked(video->isDisliked()),
    m_dislikeCount(video->dislikeCount()),
    m_favourite(video->isFavourite()),
    m_favouriteCount(video->favouriteCount()),
    m_favouriteId(video->favouriteId()),
    m_liked(video->isLiked()),
    m_likeCount(video->likeCount()),
    m_playlistItemId(video->playlistItemId())
{
    connect(YouTube::instance(), SIGNAL(videoDisliked(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
    connect(YouTube::instance(), SIGNAL(videoLiked(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
    connect(YouTube::instance(), SIGNAL(videoFavourited(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
    connect(YouTube::instance(), SIGNAL(videoUnfavourited(YouTubeVideo*)),
            this, SLOT(onVideoUpdated(YouTubeVideo*)));
}

bool YouTubeVideo::isDisliked() const {
    return m_disliked;
}

void YouTubeVideo::setDisliked(bool d) {
    if (d != isDisliked()) {
        m_disliked = d;
        emit dislikedChanged();
    }
}

qint64 YouTubeVideo::dislikeCount() const {
    return m_dislikeCount;
}

void YouTubeVideo::setDislikeCount(qint64 c) {
    if (c != dislikeCount()) {
        m_dislikeCount = c;
        emit dislikeCountChanged();
    }
}

QString YouTubeVideo::errorString() const {
    return m_request ? YouTube::getErrorString(m_request->result().toMap()) : QString();
}

bool YouTubeVideo::isFavourite() const {
    return m_favourite;
}

void YouTubeVideo::setFavourite(bool f) {
    if (f != isFavourite()) {
        m_favourite = f;
        emit favouriteChanged();
    }
}

qint64 YouTubeVideo::favouriteCount() const {
    return m_favouriteCount;
}

void YouTubeVideo::setFavouriteCount(qint64 c) {
    if (c != favouriteCount()) {
        m_favouriteCount = c;
        emit favouriteCountChanged();
    }
}

QString YouTubeVideo::favouriteId() const {
    return m_favouriteId;
}

void YouTubeVideo::setFavouriteId(const QString &i) {
    if (i != favouriteId()) {
        m_favouriteId = i;
        emit favouriteIdChanged();
    }
}

bool YouTubeVideo::isLiked() const {
    return m_liked;
}

void YouTubeVideo::setLiked(bool l) {
    if (l != isLiked()) {
        m_liked = l;
        emit likedChanged();
    }
}

qint64 YouTubeVideo::likeCount() const {
    return m_likeCount;
}

void YouTubeVideo::setLikeCount(qint64 c) {
    if (c != likeCount()) {
        m_likeCount = c;
        emit likeCountChanged();
    }
}

QString YouTubeVideo::playlistItemId() const {
    return m_playlistItemId;
}

void YouTubeVideo::setPlaylistItemId(const QString &i) {
    if (i != playlistItemId()) {
        m_playlistItemId = i;
        emit playlistItemIdChanged();
    }
}

QYouTube::ResourcesRequest::Status YouTubeVideo::status() const {
    return m_request ? m_request->status() : QYouTube::ResourcesRequest::Null;
}

void YouTubeVideo::loadVideo(const QString &id) {
    if (status() == QYouTube::ResourcesRequest::Loading) {
        return;
    }
    
    initRequest();
    
    QVariantMap filters;
    filters["id"] = id;
    
    m_request->list("/videos", QStringList() << "snippet" << "contentDetails" << "statistics", filters);
    connect(m_request, SIGNAL(finished()), this, SLOT(onVideoRequestFinished()));
    emit statusChanged(status());
}

void YouTubeVideo::loadVideo(const QVariantMap &video) {
    const QVariantMap snippet = video.value("snippet").toMap();
    const QVariantMap contentDetails = video.value("contentDetails").toMap();
    const QVariantMap statistics = video.value("statistics").toMap();
    const QVariantMap thumbnails = snippet.value("thumbnails").toMap();
    
    setDate(QDateTime::fromString(snippet.value("publishedAt").toString(), Qt::ISODate).toString("dd MMM yyyy"));
    setDescription(snippet.value("description").toString());
    setDislikeCount(statistics.value("dislikeCount").toLongLong());
    setDuration(YouTube::formatDuration(contentDetails.value("duration").toString()));
    setFavouriteCount(statistics.value("favoriteCount").toLongLong());
    setLargeThumbnailUrl(thumbnails.value("high").toMap().value("url").toString());
    setLikeCount(statistics.value("likeCount").toLongLong());
    setUserId(snippet.value("channelId").toString());
    setUsername(snippet.value("channelTitle").toString());
    setThumbnailUrl(thumbnails.value("default").toMap().value("url").toString());
    setTitle(snippet.value("title").toString());
    setViewCount(statistics.value("viewCount").toLongLong());
    
    if (video.value("kind") == "youtube#searchResult") {
        setId(video.value("id").toMap().value("videoId").toString());
    }
    else if (video.value("kind") == "youtube#playlistItem") {
        setId(snippet.value("resourceId").toMap().value("videoId").toString());
        
        if (snippet.value("playlistId") == YouTube::relatedPlaylist("favorites")) {
            setFavourite(true);
            setFavouriteId(video.value("id").toString());
        }
        else {
            setPlaylistItemId(video.value("id").toString());
            
            if (snippet.value("playlistId") == YouTube::relatedPlaylist("likes")) {
                setLiked(true);
            }
        }
    }
    else {
        setId(video.value("id").toString());
    }
    
    setUrl("https://www.youtube.com/watch?v=" + id());
}

void YouTubeVideo::loadVideo(YouTubeVideo *video) {
    CTVideo::loadVideo(video);
    setDisliked(video->isDisliked());
    setDislikeCount(video->dislikeCount());
    setFavourite(video->isFavourite());
    setFavouriteCount(video->favouriteCount());
    setFavouriteId(video->favouriteId());
    setLiked(video->isLiked());
    setLikeCount(video->likeCount());
    setPlaylistItemId(video->playlistItemId());
}
    
void YouTubeVideo::favourite() {
    if (status() == QYouTube::ResourcesRequest::Loading) {
        return;
    }
    
    Logger::log("YouTubeVideo::favourite(). ID: " + id(), Logger::MediumVerbosity);
    initRequest();
    QVariantMap resource;
    QVariantMap snippet;
    QVariantMap resourceId;
    resourceId["kind"] = "youtube#video";
    resourceId["videoId"] = id();
    snippet["playlistId"] = YouTube::relatedPlaylist("favorites");
    snippet["resourceId"] = resourceId;
    resource["snippet"] = snippet;
    
    m_request->insert(resource, "/playlistItems", QStringList() << "snippet");
    connect(m_request, SIGNAL(finished()), this, SLOT(onFavouriteRequestFinished()));
    emit statusChanged(status());
}

void YouTubeVideo::unfavourite() {
    if (status() == QYouTube::ResourcesRequest::Loading) {
        return;
    }
    
    Logger::log("YouTubeVideo::unfavourite(). ID: " + id(), Logger::MediumVerbosity);
    initRequest();
    m_request->del(favouriteId(), "/playlistItems");
    connect(m_request, SIGNAL(finished()), this, SLOT(onUnfavouriteRequestFinished()));
    emit statusChanged(status());
}

void YouTubeVideo::dislike() {
    if (status() == QYouTube::ResourcesRequest::Loading) {
        return;
    }
    
    Logger::log("YouTubeVideo::dislike(). ID: " + id(), Logger::MediumVerbosity);
    initRequest();
    QVariantMap params;
    params["id"] = id();
    params["rating"] = "dislike";
    
    m_request->insert(QVariantMap(), "/videos/rate", QStringList(), params);
    connect(m_request, SIGNAL(finished()), this, SLOT(onDislikeRequestFinished()));
    emit statusChanged(status());
}

void YouTubeVideo::like() {
    if (status() == QYouTube::ResourcesRequest::Loading) {
        return;
    }
    
    Logger::log("YouTubeVideo::like(). ID: " + id(), Logger::MediumVerbosity);
    initRequest();
    QVariantMap params;
    params["id"] = id();
    params["rating"] = "like";
    
    m_request->insert(QVariantMap(), "/videos/rate", QStringList(), params);
    connect(m_request, SIGNAL(finished()), this, SLOT(onLikeRequestFinished()));
    emit statusChanged(status());
}

void YouTubeVideo::initRequest() {
    if (!m_request) {
        m_request = new QYouTube::ResourcesRequest(this);
        m_request->setApiKey(YouTube::apiKey());
        m_request->setClientId(YouTube::clientId());
        m_request->setClientSecret(YouTube::clientSecret());
        m_request->setAccessToken(YouTube::accessToken());
        m_request->setRefreshToken(YouTube::refreshToken());
    
        connect(m_request, SIGNAL(accessTokenChanged(QString)), YouTube::instance(), SLOT(setAccessToken(QString)));
        connect(m_request, SIGNAL(refreshTokenChanged(QString)), YouTube::instance(), SLOT(setRefreshToken(QString)));
    }
}

void YouTubeVideo::onVideoRequestFinished() {
    if (m_request->status() == QYouTube::ResourcesRequest::Ready) {
        const QVariantList list = m_request->result().toMap().value("items").toList();
        
        if (!list.isEmpty()) {
            loadVideo(list.first().toMap());
        }
    }
    
    disconnect(m_request, SIGNAL(finished()), this, SLOT(onVideoRequestFinished()));
    emit statusChanged(status());
}

void YouTubeVideo::onFavouriteRequestFinished() {
    if (m_request->status() == QYouTube::ResourcesRequest::Ready) {
        setFavourite(true);
        setFavouriteCount(favouriteCount() + 1);
        setFavouriteId(m_request->result().toMap().value("id").toString());
        Logger::log("YouTubeVideo::onFavouriteRequestFinished(). Video favourited. ID: " + id(),
                    Logger::MediumVerbosity);
        emit YouTube::instance()->videoFavourited(this);
    }
    else {
        Logger::log("YouTubeVideo::onFavouriteRequestFinished(). Error: " + errorString());
    }
    
    disconnect(m_request, SIGNAL(finished()), this, SLOT(onFavouriteRequestFinished()));
    emit statusChanged(status());
}

void YouTubeVideo::onUnfavouriteRequestFinished() {
    if (m_request->status() == QYouTube::ResourcesRequest::Ready) {
        setFavourite(false);
        setFavouriteCount(favouriteCount() - 1);
        setFavouriteId(QString());
        Logger::log("YouTubeVideo::onUnfavouriteRequestFinished(). Video unfavourited. ID: " + id(),
                    Logger::MediumVerbosity);
        emit YouTube::instance()->videoUnfavourited(this);
    }
    else {
        Logger::log("YouTubeVideo::onFavouriteRequestFinished(). Error: " + errorString());
    }
    
    disconnect(m_request, SIGNAL(finished()), this, SLOT(onUnfavouriteRequestFinished()));
    emit statusChanged(status());
}

void YouTubeVideo::onLikeRequestFinished() {
    if (m_request->status() == QYouTube::ResourcesRequest::Ready) {
        setLiked(true);
        setLikeCount(likeCount() + 1);
        setDisliked(false);
        Logger::log("YouTubeVideo::onLikeRequestFinished(). Video liked. ID: " + id(), Logger::MediumVerbosity);
        emit YouTube::instance()->videoLiked(this);
    }
    else {
        Logger::log("YouTubeVideo::onLikeRequestFinished(). Error: " + errorString());
    }
    
    disconnect(m_request, SIGNAL(finished()), this, SLOT(onLikeRequestFinished()));
    emit statusChanged(status());
}

void YouTubeVideo::onDislikeRequestFinished() {
    if (m_request->status() == QYouTube::ResourcesRequest::Ready) {
        setDisliked(true);
        setDislikeCount(dislikeCount() + 1);
        setLiked(false);
        Logger::log("YouTubeVideo::onDislikeRequestFinished(). Video disliked. ID: " + id(), Logger::MediumVerbosity);
        emit YouTube::instance()->videoDisliked(this);
    }
    else {
        Logger::log("YouTubeVideo::onDislikeRequestFinished(). Error: " + errorString());
    }
    
    disconnect(m_request, SIGNAL(finished()), this, SLOT(onDislikeRequestFinished()));
    emit statusChanged(status());
}

void YouTubeVideo::onVideoUpdated(YouTubeVideo *video) {
    if ((video->id() == id()) && (video != this)) {
        loadVideo(video);
    }
}
