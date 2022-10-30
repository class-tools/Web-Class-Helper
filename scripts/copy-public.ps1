$itemarray = ".\resources", "CONTRIBUTING.md", "CONTRIBUTING.zh-CN.md", "LICENSE", "README.md", "README.zh-CN.md", "SECURITY.md", "SECURITY.zh-CN.md", "WCH.ico"
& New-Item -Path ($env:USERPROFILE + "\Downloads") -Name ("WCH_" + $args[0] + "_x64") -ItemType "directory" > $null
foreach ($item in $itemarray + ".\x64\Release\WCH.exe") {
	& Copy-Item -Path $item -Destination ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_x64") -Recurse
}
& New-Item -Path ($env:USERPROFILE + "\Downloads") -Name ("WCH_" + $args[0] + "_x86") -ItemType "directory" > $null
foreach ($item in $itemarray + ".\x86\Release\WCH.exe") {
	& Copy-Item -Path $item -Destination ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_x86") -Recurse
}
& New-Item -Path ($env:USERPROFILE + "\Downloads") -Name ("WCH_" + $args[0] + "_ARM") -ItemType "directory" > $null
foreach ($item in $itemarray + ".\ARM\Release\WCH.exe") {
	& Copy-Item -Path $item -Destination ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_ARM") -Recurse
}
& New-Item -Path ($env:USERPROFILE + "\Downloads") -Name ("WCH_" + $args[0] + "_ARM64") -ItemType "directory" > $null
foreach ($item in $itemarray + ".\ARM64\Release\WCH.exe") {
	& Copy-Item -Path $item -Destination ($env:USERPROFILE + "\Downloads\WCH_" + $args[0] + "_ARM64") -Recurse
}