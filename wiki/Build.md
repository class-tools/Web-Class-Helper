**此页面适合开发人员 / 贡献者阅读。**

### 环境

* Windows 10 / 11

* Visual Studio 2022

* `vcpkg` （需安装对应架构的动态及静态 `jsoncpp` 库）

### 配置

打开项目根目录下的 `WCH.sln` 文件进入 Visual Studio 后，选择工具顶栏的 `Release` / `Debug` 和 `x64` / `x86` / `ARM` / `ARM64` 选项，再点击右侧的 `开始执行 (不调试)` 开始构建。

也可使用 [`scripts/build.ps1`](https://github.com/class-tools/Web-Class-Helper/blob/master/scripts/build.ps1) 自动化构建。

### 运行

构建完毕会自动运行可执行文件。若您需单独运行（双击），请将资源及依赖文件同构建完毕的可执行文件复制到单独的临时文件夹并在该文件夹下双击运行或从命令行启动。

### 调试

在相应位置标记断点后选择 Visual Studio 工具顶栏的调试器即可。

_**若有任何问题欢迎与项目维护人员联系！**_