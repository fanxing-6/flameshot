#include "ocrwidget.h"

#include "ocrservicefactory.h"

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


OcrWidget::OcrWidget(const QPixmap& capture, QWidget* parent)
    : QWidget(parent), m_pixmap(capture)
{
    // ���ô��ڱ���
    setWindowTitle(tr("OCR"));
    // ���ô��ڴ�С�̶�
    setFixedSize(1200, 600); // ����Ϊ����Ҫ�Ĵ�С
    // ��������
    m_vLayout = new QVBoxLayout(this);
    m_hLayout = new QHBoxLayout;
    m_graphicsView = new QGraphicsView(this);
    m_textEdit = new QTextEdit(this);
    // �����ı��༭��Ϊֻ��
    m_textEdit->setReadOnly(true);

    // ���������÷ָ���
    m_splitter = new QSplitter(this);
    m_splitter->addWidget(m_graphicsView);
    m_splitter->addWidget(m_textEdit);
    m_splitter->setSizes(QList<int>() << 500 << 300); // ���÷ָ����ĳ�ʼ��С
    m_hLayout->addWidget(m_splitter);

    // ���ð�ť������
    m_zoomInButton = new QPushButton("Zoom In", this);
    m_zoomOutButton = new QPushButton("Zoom Out", this);
    m_translateButton = new QPushButton("Translate", this);
    connect(m_zoomInButton, &QPushButton::clicked, this, &OcrWidget::zoomIn);
    connect(m_zoomOutButton, &QPushButton::clicked, this, &OcrWidget::zoomOut);
    connect(m_translateButton,
            &QPushButton::clicked,
            this,
            &OcrWidget::translateText);
    m_btnLayout = new QHBoxLayout;
    m_btnLayout->addWidget(m_zoomInButton);
    m_btnLayout->addWidget(m_zoomOutButton);
    m_btnLayout->addWidget(m_translateButton);

    // ��������ӵ�����
    m_vLayout->addLayout(m_hLayout);
    m_vLayout->addLayout(m_btnLayout);
    setLayout(m_vLayout);

    // ����ͼƬ
    QGraphicsScene* scene = new QGraphicsScene(this);

    m_pixmapItem = scene->addPixmap(m_pixmap);

    // m_pixmapItem->setPos(0, 0);

    m_pixmapItem->setTransformationMode(Qt::SmoothTransformation);

    m_graphicsView->setScene(scene);

    m_graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QTimer::singleShot(0, this, &OcrWidget::performOcr);
}

void OcrWidget::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    adjustPixmapItemSize(); // ����ͼƬ�ߴ�
}


void OcrWidget::adjustPixmapItemSize()
{
    if (!m_graphicsView || !m_pixmapItem)
        return;

    m_graphicsView->fitInView(m_pixmapItem, Qt::KeepAspectRatio);
}

void OcrWidget::performOcr()
{
    OcrServiceBase* ocrService =
        OcrServiceFactory::createOcrService(OcrServiceFactory::Huawei,
                                            this);
    ocrService->requestOcr(m_pixmap);
}

void OcrWidget::zoomIn()
{
    m_graphicsView->scale(1.2, 1.2);
}

void OcrWidget::zoomOut()
{
    m_graphicsView->scale(1 / 1.2, 1 / 1.2);
}

void OcrWidget::translateText()
{
    // ������÷���API������OCR���
}
