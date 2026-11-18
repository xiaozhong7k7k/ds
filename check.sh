#!/usr/bin/env bash
# check.sh —— 一键编译 + 内存检查 + 试运行
# 用法：./check.sh           自动检查最近修改的 .c
#       ./check.sh d2        检查 w0_fix/d2*.c
#       ./check.sh w0_fix/x.c
set -u
cd "$(dirname "$0")" || exit 1

# ---- 找源文件 ----
if [ $# -ge 1 ]; then
  name="$1"
  if   [ -f "$name" ];            then src="$name"
  elif [ -f "w0_fix/$name" ];     then src="w0_fix/$name"
  elif [ -f "w0_fix/$name.c" ];   then src="w0_fix/$name.c"
  elif ls "w0_fix/$name"*.c >/dev/null 2>&1; then src=$(ls "w0_fix/$name"*.c | head -1)
  else echo "❌ 找不到文件：$name"; exit 1
  fi
else
  src=$(ls -t w0_fix/*.c 2>/dev/null | head -1)
  [ -z "$src" ] && { echo "❌ w0_fix 里没有 .c 文件"; exit 1; }
fi

base=$(basename "$src" .c)
out="bin/$base"
mkdir -p bin

echo "=== 1/3 编译：$src ==="
if ! gcc -Wall -Wextra -Werror -g "$src" -o "$out"; then
  echo
  echo "❌ 编译失败。先修上面的报错（VS Code 里是红线），不要运行。"
  exit 1
fi
echo "✅ 编译通过：零警告、零错误"

echo
echo "=== 2/3 内存检查（valgrind，输入用 5）==="
printf '5\n' | valgrind --leak-check=full "$out" >/tmp/check_out.txt 2>/tmp/check_err.txt
grep -E "All heap blocks were freed|definitely lost|Invalid (read|write)|ERROR SUMMARY" /tmp/check_err.txt | head -6
if grep -q "All heap blocks were freed" /tmp/check_err.txt && grep -q "ERROR SUMMARY: 0 errors" /tmp/check_err.txt; then
  echo "✅ 内存干净：没有泄漏，没有非法访问"
else
  echo "⚠️ 内存有问题，把上面几行发给我"
fi

echo
echo "=== 3/3 试运行（输入 5）==="
printf '5\n' | "$out"
echo "程序退出码=$?"
