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

#include "suc/epl/gpio/GpioEvent.hxx"

#include "suc/epl/base/EventQueue.hxx"

namespace
{
void handleEvent(const suc::gpio::Event& gpioEvent, const suc::gpio::Event::EdgeHandler& handler)
{
    std::ignore = gpioEvent.inspectInput(handler);
}
} // namespace

namespace suc::epl
{

GpioEvent::GpioEvent(suc::gpio::Event&& gpioEvent, EventQueue& eventQueue)
    : m_gpioEvent {std::move(gpioEvent)}, m_eventQueue(eventQueue)
{
    m_eventQueue.add(m_gpioEvent.getFdForInputEvent());
}

GpioEvent::~GpioEvent()
{
    m_eventQueue.del(m_gpioEvent.getFdForInputEvent());
}

void GpioEvent::onEvent(suc::gpio::Event::EdgeHandler&& func) const
{
    m_eventQueue.mod(m_gpioEvent.getFdForInputEvent(),
                     [&](auto& callbacks)
                     {
                         if (func)
                         {
                             callbacks.inputAvailable = [func = std::move(func), this]()
                             {
                                 handleEvent(m_gpioEvent, func);
                             };
                         }
                         else
                         {
                             callbacks.inputAvailable = {};
                         }
                     });
}

} // namespace suc::epl