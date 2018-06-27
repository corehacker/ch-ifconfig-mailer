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
 * \file   config.cpp
 *
 * \author Sandeep Prakash
 *
 * \date   Jun 24, 2018
 *
 * \brief
 *
 ******************************************************************************/
#include <string>
#include <glog/logging.h>

#include "config.hpp"

using std::ifstream;

namespace IfconfigMailer {


Config::Config() :
				ChCppUtils::Config("/etc/ch-ifconfig-mailer/ch-ifconfig-mailer.json",
						"./config/ch-ifconfig-mailer.json") {
   mMaxRss = 0;
   mInterval = 0;
   mIfconfigUrl = "";
	mNotEnable = false;
	mNotEmailEnable = false;
	mNotEmailThreadCount = 0;
	mNotEmailFrom = "";
	mNotEmailSubject = "";
	mNotEmailSmtpUrl = "";
	mNotEmailAggregate = 0;
}

Config::~Config() {
	LOG(INFO) << "*****************~Config";
}

bool Config::populateConfigValues() {
	LOG(INFO) << "<-----------------------Config";

	mMaxRss = mJson["max-rss"];
	LOG(INFO) << "max-rss: " << mMaxRss;

	mInterval = mJson["interval"];
	LOG(INFO) << "interval: " << mInterval;

	mIfconfigUrl = mJson["ifconfig-url"];
	LOG(INFO) << "ifconfig-url: " << mIfconfigUrl;

	mNotEnable = mJson["notifications"]["enable"];
	LOG(INFO) << "notifications.enable: " << mNotEnable;

	mNotEmailEnable = mJson["notifications"]["email"]["enable"];
	LOG(INFO) << "notifications.email.enable: " << mNotEmailEnable;

	mNotEmailThreadCount = mJson["notifications"]["email"]["thread-count"];
	LOG(INFO) << "notifications.email.thread-count: " << mNotEmailThreadCount;

	mNotEmailFrom = mJson["notifications"]["email"]["from"];
	LOG(INFO) << "notifications.email.from: " << mNotEmailFrom;

	for(auto to : mJson["notifications"]["email"]["to"]) {
		mNotEmailTo.push_back(to);
	}

	for(auto cc : mJson["notifications"]["email"]["cc"]) {
		mNotEmailCc.push_back(cc);
	}

	mNotEmailSubject = mJson["notifications"]["email"]["subject"];
	LOG(INFO) << "notifications.email.subject: " << mNotEmailSubject;

	mNotEmailSmtpUrl = mJson["notifications"]["email"]["smtp"]["url"];
	LOG(INFO) << "notifications.email.smtp.url: " << mNotEmailSmtpUrl;

	mNotEmailAggregate = mJson["notifications"]["email"]["aggregate"];
	LOG(INFO) << "notifications.email.aggregate: " << mNotEmailAggregate;

	LOG(INFO) << "----------------------->Config";
	return true;
}

void Config::init() {
	ChCppUtils::Config::init();

	populateConfigValues();
}

uint32_t Config::getMaxRss() {
   return mMaxRss;
}

uint32_t Config::getInterval() {
   return mInterval;
}

string Config::getIfconfigUrl() {
   return mIfconfigUrl;
}

bool Config::getNotEnable() {
	return mNotEnable;
}

bool Config::getNotEmailEnable() {
	return mNotEmailEnable;
}

uint32_t Config::getNotEmailThreadCount() {
	return mNotEmailThreadCount;
}

string Config::getNotEmailFrom() {
	return mNotEmailFrom;
}

vector<string> Config::getNotEmailTo() {
	return mNotEmailTo;
}

vector<string> Config::getNotEmailCc() {
	return mNotEmailCc;
}

string Config::getNotEmailSubject() {
	return mNotEmailSubject;
}

string Config::getNotEmailSmtpUrl() {
	return mNotEmailSmtpUrl;
}

uint64_t Config::getNotEmailAggregate() {
	return mNotEmailAggregate;
}


} // End namespace IfconfigMailer.
