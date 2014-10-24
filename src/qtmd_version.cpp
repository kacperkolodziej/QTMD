#include "qtmd_version.hpp"
#include <cstdint>

static const uint16_t major_version = 0;
static const uint16_t minor_version = 2;
static const uint16_t patch_version = 0;
static const QString info_version = "beta";

QString version_qstring()
{
    return QString("v%1.%2.%3-%4")
            .arg(major_version)
            .arg(minor_version)
            .arg(patch_version)
            .arg(info_version);
}
