/*
 * Copyright (C) 2019 The Android Open Source Project
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

#pragma once

#include <memory>
#include <thread>

#include <json/json.h>

#include "common/libs/fs/shared_fd.h"

namespace cuttlefish {
namespace webrtc_streaming {

// Listen to kernel log events and report them to clients.
struct KernelLogEventsHandler {
  explicit KernelLogEventsHandler(SharedFD kernel_log_fd,
      std::function<void(const Json::Value&)> send_to_client);

  ~KernelLogEventsHandler();

 private:

  std::function<void(const Json::Value&)> send_to_client_;

  void ReadLoop();

  SharedFD kernel_log_fd_;
  std::thread read_thread_;
};

}  // namespace webrtc_streaming
}  // namespace cuttlefish