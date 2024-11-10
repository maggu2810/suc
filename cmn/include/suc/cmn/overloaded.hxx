// Copyright [2024] [maggu2810]
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

#ifndef SUC_CMN_OVERLOADED_HXX
#define SUC_CMN_OVERLOADED_HXX

namespace suc::cmn {
    template<class... Ts>
    struct overloaded : Ts... {
        using Ts::operator()...;
    };
}

#endif //SUC_CMN_OVERLOADED_HXX
