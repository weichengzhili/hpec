# system 场景集成方案

> 五大落地场景的系统级接口聚合。

| 模块 | 场景 | 接口文件 |
|------|------|----------|
| [汽车域控](automotive/汽车域控.md) | 座舱/ADAS/车身域控 | include/automotive_system.hpp |
| [工业网关](gateway/工业网关.md) | 多协议、边缘计算、上云 | include/gateway_system.hpp |
| [IoT边缘节点](iot-edge/IoT边缘节点.md) | 边缘推理、无线、低功耗 | include/iot_edge.hpp |
| [AI边缘部署](ai-deploy/AI边缘部署.md) | 模型转换、NPU、MLOps | include/ai_deploy.hpp |
| [工业机器人](robot/工业机器人.md) | 运动控制、实时、视觉伺服 | include/robot_system.hpp |
