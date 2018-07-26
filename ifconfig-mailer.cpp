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
 * \file   ifconfig-mailer.cpp
 *
 * \author Sandeep Prakash
 *
 * \date   Jun 24, 2018
 *
 * \brief
 *
 ******************************************************************************/
#include <sstream>
#include <glog/logging.h>
#include <arpa/inet.h>

#include "ifconfig-mailer.hpp"

#include <glog/logging.h>

using std::ostringstream;

using json = nlohmann::json;

namespace IfconfigMailer {

IfconfigMail::IfconfigMail(Config *config) {
   mConfig = config;
   mMailClient = new MailClient(config);
   mTimer = new Timer();
   ip = "0.0.0.0";
   changedIp = "0.0.0.0";
   request = nullptr;
   procStat = new ProcStat();
}

IfconfigMail::~IfconfigMail() {
   if(request) {
      delete request;
   }
   delete mTimer;
   delete mMailClient;
   delete procStat;
}

void IfconfigMail::_onLoad(HttpRequestLoadEvent *event, void *this_) {
  IfconfigMail *obj = (IfconfigMail *) this_;
  obj->onLoad(event);
}

bool IfconfigMail::getJson(string &str, json &mJson) {
  try {
    mJson = json::parse(str);
  } catch (const std::exception &e) { 
    LOG(WARNING) << "Error in parsing JSON: " << e.what();
    return false;
  }
  return true;
}

bool IfconfigMail::getIPAddress(json &mJson, string &ipAddress) {
  try {
    ipAddress = mJson["ip"];
  } catch (const std::exception &e) { 
    LOG(WARNING) << "Error in parsing JSON. IP Address not found: " << e.what();
    return false;
  }
  return true;
}

bool IfconfigMail::validateIpAddress(string &ipAddress) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
    return result != 0;
}

bool IfconfigMail::isValidIPAddress(string &ipAddress) {
  if(!ipAddress.length()) {
    LOG(WARNING) << "Empty IP Address. Discarding.";
    return false;
  }

  if(!validateIpAddress(ipAddress)) {
    LOG(WARNING) << "IP Address invalid: " << ipAddress;
    return false;
  }

  if(ipAddress.rfind("127.", 0) != string::npos) {
    LOG(WARNING) << "IP Address starts with 127.: " << ipAddress;
    return false;
  }

  if(ipAddress.rfind("192.168.", 0) != string::npos) {
    LOG(WARNING) << "IP Address starts with 192.168.: " << ipAddress;
    return false;
  }

  if(ipAddress.rfind("10.", 0) != string::npos) {
    LOG(WARNING) << "IP Address starts with 10.: " << ipAddress;
    return false;
  }

  if(ipAddress.rfind("172.", 0) != string::npos) {
    LOG(WARNING) << "IP Address starts with 172.: " << ipAddress;
    string sub = ipAddress.substr(4);
    size_t pos = sub.find_first_of(".");
    string octet = sub.substr(0, pos);
    LOG(WARNING) << "Octet: " << octet;
    int o = atoi(octet.c_str());
    if(o >= 16 && o <= 31) {
      LOG(WARNING) << "2nd octet is between 16 and 31 inclusive which makes it a private network. https://en.wikipedia.org/wiki/Private_network.";
      return false;
    }
  }

  return true;
}

