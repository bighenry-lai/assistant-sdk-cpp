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
#include "scope_exit.h"

extern "C" {
#include <src/core/lib/json/json.h>
}

#include <cstring>
#include <iostream>

grpc_json* GetJsonValueOrNullFromDict(grpc_json* dict_node, const char* key) {
  if (dict_node->type != GRPC_JSON_OBJECT) {
    return nullptr;
  }

  grpc_json* child = dict_node->child;
  while (child != nullptr) {
    if (child->key != nullptr && strcmp(child->key, key) == 0) {
      return child;
    }
    child = child->next;
  }
  return nullptr;
}

grpc_json* GetJsonValueOrNullFromArray(grpc_json* array_node, int index) {
  if (array_node->type != GRPC_JSON_ARRAY) {
    return nullptr;
  }

  grpc_json* child = array_node->child;
  while (child != nullptr && index != 0) {
    child = child->next;
    index--;
  }
  return child;
}

std::unique_ptr<std::string> GetCustomResponseOrNull(
    const std::string& device_control_request_json) {
  if (device_control_request_json.length() == 0) {
    return nullptr;
  }
  std::unique_ptr<char[]> s(new char[device_control_request_json.length()]);
  memcpy(s.get(), device_control_request_json.c_str(),
      device_control_request_json.length());
  grpc_json* json = grpc_json_parse_string_with_len(s.get(),
      device_control_request_json.length());
  if (json == nullptr) {
    std::cerr << "Failed to parse json string: \""
        << device_control_request_json << "\"" << std::endl;
    return nullptr;
  }
  ScopeExit destroy_json([json]() {
    grpc_json_destroy(json);
  });

#define RETURN_NULLPTR_IF_NULLPTR(var) if (var == nullptr) { return nullptr; }
  grpc_json* inputsArray = GetJsonValueOrNullFromDict(json, "inputs");
  RETURN_NULLPTR_IF_NULLPTR(inputsArray);
  grpc_json* input = GetJsonValueOrNullFromArray(inputsArray, 0);
  RETURN_NULLPTR_IF_NULLPTR(input);

  grpc_json* payload = GetJsonValueOrNullFromDict(input, "payload");
  RETURN_NULLPTR_IF_NULLPTR(payload);

  grpc_json* commandsArray = GetJsonValueOrNullFromDict(payload, "commands");
  RETURN_NULLPTR_IF_NULLPTR(commandsArray);
  grpc_json* command = GetJsonValueOrNullFromArray(commandsArray, 0);
  RETURN_NULLPTR_IF_NULLPTR(command);

  grpc_json* executionsArray = GetJsonValueOrNullFromDict(command, "execution");
  RETURN_NULLPTR_IF_NULLPTR(executionsArray);
  grpc_json* execution = GetJsonValueOrNullFromArray(executionsArray, 0);
  RETURN_NULLPTR_IF_NULLPTR(execution);

  grpc_json* params = GetJsonValueOrNullFromDict(execution, "params");
  RETURN_NULLPTR_IF_NULLPTR(params);

  grpc_json* custom_response = GetJsonValueOrNullFromDict(params, "customResponse");
  RETURN_NULLPTR_IF_NULLPTR(custom_response);
#undef RETURN_NULLPTR_IF_NULLPTR
  if (custom_response && custom_response->type == GRPC_JSON_STRING) {
    return std::unique_ptr<std::string>(
        new std::string(custom_response->value));
  }

  return nullptr;
}
