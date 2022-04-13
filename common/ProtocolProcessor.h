#pragma once

#include "UTenProtocol.h"

template <typename T>
class ProtocolPackage {
public:
    T& pkg;
};

class ProtocolProcessor {
public:
    ProtocolProcessor();

    ~ProtocolProcessor();

    virtual void handlePackage();

    virtual void onReportInsider();
};