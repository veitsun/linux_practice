
# file description


## concurrency/

## test/

### test1 线程数组
### test2 获取目录下的所有文件并且输出文件信息

## kernel_lab/

一个独立的 Linux 内核实验模块，使用 `CMake` 暴露自定义目标来编译 `.ko` 文件。
常用命令：

```bash
cmake -S . -B build
cmake --build build --target kernel_lab_module
cmake --build build --target kernel_lab_help
```
