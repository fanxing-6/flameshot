#include "HuaweiOcrService.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QJsonArray>

HuaweiOcrService::HuaweiOcrService(QObject* parent)
    : OcrServiceBase(parent)
{
    QObject::connect(this,
                     &HuaweiOcrService::tokenRequestFailed,
                     this,
                     &HuaweiOcrService::onTokenRequestFailed);
}

HuaweiOcrService::~HuaweiOcrService()
{
}

void HuaweiOcrService::requestToken()
{
    if (isTokenValid()) {
        emit tokenReceived(m_currentToken);
        return;
    }
    QNetworkRequest request(
        QUrl("https://iam.cn-north-4.myhuaweicloud.com/v3/auth/tokens"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/json;charset=utf8");

     QJsonObject topLevelObject;


    QJsonObject identityObject;
    QJsonArray methodsArray;
    methodsArray.append("hw_ak_sk");
    identityObject["methods"] = methodsArray;


    QJsonObject hw_ak_skObject;
    QJsonObject accessObject;
    accessObject["key"] = "MVIEW9Q8RWLAGGAIEZUP";
    QJsonObject secretObject;
    secretObject["key"] = "fYhQZOCzB4tLtk8Lwb5sTyyWnVO5IjRQl2ZcfbL9";
    hw_ak_skObject["access"] = accessObject;
    hw_ak_skObject["secret"] = secretObject;
    identityObject["hw_ak_sk"] = hw_ak_skObject;

    QJsonObject scopeObject;
    QJsonObject projectObject;
    projectObject["name"] = "cn-north-4";
    scopeObject["project"] = projectObject;


    QJsonObject authObject;
    authObject["identity"] = identityObject;
    authObject["scope"] = scopeObject;
    topLevelObject["auth"] = authObject;

    QJsonDocument doc(topLevelObject);
    qDebug() << doc;
    QByteArray data = doc.toJson();

    request.setAttribute(QNetworkRequest::Http2AllowedAttribute, false);
    m_reply = m_networkManager.post(request, data);
    connect(m_reply,
            &QNetworkReply::finished,
            this,
            &HuaweiOcrService::handleTokenResponse);
}

bool HuaweiOcrService::isTokenValid() const
{

    return !m_currentToken.isEmpty() &&
           QDateTime::currentDateTime() < m_tokenExpiryTime;
}

void HuaweiOcrService::setToken(const QString& token)
{
    qDebug() << "void HuaweiOcrService::setToken(const QString& token):"
        << token;

    m_currentToken = token;
    m_tokenExpiryTime = QDateTime::currentDateTime().addSecs(12 * 60 * 60);
}

void HuaweiOcrService::handleTokenResponse()
{
    if (m_reply->error() == QNetworkReply::NoError) {
        // 从响应头部中获取Token
        QString token = m_reply->rawHeader("X-Subject-Token");
        setToken(token);
        emit tokenReceived(token);
    } else {
        emit tokenRequestFailed(m_reply->errorString());
    }
    m_reply->deleteLater();
}

void HuaweiOcrService::requestOcr(const QPixmap& pixmap)
{
}

void HuaweiOcrService::onTokenRequestFailed(const QString errorMessage)
{
    qDebug()
        << "HuaweiOcrService::onTokenRequestFailed(const QString errorMessage): "
        << errorMessage;
}
