/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.livedisplay-service.lenovo_msmnile"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <binder/ProcessState.h>
#include <livedisplay/sdm/PictureAdjustment.h>

#include "AntiFlicker.h"
#include "SunlightEnhancement.h"

using ::aidl::vendor::lineage::livedisplay::AntiFlicker;
using ::aidl::vendor::lineage::livedisplay::SunlightEnhancement;
using ::aidl::vendor::lineage::livedisplay::sdm::PictureAdjustment;
using ::aidl::vendor::lineage::livedisplay::sdm::SDMController;

int main() {
    android::ProcessState::self()->setThreadPoolMaxThreadCount(1);
    android::ProcessState::self()->startThreadPool();

    std::shared_ptr<SDMController> controller = std::make_shared<SDMController>();
    std::shared_ptr<PictureAdjustment> pictureAdjustment =
            ndk::SharedRefBase::make<PictureAdjustment>(controller);
    std::string instance = std::string() + PictureAdjustment::descriptor + "/default";
    if (AServiceManager_addService(pictureAdjustment->asBinder().get(), instance.c_str()) !=
        STATUS_OK) {
        LOG(ERROR) << "Cannot register picture adjustment HAL service.";
        return 1;
    }

    std::shared_ptr<AntiFlicker> antiFlicker = ndk::SharedRefBase::make<AntiFlicker>();
    instance = std::string() + AntiFlicker::descriptor + "/default";
    if (AServiceManager_addService(antiFlicker->asBinder().get(), instance.c_str()) != STATUS_OK) {
        LOG(ERROR) << "Cannot register anti flicker HAL service.";
        return 1;
    }

    std::shared_ptr<SunlightEnhancement> sunlightEnhancement =
            ndk::SharedRefBase::make<SunlightEnhancement>();
    instance = std::string() + SunlightEnhancement::descriptor + "/default";
    if (AServiceManager_addService(sunlightEnhancement->asBinder().get(), instance.c_str()) !=
        STATUS_OK) {
        LOG(ERROR) << "Cannot register sunlight enhancement HAL service.";
        return 1;
    }
    LOG(INFO) << "LiveDisplay HAL service is ready.";

    ABinderProcess_joinThreadPool();

    LOG(ERROR) << "LiveDisplay HAL service failed to join thread pool.";
    return 1;
}
