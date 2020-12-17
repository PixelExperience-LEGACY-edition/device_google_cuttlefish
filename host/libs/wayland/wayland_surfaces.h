/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <stdint.h>
#include <thread>
#include <unordered_map>

namespace wayland {

class Surface;

class Surfaces {
 public:
  Surfaces() = default;
  virtual ~Surfaces() = default;

  Surfaces(const Surfaces& rhs) = delete;
  Surfaces& operator=(const Surfaces& rhs) = delete;

  Surfaces(Surfaces&& rhs) = delete;
  Surfaces& operator=(Surfaces&& rhs) = delete;

  Surface* GetOrCreateSurface(std::uint32_t id);

  using FrameCallback = std::function<void(std::uint32_t /*display_number*/,
                                           std::uint8_t* /*frame_pixels*/)>;

  // Blocking
  void OnNextFrame(const FrameCallback& callback);

 private:
  friend class Surface;
  void HandleSurfaceFrame(std::uint32_t display_number,
                          std::uint8_t* frame_bytes);

  std::mutex surfaces_mutex_;
  std::unordered_map<std::uint32_t, std::unique_ptr<Surface>> surfaces_;

  using FrameCallbackPackaged =
    std::packaged_task<void(std::uint32_t /*display_number*/,
                            std::uint8_t* /*frame_bytes*/)>;

  std::mutex callback_mutex_;
  std::optional<FrameCallbackPackaged*> callback_;
};

}  // namespace wayland