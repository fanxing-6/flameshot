#pragma once

#include <QString>
#include <QIcon>
#include "src/tools/abstractactiontool.h"

class OcrTool : public AbstractActionTool
{
    Q_OBJECT

public:
    explicit OcrTool(QObject* parent = nullptr);

    ~OcrTool() override;

    QIcon icon(const QColor& background, bool inEditor) const override;

    QString name() const override;

    QString description() const override;

    CaptureTool::Type type() const override;

    bool closeOnButtonPressed() const override;

    CaptureTool* copy(QObject* parent) override;

public slots:
    void pressed(CaptureContext& context) override;

private:

};
