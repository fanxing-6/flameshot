#pragma once

#include <QGraphicsView>
#include <QUrl>
#include <QWidget>
#include <QObject>
#include <QSplitter>
#include <QTextEdit>
#include <QVBoxLayout>


class QPushButton;

class OcrWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OcrWidget(const QPixmap& capture, QWidget* parent = nullptr);

protected:
    // void resizeEvent(QResizeEvent* event) override;

private slots:
    void zoomIn();
    void zoomOut();
    void translateText();

private:
    QPixmap m_pixmap;
    QVBoxLayout* m_vLayout;
    QHBoxLayout* m_hLayout;
    QHBoxLayout* m_btnLayout;
    QGraphicsView* m_graphicsView;
    QGraphicsPixmapItem* m_pixmapItem;
    QTextEdit* m_textEdit;
    QPushButton* m_zoomInButton;
    QPushButton* m_zoomOutButton;
    QPushButton* m_translateButton;
    QSplitter* m_splitter;

private:
    void adjustPixmapItemSize();

    void performOcr();

protected:
    void showEvent(QShowEvent* event) override;

};
