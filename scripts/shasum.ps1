# Web Class Helper SHA Verification Script 2.1.2
# This source code file is under MIT License.
# Copyright (c) 2022 - 2023 Class Tools Develop Team
# Contributors: ren-yc
$languages = "enUS", "zhCN"
$langfull = @{enUS = "en-US"; zhCN = "zh-CN"}
$types = "config", "help", "interactive"
$content = Get-Content .\shasum.hpp -Encoding ASCII
foreach ($itemlang in $languages) {
	foreach ($itemtype in $types) {
		$content = $content -replace ("#define SHASUM_" + $itemlang + "_$itemtype L`"([^`"]*)`""), ("#define SHASUM_" + $itemlang + "_$itemtype L`"" + (Get-FileHash (".\resources\" + $langfull[$itemlang] + "\" + $itemtype + ".json") -Algorithm SHA512).Hash + "`"")
	}
}
$content | Out-File .\shasum.hpp -Encoding ASCII
