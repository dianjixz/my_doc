# openclaw
正常配置后，可以在本地使用。但是期望能够远程使用，还是要添加一些配置。
https://www.normdist.com/2026/02/25/OpenClaw-LAN-Access-Guide/
可以先参考这个，
https://cloud.tencent.com/developer/article/2631562





`openclaw gateway config patch` 是**安全的局部配置修改工具**，只会更新你指定的字段，不会覆盖其他配置，执行后会自动校验配置合法性，校验通过才会保存并重启网关生效，非常适合增删改 `openclaw.json` 里的单个/部分字段。

### 基础语法
```bash
openclaw gateway config patch --patch '【JSON格式的修改片段】'
```

---

### 常见使用场景（结合你当前的配置举例）
#### 1️⃣ 新增/修改配置
直接按JSON层级写要新增/修改的内容即可，会自动和现有配置合并：
- 例：修改网关端口为18790
  ```bash
  openclaw gateway config patch --patch '{"gateway": {"port": 18790}}'
  ```
- 例：新增一个自定义模型提供商
  ```bash
  openclaw gateway config patch --patch '{"models": {"providers": {"my-custom-provider": {"baseUrl": "https://xxx.com/v1", "apiKey": "sk-xxx", "api": "openai-completions", "models": [{"id": "custom-model", "name": "自定义模型"}]}}}}'
  ```

#### 2️⃣ 删除配置
把要删除的字段的值设为 `null` 即可：
- 例：删除之前添加的 `dianjixz-deepseek` 提供商
  ```bash
  openclaw gateway config patch --patch '{"models": {"providers": {"dianjixz-deepseek": null}}}'
  ```
- 例：删除某个不需要的模型
  ```bash
  openclaw gateway config patch --patch '{"models": {"providers": {"m5stack": {"models": [{"id": "gemini-3.1-pro-preview", "remove": true}]}}}}'
  ```

#### 3️⃣ 修改深层嵌套字段
按层级写到要改的具体字段即可：
- 例：修改GLM-5模型的输入成本为0.3元/1M tokens
  ```bash
  openclaw gateway config patch --patch '{"models": {"providers": {"m5stack": {"models": [{"id": "glm-5", "cost": {"input": 0.3}}]}}}}'
  ```

---

### 常用搭配
1. 先查看当前配置确认字段路径：
   ```bash
   openclaw gateway config get
   ```
2. 修改完成后可以再次执行上面的命令确认修改是否生效。

### 注意
如果需要**完全替换整个配置文件**，可以用 `openclaw gateway config apply --raw '【完整的openclaw.json内容】'`，不过这个会覆盖全量配置，风险更高，日常修改推荐用 `patch` 即可。








