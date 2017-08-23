/*
Copyright 2017 Google Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "json_util.h"

#include <iostream>

bool check_result(const std::string& input_json,
                  std::unique_ptr<std::string> intended_result) {
  std::unique_ptr<std::string> result = GetCustomResponseOrNull(input_json);
  return (intended_result == nullptr && result == nullptr)
      || (intended_result != nullptr && result != nullptr
          && *intended_result == *result);
}

int main() {
  std::string invalid_json = "";
  std::unique_ptr<std::string> intended_result(nullptr);
  if (!check_result(invalid_json, std::move(intended_result))) {
    std::cerr << "Test failed for invalid JSON" << std::endl;
    return 1;
  }

  std::string incomplete_json = "{}";
  intended_result.reset(nullptr);
  if (!check_result(incomplete_json, std::move(intended_result))) {
    std::cerr << "Test failed for incomplete JSON" << std::endl;
    return 1;
  }

  std::string valid_json = R"({
		"requestId": "ff36a3cc-ec34-11e6-b1a0-64510650abcf",
		"inputs": [{
		  "intent": "action.devices.EXECUTE",
		  "payload": {
		    "commands": [{
					"devices": [{
						"id": "12345"
					}],
					"execution": [{
						"command": "action.devices.commands.Custom",
						"params": {
						  "customResponse": "CUSTOM RESPONSE FROM 3P OEM CLOUD"
						}
					}]
		    }]
		  }
		}]
	})";
  intended_result.reset(
      new std::string("CUSTOM RESPONSE FROM 3P OEM CLOUD"));
  if (!check_result(valid_json, std::move(intended_result))) {
    std::cerr << "Test failed for valid JSON" << std::endl;
    return 1;
  }

  std::cerr << "Test passed" << std::endl;
}
