#ifndef ECONENGINE_H
#define ECONENGINE_H

#include <QObject>

class EconEngine : public QObject
{
    Q_OBJECT
public:
    explicit EconEngine(QObject *parent = nullptr);

signals:

public slots:
};

#endif // ECONENGINE_H
