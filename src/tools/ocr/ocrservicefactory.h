#pragma once

#include "OcrServiceBase.h"

class OcrServiceFactory
{
public:
    enum OcrServiceProvider
    {
        Azure,
        AWS,
        Huawei,
        Ali
    };

    static OcrServiceBase* createOcrService(OcrServiceProvider provider,
                                            QObject* parent = nullptr);
};