void IfconfigMail::handleResponse(string &str) {
  json mJson;
  bool valid = getJson(str, mJson);
  if(!valid) {
    LOG(WARNING) << "Invalid JSON. Will discard!";
    return;
  }
  
  LOG(INFO) << "Valid JSON. Will process!";
  string changedIp;
  valid = getIPAddress(mJson, changedIp);
  if(!valid) {
    LOG(WARNING) << "Invalid JSON. IP Address field missing. Will discard!";
    return;
  }

  valid = isValidIPAddress(changedIp);
  if(!valid) {
    LOG(WARNING) << "Invalid IP Address. Will discard!";
    return;
  }

  LOG(INFO) << "Current IP Address: " << ip << " Changed IP Address: " << changedIp;
  if(ip != changedIp) {
    LOG(INFO) << "IP Address change. Setting IP to : " << changedIp;
    ip = changedIp;
    if(mConfig->getNotEnable() && mConfig->getNotEmailEnable()) {
      string eventString = ip + " --> " + changedIp;
      mMailClient->notifyIpAddressChange(eventString);
    } else {
      LOG(WARNING) << "Email notification disabled.";
    }
  } else {
    LOG(INFO) << "No IP Address change: " << ip;
  }
}

void IfconfigMail::onLoad(HttpRequestLoadEvent *event) {
  HttpResponse *response = event->getResponse();
  char *body = nullptr;
  uint32_t length = 0;
  if(response->getResponseCode() != 200) {
    LOG(INFO) << "Invalid response. Will try later";
    mTimer->restart(mTimerEvent);
    return;
  }
  response->getResponseBody((uint8_t **) &body, &length);
  string jsonBody(body, length);
  LOG(INFO) << "Response Body: \"" << jsonBody << "\"";
  if(body) free(body);

  handleResponse(jsonBody);

  //  json mJson = json::parse(jsonBody);
  //  changedIp = mJson["ip"];
  //  LOG(INFO) << "Current IP Address: " << ip << " Changed IP Address: " << changedIp;
  //  if(ip != changedIp) {
  //     LOG(INFO) << "IP Address change. Setting IP to : " << changedIp;
  //     string eventString = ip + " --> " + changedIp;
  //     ip = changedIp;
  //     if(mConfig->getNotEnable() && mConfig->getNotEmailEnable()) {
  //       mMailClient->notifyIpAddressChange(eventString);
  //     } else {
  //       LOG(WARNING) << "Email notification disabled.";
  //     }
      
  //  } else {
  //     LOG(INFO) << "No IP Address change: " << ip;
  //  }
  LOG(INFO) << "Request Complete: " << response->getResponseCode();
  LOG(INFO) << "Waiting for " << mConfig->getInterval() << " seconds until next check.";
  mTimer->restart(mTimerEvent);
}

void IfconfigMail::_onTimerEvent(TimerEvent *event, void *this_) {
  IfconfigMail *server = (IfconfigMail *) this_;
  server->onTimerEvent(event);
}

void IfconfigMail::onTimerEvent(TimerEvent *event) {
  LOG(INFO) << "Timer fired!";

  uint32_t rss = procStat->getRSS();
  LOG(INFO) << "RSS: " << rss / 1024 << " KB, Max RSS Configured: " << 
    (mConfig->getMaxRss() / 1024) << " KB";

  if(rss > mConfig->getMaxRss()) {
    LOG(ERROR) << "*********FATAL**********";
    LOG(ERROR) << "Too much memory in use. Exiting process.";
    LOG(FATAL) << "*********FATAL**********";
    exit(1);
  }

   if(request) {
      LOG(INFO) << "Deleting previous request context.";
      delete request;
      request = nullptr;
   }

   ostringstream os1;
      os1 << mConfig->getIfconfigUrl();

   request = new HttpRequest();
   request->onLoad(IfconfigMail::_onLoad).bind(this);
   request->open(EVHTTP_REQ_GET, os1.str())
      .setHeader("Host", "ifconfig.co")
      .setHeader("User-Agent", "curl/7.58.0")
      .setHeader("Accept", "*/*")
      .send();
}

void IfconfigMail::start() {
  LOG(INFO) << "Starting server";

  struct timeval tv = {0};
  tv.tv_sec = mConfig->getInterval();
  mTimerEvent = mTimer->create(&tv, IfconfigMail::_onTimerEvent, this);
}

void IfconfigMail::stop() {
  LOG(INFO) << "Stopping server";
}

}
