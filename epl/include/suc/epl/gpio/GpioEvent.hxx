// Copyright [2026] [maggu2810]
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

//
// Created by maggu2810 on 3/4/26.
//
#ifndef SUC_EPL_GPIOEVENT_HXX
#define SUC_EPL_GPIOEVENT_HXX

#include "suc/epl/base/Fd.hxx"

#include <suc/gpio/Event.hxx>

namespace suc::epl
{
class GpioEvent
{
public:
    explicit GpioEvent(suc::gpio::Event&& gpioEvent,
                       EventQueue&        eventQueue = EventQueue::coreInstance());
    ~GpioEvent();

    void onEvent(suc::gpio::Event::EdgeHandler&& func) const;

private:
    suc::gpio::Event m_gpioEvent;
    EventQueue&      m_eventQueue;
};
} // namespace suc::epl

#endif // SUC_EPL_GPIOEVENT_HXX
