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

#ifndef NEWSMODEL_H
#define NEWSMODEL_H

#include <QAbstractListModel>

#include "hackernewsapi.h"

class NewsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum NewsRoles {
        IdRole = Qt::UserRole + 1,
        TitleRole = Qt::UserRole + 2,
        UrlRole = Qt::UserRole + 3
    };

    explicit NewsModel(QObject *parent = 0);
    virtual ~NewsModel();

    virtual int rowCount(const QModelIndex&) const { return backing.size(); }
    virtual QVariant data(const QModelIndex &index, int role) const;

    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void loadNewStories();
    Q_INVOKABLE void loadTopStories();

protected Q_SLOTS:
    void onItemFetched(HackerNewsAPI::Item item);
    void loadItems(QList<int> ids);

private:
    QVector<HackerNewsAPI::Item> backing;
    HackerNewsAPI* api;
    QList<int> order;
};

#endif // NEWSMODEL_H
