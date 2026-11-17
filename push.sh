#!/usr/bin/env bash
# push.sh —— 一键提交并上传到 GitHub
# 用法：./push.sh "这次做了什么"
set -u
cd "$(dirname "$0")" || exit 1

msg="${1:-}"
if [ -z "$msg" ]; then
  echo '用法：./push.sh "这次做了什么"'
  exit 1
fi

git add -A
if git diff --cached --quiet; then
  echo "没有新改动，不需要提交"
  exit 0
fi

echo "=== 这次要上传的文件 ==="
git diff --cached --name-status

git commit -m "$msg" || { echo "❌ 提交失败"; exit 1; }

echo
echo "=== 上传到 GitHub ==="
if git push; then
  echo "✅ 上传成功"
else
  echo "❌ 上传失败。把上面的报错发给我，不要自己反复试。"
  exit 1
fi
