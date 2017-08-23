# gRPC C++ samples for Google Assistant SDK

## Requirements

This project is officially supported on Ubuntu 14.04. Other Linux distributions may be able to run
this sample.

Refer to the [Assistant SDK documentation](https://developers.google.com/assistant/sdk/) for more information.

## Setup instructions

1. Remove previously installed Protobuf/gRPC/related packages and files. If you have not setup before, you can skip this step:
```
sudo apt-get purge libc-ares-dev  # https://github.com/grpc/grpc/pull/10706#issuecomment-302775038
sudo apt-get purge libprotobuf-dev libprotoc-dev
sudo rm -rf /usr/local/bin/grpc_* /usr/local/bin/protoc \
    /usr/local/include/google/protobuf/ /usr/local/include/grpc/ /usr/local/include/grpc++/ \
    /usr/local/lib/libproto* /usr/local/lib/libgpr* /usr/local/lib/libgrpc* \
    /usr/local/lib/pkgconfig/protobuf* /usr/local/lib/pkgconfig/grpc* \
    /usr/local/share/grpc/
```

2. Install dependencies
```
sudo apt-get install autoconf automake libtool build-essential curl unzip
sudo apt-get install libasound2-dev  # For ALSA sound output
sudo apt-get install libcurl4-openssl-dev # CURL development library
```

3. Build Protocol Buffer, gRPC and Google APIs
```
git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc
cd grpc/
git submodule update --init

cd third_party/protobuf
./autogen.sh && ./configure && make
sudo make install
sudo ldconfig

cd ../../
make
sudo make install
sudo ldconfig

cd ../
git clone https://github.com/googleapis/googleapis.git
cd googleapis/
make LANGUAGE=cpp
```

4. Make sure you setup env variable `$GOOGLEAPIS_GENS_PATH`
```
export GOOGLEAPIS_GENS_PATH=`pwd`/gens
```

5. Clone and build assistant-grpc
```
cd ../
git clone https://github.com/googlesamples/assistant-sdk-cpp.git

cd assistant-sdk-cpp/
make run_assistant
```

6. Get credentials file. It can be either an end-user's credentials, or a service account's credentials.

For end-user's credentials:

* Download the client secret json file from Google Cloud Platform Console following [these instructions](https://developers.google.com/assistant/sdk/develop/python/config-dev-project-and-account)
* Move it in this folder and rename it to `client_secret.json`
* run `get_credentials.sh` in this folder. It will create the file `credentials.json`.

For service account's credentials in Google Cloud Platform Console:

* Open **API Manager -> Credentials**
* Click **Create credentials -> Service account key**. Choose key type **JSON**, and click **Create**
* The service account's credentials JSON file will be downloaded. Notice that this JSON file can only be downloaded once, so keep it for future reference.

7. Start `run_assistant`
```
./run_assistant --audio_input ./resources/switch_to_channel_5.raw --credentials_file ./credentials.json --credentials_type USER_ACCOUNT
./run_assistant --audio_input ./resources/weather_in_mountain_view.raw --credentials_file <YOUR_SERVICE_ACCOUNT_JSON> --credentials_type SERVICE_ACCOUNT
# On Linux workstation, you can also use ALSA audio input:
./run_assistant --audio_input ALSA_INPUT --credentials_file ./credentials.json --credentials_type USER_ACCOUNT
```

Default Assistant gRPC API endpoint is embeddedassistant.googleapis.com. If you want to test with a custom Assistant gRPC API endpoint, you can pass an extra "--api_endpoint CUSTOM_API_ENDPOINT" to run_assistant.
