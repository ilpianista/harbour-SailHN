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

const static int MAX_ITEMS = 30;

NewsModel::NewsModel(QObject *parent) :
    QAbstractListModel(parent)
  , api(new HackerNewsAPI(this))
{
    api->getTopStories();
    connect(api, SIGNAL(multipleStoriesFetched(QVariantList)), this, SLOT(loadItems(QVariantList)));
}

NewsModel::~NewsModel()
{
    delete api;
}

QHash<int, QByteArray> NewsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    return roles;
}

QVariant NewsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (roleNames().contains(role)) {
        return backing[index.row()].value(roleNames().value(role));
    }

    return QVariant();
}

void NewsModel::onItemFetched(QVariantMap item)
{
    beginInsertRows(QModelIndex(), backing.size(), backing.size() + 1);
    backing.append(item);
    endInsertRows();
}

void NewsModel::loadItems(QVariantList ids)
{
    connect(api, SIGNAL(itemFetched(QVariantMap)), this, SLOT(onItemFetched(QVariantMap)));

    QVariantList limited = ids.mid(0, MAX_ITEMS);
    Q_FOREACH (const QVariant id, limited) {
        api->getItem(id.toInt());
    }
}
