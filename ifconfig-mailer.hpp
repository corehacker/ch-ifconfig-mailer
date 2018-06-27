/*******************************************************************************
 *
 *  BSD 2-Clause License
 *
 *  Copyright (c) 2018, Sandeep Prakash
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

/*******************************************************************************
 * Copyright (c) 2018, Sandeep Prakash <123sandy@gmail.com>
 *
 * \file   ifconfig-mailer.hpp
 *
 * \author Sandeep Prakash
 *
 * \date   Jun 24, 2018
 *
 * \brief
 *
 ******************************************************************************/

#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/buffer.h>
#include <ch-cpp-utils/http-connection.hpp>
#include <ch-cpp-utils/http-client.hpp>
#include <ch-cpp-utils/http-request.hpp>
#include <ch-cpp-utils/timer.hpp>
#include <ch-cpp-utils/utils.hpp>
#include <ch-cpp-utils/proc-stat.hpp>

#include "config.hpp"
#include "mail-client.hpp"

#ifndef SRC_IFCONFIG_MAILER_HPP_
#define SRC_IFCONFIG_MAILER_HPP_

using ChCppUtils::Timer;
using ChCppUtils::TimerEvent;
using ChCppUtils::Http::Client::HttpRequest;
using ChCppUtils::Http::Client::HttpResponse;
using ChCppUtils::Http::Client::HttpRequestLoadEvent;
using ChCppUtils::ProcStat;

namespace IfconfigMailer {

class IfconfigMail {
   private:
      Config *mConfig;
      MailClient *mMailClient;
      Timer *mTimer;
      TimerEvent *mTimerEvent;
      HttpRequest *request;
      string ip;
      string changedIp;
      ProcStat *procStat;

      static void _onLoad(HttpRequestLoadEvent *event, void *this_);
      void onLoad(HttpRequestLoadEvent *event);

      static void _onTimerEvent(TimerEvent *event, void *this_);
      void onTimerEvent(TimerEvent *event);
   public:
      IfconfigMail(Config *config);
      ~IfconfigMail();

      void start();
      void stop();
};


}

#endif // SRC_IFCONFIG_MAILER_HPP_
