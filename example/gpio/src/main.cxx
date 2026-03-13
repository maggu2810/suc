// Copyright [2024-2025] [maggu2810]
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

#include <cstdlib>
#include <suc/gpio/Chip.hxx>
#include <unistd.h>

#if 0
https://docs.kernel.org/admin-guide/gpio/gpio-sim.html

cd /sys/kernel/config
modprobe gpio-sim
cd gpio-sim
mkdir gpio-device
mkdir gpio-device/gpio-bank0
echo 30 > gpio-device/gpio-bank0/num_lines
mkdir gpio-device/gpio-bank0/line5
echo foo > gpio-device/gpio-bank0/line5/name
mkdir gpio-device/gpio-bank0/line10
echo bar > gpio-device/gpio-bank0/line10/name
echo mylabel > gpio-device/gpio-bank0/label
echo 1 > gpio-device/live

while true; do for i in pull-up pull-down; do echo $i > /sys/devices/platform/gpio-sim.0/gpiochip0/sim_gpio9/pull; sleep 1; done; done
#endif

int main(int argc, char* argv[])
{
    suc::gpio::Chip chip {0};
    auto            o7 = chip.getOutput(7);
    auto            i8 = chip.getInput(8);
    auto            e9 = chip.getEvent(9);
    sleep(60);
    return EXIT_SUCCESS;
}
