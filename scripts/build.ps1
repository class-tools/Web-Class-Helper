$devenv_path = (& (${env:ProgramFiles(x86)} + "\Microsoft Visual Studio\Installer\vswhere.exe") -latest -nologo -property installationPath -format value) + "\Common7\IDE\devenv.com"
$solution_path = ".\WCH.sln"
$run_mode = "/Build"
$configarray = "x86", "x64", "ARM", "ARM64"
if ($args[0] -eq "Debug" || $args[0] -eq "Release") {
	foreach ($config in $configarray) {
		& $devenv_path $solution_path $run_mode ($args[0] + "|" + $config)
	}
} else {
	& $devenv_path $solution_path $run_mode $args[0]
}