### 资源文件

`resources` 文件夹包含若干个以语言代码为名称的子文件夹。

每个子文件夹下包含 3 个 Json 格式的 `.json` 依赖文件，分别保存着配置参考、帮助信息、交互文字的数据，请勿更改。

### 运行时生成

**所有运行时生成的数据文件均在 `%USERPROFILE%/AppData/Local/WCH` 目录下。**

`settings.json` 是程序的配置文件（若无则按默认配置生成）。

`data` 文件夹下最多包含 3 个 Json 格式的 `.json` 数据文件，分别保存着闹钟列表、进程列表、工作计划的相关信息，请勿随意更改。

`logs` 文件夹下用于存放 `.log` 日志文件，保存着每一次程序运行的详细信息，您在报告错误时可以向我们发送该次运行日志文件。

### 目录结构

```
AppData/
 - Local/
 - - WCH/
 - - - data/
 - - - - clock.json
 - - - - task.json
 - - - - work.json
 - - - logs/
 - - - - *.log
 - - - settings.json
WCHExecDir/
 - resources/
 - - en-US/
 - - - config.json
 - - - help.json
 - - - interactive.json
 - - zh-CN/
 - - - config.json
 - - - help.json
 - - - interactive.json
 - CONTRIBUTING.md
 - CONTRIBUTING.zh-CN.md
 - LICENSE
 - README.md
 - README.zh-CN.md
 - SECURITY.md
 - SECURITY.zh-CN.md
 - WCH.exe
 - WCH.ico
```