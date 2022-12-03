$itemarray = ".\resources", ".\CONTRIBUTING.md", ".\CONTRIBUTING.zh-CN.md", ".\LICENSE", ".\README.md", ".\README.zh-CN.md", ".\SECURITY.md", ".\SECURITY.zh-CN.md", ".\WCH.ico"
& New-Item -Path ($env:USERPROFILE + "\Downloads") -Name ("WCH_" + $args[0] + "_all") -ItemType "directory" > $null
foreach ($item in $itemarray) {
	& Copy-Item -Path $item -Destination ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_all") -Recurse
}
& Copy-Item -Path ".\x64\Release\WCH.exe" -Destination ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_all\WCH_x64.exe") -Recurse
& Copy-Item -Path ".\x86\Release\WCH.exe" -Destination ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_all\WCH_x86.exe") -Recurse
& Copy-Item -Path ".\ARM\Release\WCH.exe" -Destination ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_all\WCH_ARM.exe") -Recurse
& Copy-Item -Path ".\ARM64\Release\WCH.exe" -Destination ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_all\WCH_ARM64.exe") -Recurse