#pragma once

#include "ocrservicebase.h"
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

class HuaweiOcrService : public OcrServiceBase
{
    Q_OBJECT

public:
    explicit HuaweiOcrService(QObject* parent = nullptr);
    ~HuaweiOcrService();


    void requestToken() override;
    bool isTokenValid() const;

    void requestOcr(const QPixmap& pixmap) override;

signals:
    void tokenReceived(QString token);
    void tokenRequestFailed(QString errorMessage);
    void ocrRequestFailed(QString errorMessage);

private slots:
    void handleTokenResponse();
    void handleOcrResponse();

    void onTokenRequestFailed(const QString errorMessage);
    void onOcrRequestFailed(const QString errorMessage);
    void performOcrRequest(const QPixmap pixmap);

private:
    QNetworkAccessManager* m_networkManager;
    QString m_currentToken;
    QDateTime m_tokenExpiryTime;
    QNetworkReply* m_tokenReply;
    QNetworkReply* m_OcrReply;

    void setToken(const QString& token);
};
