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

#ifndef JSON_UTIL_H
#define JSON_UTIL_H

#include <memory>
#include <string>

/* Extracts "customResponse" field from json string. E.g.
   {
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
   }
   Should output "CUSTOM RESPONSE FROM 3P OEM CLOUD".
 */
std::unique_ptr<std::string> GetCustomResponseOrNull(
    const std::string& device_control_request_json);

#endif
