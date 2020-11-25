#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <QDebug>
#include "settings.h"

char* readFile(const char* path) {
  FILE *file = fopen(path, "r");
  if (file) {
    fseek(file, 0, SEEK_END);
    long length=ftell(file);
    fseek (file, 0, SEEK_SET);
    char *buffer = (char*)malloc(length);
    if (buffer) {
      fread (buffer, 1, length, file);
    }
    fclose(file);
    return buffer;
  }
  return nullptr;
}

void Settings::readDeviceType() {
  char* device_type = readFile("/sys/devices/soc0/machine");
  qDebug() << device_type;
  _deviceType = DeviceType::UNKNOWN;

  if (device_type) {
    const char* const remarkable1[] {
      "reMarkable 1",
      "reMarkable Prototype 1",
    };
    const char* const remarkable2[] {
      "reMarkable 2"
    };
    for (int i = 0; i < sizeof(remarkable1) / sizeof(remarkable1[0]); ++i) {
      qDebug() << remarkable1[i] << strlen(remarkable1[i]);
      if (strncmp(remarkable1[i], device_type, strlen(remarkable1[i])) == 0) {
        _deviceType = DeviceType::RM1;
        qDebug() << "RM1 detected...";
      }
    }
    for (int i = 0; i < sizeof(remarkable2) / sizeof(remarkable2[0]); ++i) {
      if (strncmp(remarkable2[i], device_type, strlen(remarkable2[i])) == 0) {
        qDebug() << remarkable2[i] << strlen(remarkable2[i]);
        _deviceType = DeviceType::RM2;
        qDebug() << "RM2 detected...";
      }
    }
    free(device_type);
  }
}

DeviceType Settings::getDeviceType() {
  qDebug() << "Getting device type...";
  if (_deviceType == DeviceType::UNDETERMINED) {
  qDebug() << "Reading device type...";
    readDeviceType();
  }
  return _deviceType;
}

const char* Settings::getButtonsDevicePath() {
  switch(getDeviceType()) {
    case DeviceType::RM1:
      return "/dev/input/event2";
    case DeviceType::RM2:
      return "/dev/input/event0";
    default:
      return "";
  }
}

const char* Settings::getWacomDevicePath() {
  switch(getDeviceType()) {
    case DeviceType::RM1:
      return "/dev/input/event0";
    case DeviceType::RM2:
      return "/dev/input/event1";
    default:
      return "";
  }
}

const char* Settings::getTouchDevicePath() {
  switch(getDeviceType()) {
    case DeviceType::RM1:
      return "/dev/input/event1";
    case DeviceType::RM2:
      return "/dev/input/event2";
    default:
      return "";
  }
}
