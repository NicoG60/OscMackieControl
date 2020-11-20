#ifndef IOMONITOR_H
#define IOMONITOR_H

#include <QObject>
#include <QPointer>
#include <QTimer>

class IOMonitor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(quint64 lastCountIn READ lastCountIn NOTIFY lastCountInChanged)
    Q_PROPERTY(quint64 lastCountOut READ lastCountOut NOTIFY lastCountOutChanged)

    Q_PROPERTY(qreal averageIn READ averageIn NOTIFY averageInChanged)
    Q_PROPERTY(qreal averageOut READ averageOut NOTIFY averageOutChanged)

    Q_PROPERTY(int averageSize READ averageSize WRITE setAverageSize NOTIFY averageSizeChanged)

public:
    explicit IOMonitor(QObject *parent = nullptr);
    IOMonitor(QTimer* timer, QObject *parent = nullptr);

    quint64 lastCountIn()  const { return _lastCountIn;  }
    quint64 lastCountOut() const { return _lastCountOut; }

    qreal averageIn()  const { return _averageIn;  }
    qreal averageOut() const { return _averageOut; }

    int averageSize() const { return _averageSize; }
    void setAverageSize(int s);

signals:
    void lastCountInChanged();
    void lastCountOutChanged();
    void averageInChanged();
    void averageOutChanged();
    void averageSizeChanged();

public slots:
    inline void countIn()  { _counterIn++;  }
    inline void countOut() { _counterOut++; }

private slots:
    void timeout();

private:
    void setupAverage();
    void pushAverageValue(quint64 in, quint64 out);
    void computeAverage();

private:
    QPointer<QTimer> _timer;

    quint64 _lastCountIn  = 0;
    quint64 _lastCountOut = 0;

    int              _averageSize = 10;
    int              _averagePos  = 0;
    QVector<quint64> _averageVecIn;
    QVector<quint64> _averageVecOut;
    qreal            _averageIn  = 0;
    qreal            _averageOut = 0;

    quint64 _counterIn  = 0;
    quint64 _counterOut = 0;
};

#endif // IOMONITOR_H
