& gpg -o ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_x64.zip.sig") -ab ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_x64.zip")
& gpg -o ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_x86.zip.sig") -ab ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_x86.zip")
& gpg -o ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_ARM.zip.sig") -ab ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_ARM.zip")
& gpg -o ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_ARM64.zip.sig") -ab ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_ARM64.zip")