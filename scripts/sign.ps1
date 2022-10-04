& Write-Output "--- x64 ---"
& gpg -o ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_x64.zip.sig") -ab ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_x64.zip")
& Write-Output "--- Done ---"
& Write-Output "--- x86 ---"
& gpg -o ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_x86.zip.sig") -ab ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_x86.zip")
& Write-Output "--- Done ---"
& Write-Output "--- ARM ---"
& gpg -o ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_ARM.zip.sig") -ab ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_ARM.zip")
& Write-Output "--- Done ---"
& Write-Output "--- ARM64 ---"
& gpg -o ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_ARM64.zip.sig") -ab ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_ARM64.zip")
& Write-Output "--- Done ---"