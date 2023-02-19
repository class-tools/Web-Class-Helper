**此页面适合开发人员 / 贡献者阅读。**

### 环境

* Windows 10 / 11

* Visual Studio 2022

* PowerShell

* vcpkg

### 依赖

vcpkg 依赖库安装流程如下：

```
vcpkg install jsoncpp:${arch}-windows-static spdlog:${arch}-windows-static
vcpkg integrate install
```

其中 `${arch}` 为构建的程序架构，可为 `x64` / `x86` / `ARM` / `ARM64`。

### 配置

打开项目根目录下的 `WCH.sln` 文件进入 Visual Studio 后，选择工具顶栏的 `Release` / `Debug` 和 `x64` / `x86` / `ARM` / `ARM64` 选项，再点击右侧的 `开始执行 (不调试)` 开始构建。

也可使用 [`scripts/build.ps1`](https://github.com/class-tools/Web-Class-Helper/blob/master/scripts/build.ps1) 自动化构建，具体方法见 [脚本](./Scripts)。

### 运行

构建完毕会自动运行可执行文件，之后还可直接双击运行构建目录中的可执行文件（**程序运行不受运行目录影响，但程序所在目录下必须有所需的依赖文件**）。

### 调试

在相应位置标记断点后选择 Visual Studio 工具顶栏的调试器即可。


### 问题

若在构建过程中出现 PowerShell 脚本返回值不为零的情况，请尝试以管理员身份运行 PowerShell 并执行下列命令：

```
Set-ExecutionPolicy RemoteSigned
```

_**若有任何问题欢迎与项目维护人员联系！**_
