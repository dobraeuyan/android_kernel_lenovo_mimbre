# Copyright (C) 2012 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Emit extra commands needed for Group during OTA installation
(installing the bootloader)."""

import struct
import common


def WriteCapsule(info):
  info.script.AppendExtra('package_extract_file("capsule_32to64_pvt.bin", "/tmp/capsule.bin");')
  info.script.AppendExtra("""flash_capsule("/tmp/capsule.bin");""")

def WriteEspUpdate(info):
  info.script.AppendExtra('package_extract_file("esp.zip", "/tmp/esp.zip");')
  info.script.AppendExtra("""flash_esp_update("/tmp/esp.zip");""")

def WriteDroidboot(info):
  info.script.WriteRawImage("/fastboot", "droidboot.img")

def FullOTA_InstallEnd(info):
  try:
    capsule = info.input_zip.read("RADIO/capsule_32to64_pvt.bin")
  except KeyError:
    print "no capsule in target_files; skipping install"
  else:
    common.ZipWriteStr(info.output_zip, "capsule_32to64_pvt.bin", capsule)
    WriteCapsule(info)
  try:
    esp = info.input_zip.read("RADIO/esp.zip")
  except KeyError:
    print "no esp in target_files; skipping install"
  else:
    common.ZipWriteStr(info.output_zip, "esp.zip", esp)
    WriteEspUpdate(info)
  try:
    droidboot_img = info.input_zip.read("RADIO/droidboot.img")
  except KeyError:
    print "no droidboot.img in target_files; skipping install"
  else:
    common.ZipWriteStr(info.output_zip, "droidboot.img", droidboot_img)
    WriteDroidboot(info)


def IncrementalOTA_InstallEnd(info):
  try:
    capsule = info.target_zip.read("RADIO/capsule_32to64_pvt.bin")
  except KeyError:
    print "no capsule in target_files; skipping install"
  else:
    common.ZipWriteStr(info.output_zip, "capsule_32to64_pvt.bin", capsule)
    WriteCapsule(info)
  try:
    esp = info.target_zip.read("RADIO/esp.zip")
  except KeyError:
    print "no esp in target_files; skipping install"
  else:
    common.ZipWriteStr(info.output_zip, "esp.zip", esp)
    WriteEspUpdate(info)
  try:
    droidboot_img = info.target_zip.read("RADIO/droidboot.img")
  except KeyError:
    print "no droidboot.img in target_files; skipping install"
  else:
    common.ZipWriteStr(info.output_zip, "droidboot.img", droidboot_img)
    WriteDroidboot(info)

