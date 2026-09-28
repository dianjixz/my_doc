#!/usr/bin/env bash

# bash <(curl -sL 'https://gist.github.com/dianjixz/4aa5018d2d81f67251a337f838049661/raw/a8abafe42b8c56c6df951132d8d8306ff85ab939/install_systemd.sh')
# SERVICE_NAME="myttyd" BINARY_PATH="/usr/local/bin/ttyd" bash <(curl -sL 'https://gist.github.com/dianjixz/4aa5018d2d81f67251a337f838049661/raw/a8abafe42b8c56c6df951132d8d8306ff85ab939/install_systemd.sh')

set -euo pipefail

# ===== 配置变量 =====
SERVICE_NAME="${SERVICE_NAME:-ttyd}"
BINARY_PATH="${BINARY_PATH:-/home/pi/ttyd.aarch64}"
EXEC_ARGS="${EXEC_ARGS:--W bash}"
RUN_USER="${RUN_USER:-pi}"          # 仅在系统级模式下生效；留空则不写入 User= 字段

# ===== 确定运行模式 =====
# 未传参数时：root → system，普通用户 → user
# 也可显式指定: ./install.sh system  或  ./install.sh user
if [[ -z "${1:-}" ]]; then
  if [[ "${EUID}" -eq 0 ]]; then
    MODE="system"
  else
    MODE="user"
  fi
else
  MODE="$1"
fi

echo ">>> 安装模式: ${MODE}"
echo ">>> 服务名称: ${SERVICE_NAME}"

# ===== 检查二进制是否存在 =====
if [[ ! -f "${BINARY_PATH}" ]]; then
  echo "错误: 找不到 ${BINARY_PATH}"
  exit 1
fi
chmod +x "${BINARY_PATH}"

# ===== 根据模式确定路径与命令 =====
case "${MODE}" in
  system)
    if [[ "${EUID}" -ne 0 ]]; then
      echo "错误: 系统级模式请使用 sudo 运行此脚本"
      exit 1
    fi
    SERVICE_DIR="/etc/systemd/system"
    SYSTEMCTL="systemctl"
    WANTED_BY="multi-user.target"
    ;;
  user)
    if [[ "${EUID}" -eq 0 ]]; then
      echo "错误: 用户级模式请勿使用 sudo / root 运行"
      exit 1
    fi
    SERVICE_DIR="${HOME}/.config/systemd/user"
    SYSTEMCTL="systemctl --user"
    WANTED_BY="default.target"
    RUN_USER=""   # 用户级服务不需要指定 User=
    mkdir -p "${SERVICE_DIR}"
    ;;
  *)
    echo "用法: $0 [system|user]"
    exit 1
    ;;
esac

SERVICE_FILE="${SERVICE_DIR}/${SERVICE_NAME}.service"

# ===== 组装 [Service] 段（User= 可选）=====
USER_LINE=""
if [[ -n "${RUN_USER}" ]]; then
  USER_LINE="User=${RUN_USER}"
fi

# ===== 写入服务文件 =====
cat > "${SERVICE_FILE}" <<EOF
[Unit]
Description=ttyd Web Terminal
After=network.target

[Service]
${USER_LINE}
ExecStart=${BINARY_PATH} ${EXEC_ARGS}
Restart=always
RestartSec=3

[Install]
WantedBy=${WANTED_BY}
EOF

echo ">>> 服务文件已创建: ${SERVICE_FILE}"

# ===== 重载并启用 =====
${SYSTEMCTL} daemon-reload
${SYSTEMCTL} enable --now "${SERVICE_NAME}.service"

echo ">>> 完成！服务状态："
${SYSTEMCTL} status "${SERVICE_NAME}.service" --no-pager