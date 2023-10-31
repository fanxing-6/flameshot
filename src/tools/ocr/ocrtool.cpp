#include "ocrtool.h"

OcrTool::OcrTool(QObject* parent)
    : AbstractActionTool(parent)
{

}

CaptureTool::Type OcrTool::type() const
{
    return CaptureTool::TYPE_OCR;
}

QString OcrTool::description() const
{
    return tr("ocr");
}

QIcon OcrTool::icon(const QColor& background, bool inEditor) const
{
    Q_UNUSED(inEditor);
    // TODO£ºÐÞ¸ÄÍ¼±ê
    return QIcon(iconPath(background) + "cloud-upload.svg");
}

OcrTool::~OcrTool()
{
}

QString OcrTool::name() const
{
    return tr("OCR");
}

bool OcrTool::closeOnButtonPressed() const
{
    return true;
}

CaptureTool* OcrTool::copy(QObject* parent)
{
    return new OcrTool(parent);
}

void OcrTool::pressed(CaptureContext& context)
{

    emit requestAction(REQ_CLEAR_SELECTION);
    emit requestAction(REQ_CAPTURE_DONE_OK);
    context.request.addTask(CaptureRequest::OCR);
    emit requestAction(REQ_CLOSE_GUI);
}
