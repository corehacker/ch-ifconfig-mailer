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
 * \file   config.hpp
 *
 * \author Sandeep Prakash
 *
 * \date   Jun 24, 2018
 *
 * \brief
 *
 ******************************************************************************/
#include <vector>
#include <string>
#include <ch-cpp-utils/utils.hpp>
#include <ch-cpp-utils/config.hpp>

#ifndef SRC_CONFIG_HPP_
#define SRC_CONFIG_HPP_

using std::vector;
using std::string;

namespace IfconfigMailer {

class Config : public ChCppUtils::Config {
public:
	Config();
	~Config();
	void init();

   uint32_t getMaxRss();
   uint32_t getInterval();
   string getIfconfigUrl();

	bool getNotEnable();
	bool getNotEmailEnable();
	uint32_t getNotEmailThreadCount();
	string getNotEmailFrom();
	vector<string> getNotEmailTo();
	vector<string> getNotEmailCc();
	string getNotEmailSubject();
	string getNotEmailSmtpUrl();
	uint64_t getNotEmailAggregate();

private:

	string etcConfigPath;
	string localConfigPath;
	string selectedConfigPath;

   uint32_t mMaxRss;
   uint32_t mInterval;
   string mIfconfigUrl;

	bool mNotEnable;
	bool mNotEmailEnable;
	uint32_t mNotEmailThreadCount;
	string mNotEmailFrom;
	vector<string> mNotEmailTo;
	vector<string> mNotEmailCc;
	string mNotEmailSubject;
	string mNotEmailSmtpUrl;
	uint64_t mNotEmailAggregate;

	bool populateConfigValues();
};

} // End namespace IfconfigMailer.


#endif /* SRC_CONFIG_HPP_ */
