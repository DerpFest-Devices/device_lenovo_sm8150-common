/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "FingerprintInscreenService"

#include "FingerprintInscreen.h"

#include <unistd.h>
#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>
#include <fstream>
#include <cmath>
#include <thread>

#define CMD_FINGERPRINT_EVENT 10

#define DIMLAYER_HBM_PATH "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dimlayer_hbm"
#define DC_DIM_PATH "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dimlayer_bl"
#define DOZE_MODE_PATH "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/doze_status"

namespace vendor {
namespace lineage {
namespace biometrics {
namespace fingerprint {
namespace inscreen {
namespace V1_0 {
namespace implementation {

bool dcDimState;

/*
 * Write value to path and close file.
 */
template <typename T>
static void set(const std::string& path, const T& value) {
    std::ofstream file(path);
    file << value;
    LOG(INFO) << "wrote path: " << path << ", value: " << value << "\n";
}

template <typename T>
static T get(const std::string& path, const T& def) {
    std::ifstream file(path);
    T result;

    file >> result;
    return file.fail() ? def : result;
}

FingerprintInscreen::FingerprintInscreen() {
    this->mFodCircleVisible = false;
	this->mFingerPressed = false;
    this->mVendorFpService = IGoodixFPExtendService::getService();
}

Return<void> FingerprintInscreen::onStartEnroll() {
    this->mVendorFpService->goodixExtendCommand(CMD_FINGERPRINT_EVENT, 1);
    return Void();
}

Return<void> FingerprintInscreen::onFinishEnroll() {
    return Void();
}

Return<void> FingerprintInscreen::onPress() {
    mFingerPressed = true;
    //std::thread([this]() {
    //    std::this_thread::sleep_for(std::chrono::milliseconds(15));
        if (mFingerPressed) {
            this->mVendorFpService->goodixExtendCommand(CMD_FINGERPRINT_EVENT, 1);
        }
    //}).detach();
    return Void();
}

Return<void> FingerprintInscreen::onRelease() {
    mFingerPressed = false;
    this->mVendorFpService->goodixExtendCommand(CMD_FINGERPRINT_EVENT, 0);
    return Void();
}

Return<void> FingerprintInscreen::onShowFODView() {
//    if (!mFodCircleVisible) {
//        dcDimState = get(DC_DIM_PATH, 0);
//        set(DC_DIM_PATH, 0);
//    }
    this->mFodCircleVisible = true;
    set(DIMLAYER_HBM_PATH, 1);
    return Void();
}

Return<void> FingerprintInscreen::onHideFODView() {
//    if (mFodCircleVisible) {
//        set(DC_DIM_PATH, dcDimState);
//    }
    this->mFodCircleVisible = false;
    set(DIMLAYER_HBM_PATH, 0);
    return Void();
}

Return<bool> FingerprintInscreen::handleAcquired(int32_t acquiredInfo, int32_t vendorCode) {
    LOG(ERROR) << "acquiredInfo: " << acquiredInfo << ", vendorCode: " << vendorCode << "\n";
    return false;
}

Return<bool> FingerprintInscreen::handleError(int32_t error, int32_t vendorCode) {
    LOG(ERROR) << "error: " << error << ", vendorCode: " << vendorCode << "\n";
    return false;
}

Return<void> FingerprintInscreen::setLongPressEnabled(bool) {
    return Void();
}

Return<int32_t> FingerprintInscreen::getDimAmount(int32_t) {
	return 0;
}

Return<bool> FingerprintInscreen::shouldBoostBrightness() {
    return false;
}

Return<void> FingerprintInscreen::setCallback(const sp<IFingerprintInscreenCallback>& callback) {
    {
        std::lock_guard<std::mutex> _lock(mCallbackLock);
        mCallback = callback;
    }

    return Void();
}

Return<int32_t> FingerprintInscreen::getPositionX() {
    return FOD_POS_X;
}

Return<int32_t> FingerprintInscreen::getPositionY() {
    return FOD_POS_Y;
}

Return<int32_t> FingerprintInscreen::getSize() {
    return FOD_SIZE;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace inscreen
}  // namespace fingerprint
}  // namespace biometrics
}  // namespace lineage
}  // namespace vendor
