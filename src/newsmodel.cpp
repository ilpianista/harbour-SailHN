/*
  The MIT License (MIT)

  Copyright (c) 2015-2016 Andrea Scarpino <me@andreascarpino.it>

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
#include <QEventLoop>

#include "item.h"
#include "hackernewsapi.h"

const static int MAX_ITEMS = 30;

NewsModel::NewsModel(QObject *parent) :
    QAbstractListModel(parent)
  , api(new HackerNewsAPI(this))
  , m_start(0), m_end(MAX_ITEMS)
{
    connect(api, &HackerNewsAPI::storiesFetched, this, &NewsModel::onStoriesFetched);
}

NewsModel::~NewsModel()
{
    disconnect(api, &HackerNewsAPI::itemFetched, this, &NewsModel::onItemFetched);

    if (!backing.isEmpty()) {
        qDeleteAll(backing);
        backing.clear();
    }

    delete api;
}

QHash<int, QByteArray> NewsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[ByRole] = "by";
    roles[DeadRole] = "dead";
    roles[DescendantsRole] = "descendants";
    roles[KidsRole] = "kids";
    roles[ParentRole] = "parent";
    roles[ScoreRole] = "score";
    roles[TextRole] = "itemText";
    roles[TimeRole] = "time";
    roles[TitleRole] = "title";
    roles[UrlRole] = "url";
    return roles;
}

void NewsModel::loadAskStories()
{
    reset();
    api->getStories(HackerNewsAPI::Ask);
}

void NewsModel::loadNewStories()
{
    reset();
    api->getStories(HackerNewsAPI::New);
}

void NewsModel::loadJobStories()
{
    reset();
    api->getStories(HackerNewsAPI::Job);
}

void NewsModel::loadShowStories()
{
    reset();
    api->getStories(HackerNewsAPI::Show);
}

void NewsModel::loadTopStories()
{
    reset();
    api->getStories(HackerNewsAPI::Top);
}

void NewsModel::refresh(const int itemId)
{
    api->getItem(itemId);

    QEventLoop loop;
    connect(api, &HackerNewsAPI::itemFetched, &loop, &QEventLoop::quit);
    connect(api, &HackerNewsAPI::itemFetched, this, &NewsModel::onRefreshComments);
    loop.exec();
}

void NewsModel::onRefreshComments(Item *item)
{
    disconnect(api, &HackerNewsAPI::itemFetched, this, &NewsModel::onRefreshComments);

    loadComments(item->kids());
}

void NewsModel::loadComments(const QList<int> kids)
{
    reset();
    m_ids = kids;
    loadItems();
}

void NewsModel::nextItems()
{
    if (m_end < m_ids.size()) {
        m_start = m_end;
        m_end += MAX_ITEMS;

        if (m_end >= m_ids.size()) {
            m_end = -1;
        }

        loadItems();
    }
}

QVariant NewsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    Item *item = backing[index.row()];
    switch (role) {
        case IdRole: return item->id();
        case ByRole: return item->by();
        case DeadRole: return item->dead();
        case DescendantsRole: return item->descendants();
        case KidsRole: {
            QVariantList kids;
            Q_FOREACH (const int kid, item->kids()) {
                kids.append(kid);
            }
            return kids;
        }
        case ParentRole: return item->parent();
        case ScoreRole: return item->score();
        case TextRole: return item->text();
        case TimeRole: return item->time();
        case TitleRole: return item->title();
        case UrlRole: return item->url();
        default: qCritical() << "Unrecognized role" << role;
    }

    return QVariant();
}

void NewsModel::onItemFetched(Item *item)
{
    if (!item->deleted()) {
        beginInsertRows(QModelIndex(), backing.size(), backing.size());
        backing.append(item);
        endInsertRows();
    }
}

void NewsModel::onStoriesFetched(QList<int> ids)
{
    m_ids = ids;
    loadItems();
}

void NewsModel::reset()
{
    disconnect(api, &HackerNewsAPI::itemFetched, this, &NewsModel::onItemFetched);

    if (!backing.isEmpty()) {
        beginResetModel();
        qDeleteAll(backing);
        backing.clear();
        endResetModel();
    }

    m_start = 0;
    m_end = MAX_ITEMS;
    m_ids.clear();

    connect(api, &HackerNewsAPI::itemFetched, this, &NewsModel::onItemFetched);
}

void NewsModel::loadItems()
{
    QList<int> limited = m_ids.mid(m_start, m_end);
    Q_FOREACH (const int id, limited) {
        api->getItem(id);
    }
}
