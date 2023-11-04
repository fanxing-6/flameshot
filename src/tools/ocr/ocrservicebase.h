#pragma once

#include <QObject>
#include <QPixmap>
#include <QJsonObject>

class OcrServiceBase : public QObject
{
    Q_OBJECT

public:
    explicit OcrServiceBase(QObject* parent = nullptr)
      : QObject(parent)
    {}
    virtual ~OcrServiceBase() = default;
    virtual void requestOcr(const QPixmap& pixmap) = 0;
    virtual void requestToken() = 0;
signals:
    void ocrResultReceived(const QJsonObject& result);
    void ocrError(const QString& errorMessage);
};