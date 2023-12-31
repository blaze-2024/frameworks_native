/*
 * Copyright (C) 2022 The Android Open Source Project
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

#include "gui/VsyncEventData.h"
#include <gui/DisplayEventReceiver.h>
#include <private/gui/ParcelUtils.h>
#include <utils/Log.h>
#include <utils/Looper.h>
#include <cstdint>

namespace android::gui {

static_assert(VsyncEventData::kFrameTimelinesCapacity == 7,
              "Must update value in DisplayEventReceiver.java#FRAME_TIMELINES_CAPACITY (and here)");

int64_t VsyncEventData::preferredVsyncId() const {
    return frameTimelines[preferredFrameTimelineIndex].vsyncId;
}

int64_t VsyncEventData::preferredDeadlineTimestamp() const {
    return frameTimelines[preferredFrameTimelineIndex].deadlineTimestamp;
}

int64_t VsyncEventData::preferredExpectedPresentationTime() const {
    return frameTimelines[preferredFrameTimelineIndex].expectedPresentationTime;
}

status_t ParcelableVsyncEventData::readFromParcel(const Parcel* parcel) {
    if (parcel == nullptr) {
        ALOGE("%s: Null parcel", __func__);
        return BAD_VALUE;
    }

    SAFE_PARCEL(parcel->readInt64, &vsync.frameInterval);

    uint32_t uintPreferredFrameTimelineIndex;
    SAFE_PARCEL(parcel->readUint32, &uintPreferredFrameTimelineIndex);
    vsync.preferredFrameTimelineIndex = static_cast<size_t>(uintPreferredFrameTimelineIndex);

    uint32_t uintFrameTimelinesLength;
    SAFE_PARCEL(parcel->readUint32, &uintFrameTimelinesLength);
    vsync.frameTimelinesLength = static_cast<size_t>(uintFrameTimelinesLength);

    for (size_t i = 0; i < vsync.frameTimelinesLength; i++) {
        SAFE_PARCEL(parcel->readInt64, &vsync.frameTimelines[i].vsyncId);
        SAFE_PARCEL(parcel->readInt64, &vsync.frameTimelines[i].deadlineTimestamp);
        SAFE_PARCEL(parcel->readInt64, &vsync.frameTimelines[i].expectedPresentationTime);
    }

    return OK;
}
status_t ParcelableVsyncEventData::writeToParcel(Parcel* parcel) const {
    SAFE_PARCEL(parcel->writeInt64, vsync.frameInterval);
    SAFE_PARCEL(parcel->writeUint32, vsync.preferredFrameTimelineIndex);
    SAFE_PARCEL(parcel->writeUint32, vsync.frameTimelinesLength);
    for (size_t i = 0; i < vsync.frameTimelinesLength; i++) {
        SAFE_PARCEL(parcel->writeInt64, vsync.frameTimelines[i].vsyncId);
        SAFE_PARCEL(parcel->writeInt64, vsync.frameTimelines[i].deadlineTimestamp);
        SAFE_PARCEL(parcel->writeInt64, vsync.frameTimelines[i].expectedPresentationTime);
    }

    return OK;
}

}; // namespace android::gui
