//
// Copyright (C) 2011-2012 Andrey Sibiryov <me@kobology.ru>
//
// Licensed under the BSD 2-Clause License (the "License");
// you may not use this file except in compliance with the License.
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef COCAINE_EVENTS_HPP
#define COCAINE_EVENTS_HPP

#include <boost/statechart/event.hpp>

#include "cocaine/common.hpp"
#include "cocaine/forwards.hpp"

#include "cocaine/dealer/types.hpp"

namespace cocaine { namespace engine { namespace events {

namespace sc = boost::statechart;

struct heartbeat_t:
    public sc::event<heartbeat_t>
{ };

struct terminate_t:
    public sc::event<terminate_t>
{ };

struct enqueue_t:
    public sc::event<enqueue_t>
{
    enqueue_t(size_t position_):
        position(position_)
    { }

    const size_t position;
};

struct invoke_t:
    public sc::event<invoke_t>
{
    invoke_t(const boost::shared_ptr<job_t>& job_):
        job(job_)
    { }

    const boost::shared_ptr<job_t>& job;
};

struct push_t:
    public sc::event<push_t>
{
    push_t(zmq::message_t& message_):
        message(message_)
    { }

    // NOTE: There's no way to pass it via a const-reference due to ZeroMQ
    // limitations - the message gets destroyed when sent.
    zmq::message_t& message;
};

struct error_t:
    public sc::event<error_t>
{
    error_t(client::error_code code_, const std::string& message_):
        code(code_),
        message(message_)
    { }
    
    const client::error_code code;
    const std::string& message;
};

struct release_t:
    public sc::event<release_t>
{ };

}}}

#endif
