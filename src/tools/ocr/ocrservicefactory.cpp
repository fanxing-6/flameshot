#include "ocrservicefactory.h"
#include "huaweiocrservice.h"


OcrServiceBase* OcrServiceFactory::createOcrService(OcrServiceProvider provider,
                                                    QObject* parent)
{
    switch (provider) {
        case Azure:
            return nullptr;
        case AWS:
        // return new AwsOcrService(parent);
        case Huawei:
            return new HuaweiOcrService(parent);
        case Ali:
            // return new AliOcrService(parent);
        default:
            return nullptr;
    }
}
