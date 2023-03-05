# Web Class Helper OpenPGP Signature Script 2.1.2
# This source code file is under MIT License.
# Copyright (c) 2022 - 2023 Class Tools Develop Team
# Contributors: ren-yc
gpg -o ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_x64.zip.sig") -ab ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_x64.zip")
gpg -o ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_x86.zip.sig") -ab ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_x86.zip")
gpg -o ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_ARM.zip.sig") -ab ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_ARM.zip")
gpg -o ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_ARM64.zip.sig") -ab ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_ARM64.zip")
