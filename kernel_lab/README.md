# kernel_lab

这是一个独立的 Linux 内核实验模块，适合在当前仓库里单独练习几个常见的内核开发点：

- `module_param` / `module_param_string`
- `procfs` 节点创建与读写
- `copy_from_user`
- `jiffies` 与简单时间信息
- 模块的 `init` / `exit`

## 构建

先在项目根目录执行：

```bash
cmake -S . -B build
cmake --build build --target kernel_lab_module
```

如果系统已经安装当前内核版本的 headers，模块会生成在：

```bash
kernel_lab/kernel_lab.ko
```

## 加载与交互

```bash
sudo insmod kernel_lab/kernel_lab.ko greeting="hello kernel" repeat=3
cat /proc/kernel_lab
echo "new message from userspace" | sudo tee /proc/kernel_lab
cat /proc/kernel_lab
sudo rmmod kernel_lab
```

## CMake 目标

- `kernel_lab_module`：编译 `.ko`
- `kernel_lab_clean`：清理内核模块构建产物
- `kernel_lab_help`：打印常用命令提示
