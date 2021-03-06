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

#ifndef PLUGINUSER_H
#define PLUGINUSER_H

#include "user.h"
#include "resourcesrequest.h"

class PluginUser : public CTUser
{
    Q_OBJECT
    
    Q_PROPERTY(QString playlistsId READ playlistsId NOTIFY playlistsIdChanged)
    Q_PROPERTY(QString videosId READ videosId NOTIFY videosIdChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY statusChanged)
    Q_PROPERTY(ResourcesRequest::Status status READ status NOTIFY statusChanged)

public:
    explicit PluginUser(QObject *parent = 0);
    explicit PluginUser(const QString &service, const QString &id, QObject *parent = 0);
    explicit PluginUser(const QString &service, const QVariantMap &user, QObject *parent = 0);
    explicit PluginUser(const PluginUser *user, QObject *parent = 0);
    
    QString playlistsId() const;
    QString videosId() const;
    
    QString errorString() const;
        
    ResourcesRequest::Status status() const;
    
    Q_INVOKABLE void loadUser(const QString &service, const QString &id);
    Q_INVOKABLE void loadUser(const QString &service, const QVariantMap &user);
    Q_INVOKABLE void loadUser(PluginUser *user);

protected:
    void setPlaylistsId(const QString &i);
    void setVideosId(const QString &i);

private Q_SLOTS:
    void onRequestFinished();
    
Q_SIGNALS:
    void playlistsIdChanged();
    void videosIdChanged();
    void statusChanged(ResourcesRequest::Status s);

private:
    ResourcesRequest* request();
    
    ResourcesRequest *m_request;
    
    QString m_playlistsId;
    QString m_videosId;
};

#endif // PLUGINUSER_H
