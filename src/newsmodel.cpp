/*
  The MIT License (MIT)

  Copyright (c) 2015 Andrea Scarpino <me@andreascarpino.it>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "newsmodel.h"

#include <QDebug>

const static int MAX_ITEMS = 30;

NewsModel::NewsModel(QObject *parent) :
    QAbstractListModel(parent)
  , api(new HackerNewsAPI(this))
{
}

NewsModel::~NewsModel()
{
    delete api;
}

QHash<int, QByteArray> NewsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[TimeRole] = "time";
    roles[TitleRole] = "title";
    roles[UrlRole] = "url";
    return roles;
}

void NewsModel::loadNewStories()
{
    api->getNewStories();
    connect(api, SIGNAL(multipleStoriesFetched(QList<int>)), this, SLOT(loadItems(QList<int>)));
}

void NewsModel::loadTopStories()
{
    api->getTopStories();
    connect(api, SIGNAL(multipleStoriesFetched(QList<int>)), this, SLOT(loadItems(QList<int>)));
}

QVariant NewsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    HackerNewsAPI::Item item = backing[index.row()];
    switch (role) {
    case IdRole: return item.id;
    case TimeRole: return item.time;
    case TitleRole: return item.title;
    case UrlRole: return item.url;
    default: qCritical() << "Role not recognized";
    }

    return QVariant();
}

void NewsModel::onItemFetched(HackerNewsAPI::Item item)
{
    beginInsertRows(QModelIndex(), backing.size(), backing.size());
    backing.append(item);
    endInsertRows();
}

void NewsModel::loadItems(QList<int> ids)
{
    connect(api, SIGNAL(itemFetched(HackerNewsAPI::Item)), this, SLOT(onItemFetched(HackerNewsAPI::Item)));

    beginResetModel();
    backing.clear();
    endResetModel();

    QList<int> limited = ids.mid(0, MAX_ITEMS);
    Q_FOREACH (const int id, limited) {
        api->getItem(id);
    }
}
