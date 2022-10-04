$devenv_path = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.com"
$solution_path = ".\WCH.sln"
$run_mode = "/Build"
$config_mode = "Release"
& Write-Output "--- x64 ---"
& $devenv_path $solution_path $run_mode ($config_mode + "|x64")
& Write-Output "--- Done ---"
& Write-Output "--- x86 ---"
& $devenv_path $solution_path $run_mode ($config_mode + "|x86")
& Write-Output "--- Done ---"
& Write-Output "--- ARM ---"
& $devenv_path $solution_path $run_mode ($config_mode + "|ARM")
& Write-Output "--- Done ---"
& Write-Output "--- ARM64 ---"
& $devenv_path $solution_path $run_mode ($config_mode + "|ARM64")
& Write-Output "--- Done ---"