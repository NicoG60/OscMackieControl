#include "iomonitor.h"

IOMonitor::IOMonitor(QObject *parent) :
    QObject(parent),
    _timer(new QTimer(this))
{
    setupAverage();

    _timer->setInterval(1000);
    connect(_timer, &QTimer::timeout, this, &IOMonitor::timeout);
    _timer->start();
}

IOMonitor::IOMonitor(QTimer* timer, QObject *parent) :
    QObject(parent),
    _timer(timer)
{
    setupAverage();

    connect(_timer, &QTimer::timeout, this, &IOMonitor::timeout);
}

void IOMonitor::setAverageSize(int s)
{
    if(s <= 0 || s == _averageSize)
        return;

    _averageSize = s;
    setupAverage();

    emit averageSizeChanged();
}

void IOMonitor::timeout()
{
    _lastCountIn  = _counterIn;
    _lastCountOut = _counterOut;

    _counterIn  = 0;
    _counterOut = 0;

    emit lastCountInChanged();
    emit lastCountOutChanged();

    pushAverageValue(_lastCountIn, _lastCountOut);
    computeAverage();
    emit updated();
}

void IOMonitor::setupAverage()
{
    _averageVecIn.resize(_averageSize);
    _averageVecIn.fill(0);

    _averageVecOut.resize(_averageSize);
    _averageVecOut.fill(0);

    _averagePos = 0;

    _averageIn  = 0;
    _averageOut = 0;

    emit averageInChanged();
    emit averageOutChanged();
}

void IOMonitor::pushAverageValue(quint64 in, quint64 out)
{
    _averageVecIn[_averagePos]  = in;
    _averageVecOut[_averagePos] = out;

    _averagePos++;
    _averagePos %= _averageSize;
}

void IOMonitor::computeAverage()
{
    _averageIn = std::accumulate(_averageVecIn.begin(), _averageVecIn.end(), 0);
    _averageIn /= _averageSize;

    _averageOut = std::accumulate(_averageVecOut.begin(), _averageVecOut.end(), 0);
    _averageOut /= _averageSize;

    emit averageInChanged();
    emit averageOutChanged();
}
