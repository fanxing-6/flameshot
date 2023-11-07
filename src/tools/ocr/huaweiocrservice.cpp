#include "HuaweiOcrService.h"

#include <QBuffer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QJsonArray>

HuaweiOcrService::HuaweiOcrService(QObject* parent)
    : OcrServiceBase(parent)
{
    m_networkManager = new QNetworkAccessManager(this);

    QObject::connect(this,
                     &HuaweiOcrService::tokenRequestFailed,
                     this,
                     &HuaweiOcrService::onTokenRequestFailed);

    QObject::connect(this,
                     &HuaweiOcrService::ocrRequestFailed,
                     this,
                     &HuaweiOcrService::onOcrRequestFailed);

}

HuaweiOcrService::~HuaweiOcrService()
{
}

void HuaweiOcrService::requestToken()
{
    // if (isTokenValid()) {
    //     // emit tokenReceived(m_currentToken);
    //     return;
    // }

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

    QByteArray data = doc.toJson();

    m_tokenReply = m_networkManager->post(request, data);

    QByteArray tokenResponseData = m_tokenReply->readAll();
    qDebug() << "Response Data :" << tokenResponseData;

    QObject::connect(m_tokenReply,
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
    if (m_tokenReply->error() == QNetworkReply::NoError) {
        // 从响应头部中获取Token
        QString token = m_tokenReply->rawHeader("X-Subject-Token");
        setToken(token);
        emit tokenReceived(token);
    } else {
        emit tokenRequestFailed(m_tokenReply->errorString());
    }
    m_tokenReply->deleteLater();
}

void HuaweiOcrService::requestOcr(const QPixmap& pixmap)
{
    if (!isTokenValid()) {
        QObject::connect(this,
                         &HuaweiOcrService::tokenReceived,
                         this,
                         [this, pixmap]() { performOcrRequest(pixmap); });

        QObject::connect(this,
                         &HuaweiOcrService::tokenRequestFailed,
                         this,
                         [this](const QString error) {
                             onTokenRequestFailed(error);
                         });

        requestToken();
    } else {
        performOcrRequest(pixmap);
    }

}

void HuaweiOcrService::performOcrRequest(const QPixmap pixmap)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    // pixmap.save(&buffer, "PNG");
    pixmap.save(&buffer, "PNG");
    QByteArray base64Data = byteArray.toBase64();

    QJsonObject json;
    json.insert("image", QString::fromLatin1(base64Data));
    json.insert("character_mode", true);


    QJsonDocument doc(json);
    QByteArray jsonData = doc.toJson();

    QNetworkRequest request;
    QString url = QString("https://ocr.cn-north-4.myhuaweicloud.com/v2/"
        "0c0486f81a800f2a2f0cc00040f100f6/ocr/general-text");
    // TODO:改成这种形式：QString("https://%1/v2/%2/ocr/general-table").arg(endpoint).arg(project_id);

    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    request.setRawHeader("X-Auth-Token", m_currentToken.toUtf8());

    // 获取并打印所有的头部字段
    QList<QByteArray> headerList = request.rawHeaderList();
    foreach(QByteArray header, headerList) {
        qDebug() << header << ":" << request.rawHeader(header);
    }

    m_OcrReply = m_networkManager->post(request, jsonData);

    QObject::connect(m_OcrReply,
                     &QNetworkReply::finished,
                     this,
                     &HuaweiOcrService::handleOcrResponse);
}


void HuaweiOcrService::handleOcrResponse()
{
    if (m_OcrReply->error() == QNetworkReply::NoError) {
        qDebug() << m_OcrReply->readAll();

    } else {
        emit ocrRequestFailed(m_OcrReply->errorString());
    }
    m_OcrReply->deleteLater();
}

void HuaweiOcrService::onTokenRequestFailed(const QString errorMessage)
{
    qDebug()
        << "HuaweiOcrService::onTokenRequestFailed(const QString errorMessage): "
        << errorMessage;
}


void HuaweiOcrService::onOcrRequestFailed(const QString errorMessage)
{
    qDebug()
        << "HuaweiOcrService::onOcrRequestFailed(const QString errorMessage) :"
        << errorMessage;

}
