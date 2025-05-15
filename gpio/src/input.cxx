// Copyright [2025] [maggu2810]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "suc/gpio/input.hxx"

#include <linux/gpio.h>
#include <stdexcept>
#include <suc/cmn/runtimeerror_errno.hxx>
#include <sys/ioctl.h>

namespace suc::gpio {
    input::input(suc::cmn::openfd&& fd) : line(std::move(fd)) {}

    bool input::get() const {
        gpio_v2_line_values values{.bits = 0ULL, .mask = 1};

        if (ioctl(m_fd.fd(), GPIO_V2_LINE_GET_VALUES_IOCTL, &values) == -1) {
            throw suc::cmn::runtimeerror_errno("get");
        }

        return values.bits & 1;
    }
} // namespace suc::gpio
