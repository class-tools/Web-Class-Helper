$msbuild_path = (& (${env:ProgramFiles(x86)} + "\Microsoft Visual Studio\Installer\vswhere.exe") -latest -nologo -property installationPath -format value) + "\Msbuild\Current\Bin\MSBuild.exe"
$configarray = "x86", "x64", "ARM", "ARM64"
if ($args[1] -eq $null) {
	foreach ($config in $configarray) {
		& $msbuild_path -m /t:Build ("/p:Configuration=" + $args[0]) ("/p:Platform=" + $config) ".\WCH.sln"
	}
} else {
	& $msbuild_path -m /t:Build ("/p:Configuration=" + $args[0]) ("/p:Platform=" + $args[1]) ".\WCH.sln"
}