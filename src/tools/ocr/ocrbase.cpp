#include "ocrbase.h"

#include <QClipboard>
#include <QCursor>
#include <QDesktopServices>
#include <QDrag>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMimeData>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QRect>
#include <QScreen>
#include <QShortcut>
#include <QTimer>
#include <QUrlQuery>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QSplitter>


OcrBase::OcrBase(const QPixmap& capture, QWidget* parent)
    : QWidget(parent), m_pixmap(capture)
{
    setWindowTitle(tr("OCR"));

    m_vLayout = new QVBoxLayout(this);
    m_hLayout = new QHBoxLayout;
    m_btnLayout = new QHBoxLayout;

    m_graphicsView = new QGraphicsView(this);
    m_pixmapItem = new QGraphicsPixmapItem(m_pixmap);
    m_graphicsView->setScene(new QGraphicsScene(this));
    m_graphicsView->scene()->addItem(m_pixmapItem);
    m_graphicsView->setRenderHint(QPainter::Antialiasing);

    m_textEdit = new QTextEdit(this);

    // Assuming you want the OCR results to be read-only
    m_textEdit->setReadOnly(true);

    m_zoomInButton = new QPushButton("ZoomIn", this);
    m_zoomOutButton = new QPushButton("ZoomOut", this);
    m_translateButton = new QPushButton("translate", this);

    connect(m_zoomInButton, &QPushButton::clicked, this, &OcrBase::zoomIn);
    connect(m_zoomOutButton, &QPushButton::clicked, this, &OcrBase::zoomOut);
    connect(m_translateButton,
            &QPushButton::clicked,
            this,
            &OcrBase::translateText);
    m_hLayout->addWidget(m_graphicsView);
    m_hLayout->addWidget(m_textEdit);
    m_btnLayout->addWidget(m_zoomInButton);
    m_btnLayout->addWidget(m_zoomOutButton);
    m_btnLayout->addWidget(m_translateButton);

    m_vLayout->addLayout(m_hLayout);
    m_vLayout->addLayout(m_btnLayout);

    setLayout(m_vLayout);

}


void OcrBase::zoomIn()
{
    m_graphicsView->scale(1.2, 1.2);
}

void OcrBase::zoomOut()
{
    m_graphicsView->scale(1 / 1.2, 1 / 1.2);
}

void OcrBase::translateText()
{
    // 这里调用翻译API来翻译OCR结果
}
