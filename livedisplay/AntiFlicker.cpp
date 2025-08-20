/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "AntiFlickerService"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

#include "AntiFlicker.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {

static constexpr const char* kAntiFlickerStatusPath =
        "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dimlayer_bl";

ndk::ScopedAStatus AntiFlicker::getEnabled(bool* aidl_return) {
    std::string buf;
    if (!android::base::ReadFileToString(kAntiFlickerStatusPath, &buf)) {
        LOG(ERROR) << "Failed to read " << kAntiFlickerStatusPath;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
    *aidl_return = std::stoi(android::base::Trim(buf)) == 1;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus AntiFlicker::setEnabled(bool enabled) {
    if (!android::base::WriteStringToFile((enabled ? "1" : "0"), kAntiFlickerStatusPath)) {
        LOG(ERROR) << "Failed to write " << kAntiFlickerStatusPath;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
    return ndk::ScopedAStatus::ok();
}

}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
