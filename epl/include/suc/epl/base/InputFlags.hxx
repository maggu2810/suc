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
// Created by maggu2810 on 1/16/26.
//
#ifndef SUC_EPL_INPUT_FLAGS_HXX
#define SUC_EPL_INPUT_FLAGS_HXX

namespace suc::epl {
    enum class InputFlags {
        EdgeTriggered, /**< Requests edge-triggered notification for the associated file descriptor.  The default
              behavior for epoll is level-triggered.  See epoll(7) for more detailed information about edge-triggered
              and level-triggered  notifica‐ tion. */
        OneShot, /**< Requests  one-shot  notification  for  the  associated  file descriptor.  This means that after an
              event notified for the file descriptor by epoll_wait(2), the file descriptor is disabled in the interest
              list and no other events will be reported by the epoll interface.  The user must call epoll_ctl() with
              EPOLL_CTL_MOD to rearm the file descriptor with a new event mask. */
        WakeUp, /**< If EPOLLONESHOT and EPOLLET are clear and the process has the CAP_BLOCK_SUSPEND capability, ensure
        that the system does not enter "suspend" or "hibernate" while this event is pending or being processed.  The
        event is considered as being "processed" from the time when it is returned by a call to epoll_wait(2) until
        the next call to epoll_wait(2) on the same epoll(7) file descriptor, the closure of that file descriptor, the
        removal of  the event file descriptor with EPOLL_CTL_DEL, or the clearing of EPOLLWAKEUP for the event file
        descriptor with EPOLL_CTL_MOD.  See also BUGS. */
        Exclusive /**< Sets  an  exclusive  wakeup mode for the epoll file descriptor that is being attached to the
        target file descriptor, fd.  When a wakeup event occurs and multiple epoll file descriptors are attached to the
        same target file using EPOLLEXCLUSIVE, one or more of the epoll file descriptors will receive an event with
        epoll_wait(2).  The default in this scenario (when EPOLLEXCLUSIVE is not set) is for all epoll  file descriptors
        to  receive  an event.  EPOLLEXCLUSIVE is thus useful for avoiding thundering herd problems in certain
        scenarios.

        If  the same file descriptor is in multiple epoll instances, some with the EPOLLEXCLUSIVE flag, and others
        without, then events will be provided to all epoll instances that did not specify EPOLLEXCLUSIVE, and at least
        one of the epoll instances that did specify EPOLLEXCLUSIVE.

        The following values may be specified in conjunction with EPOLLEXCLUSIVE: EPOLLIN, EPOLLOUT, EPOLLWAKEUP, and
        EPOLLET.  EPOLLHUP and EPOLLERR can also be specified, but this is not required: as usual, these events are al‐
        ways reported if they occur, regardless of whether they are specified in events.  Attempts to specify other
        values in events yield the error EINVAL.

        EPOLLEXCLUSIVE may be used only in an EPOLL_CTL_ADD operation; attempts to employ it with EPOLL_CTL_MOD yield an
        error.  If EPOLLEXCLUSIVE has been set using epoll_ctl(),  then  a  subsequent  EPOLL_CTL_MOD  on  the  same
        epfd,  fd  pair yields an error.  A call to epoll_ctl() that specifies EPOLLEXCLUSIVE in events and specifies
        the target file descriptor fd as an epoll instance will likewise fail.  The error in all of these cases is EIN‐
        VAL. */
    };
}

#endif // SUC_EPL_INPUT_FLAGS_HXX
