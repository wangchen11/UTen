# UTen传输协议

## 基础包格式

### UTenProtocol

UTen数据包使用小端字节序1字节对齐，并照以下顺序排列

| 名称      | 类型                  | 字节数     | 描述               |
| --------- | --------------------- | ---------- | ------------------ |
| magicCode | int32_t               | 4          | 魔法值固定为'UTen' |
| checkSum  | int16_t               | 2          | 整个包校验和       |
| type      | enum UTenTypeCode :16 | 2          | 数据包类型         |
| dataSize  | int16_t               | 2          | 数据包大小         |
| data      | char[0]               | 0~(MTU-10) | 数据包内容         |

协议包大小最小值为10

### UTenTypeCode

包类型枚举值

| 名称                              | 值   | UTenProtocol->data结构    | 描述               |
| --------------------------------- | ---- | ------------------------- | ------------------ |
| UTEN_TYPE_MIN                     | 0    | 无                        | 小于等于此值无意义 |
| UTEN_TYPE_REPORT_INSIDER_REQUEST  | 1    | UTenReportInsiderRequest  | insider->host      |
| UTEN_TYPE_REPORT_INSIDER_RESPONSE | 2    | UTenReportInsiderResponse | host->insider      |
| UTEN_TYPE_MEET_INSIDER_REQUEST    | 3    | UTenMeetInsiderRequest    | outsider->host     |
| UTEN_TYPE_MEET_INSIDER_RESPONSE   | 4    | UTenMeetInsiderResponse   | host->outsider     |
| UTEN_TYPE_MEET_OUTSIDER_REQUEST   | 5    | UTenMeetOutsiderRequest   | host->insider      |
| UTEN_TYPE_PING_REQUEST            | 20   |                           |                    |
| UTEN_TYPE_PING_RESPONSE           | 21   |                           |                    |
| UTEN_TYPE_SEEK_REQUEST            | 22   |                           | outsider<->insider |
| UTEN_TYPE_SEEK_RESPONSE           | 23   |                           | outsider<->insider |
| UTEN_TYPE_DPK                     | 24   |                           | outsider<->insider |
| UTEN_TYPE_STREAM                  | 25   |                           | outsider<->insider |
| UTEN_TYPE_MAX                     | 26   |                           | 大于等于此值无意义 |

### UTenReportInsiderRequest

跳板机(Insider)报告自身ID

| 名称           | 类型     | 字节数 | 描述                          |
| -------------- | -------- | ------ | ----------------------------- |
| serisId        | uint32_t | 4      | 包ID。用于确认响应的是哪个包  |
| identifierCode | uint64_t | 8      | 自身ID。若为0则由服务器分配ID |

### UTenReportInsiderResponse

响应跳板机(Insider)报告自身ID

| 名称           | 类型                      | 字节数 | 描述                         |
| -------------- | ------------------------- | ------ | ---------------------------- |
| serisId        | uint32_t                  | 4      | 包ID。用于确认响应的是哪个包 |
| identifierCode | uint64_t                  | 8      | 跳板机ID                     |
| respCode       | enum UTenResponseCode :16 | 2      | 状态。0:SUCCESS  1:FAILED    |

### UTenMeetInsiderRequest

向服务器请求与跳板机连接

| 名称           | 类型     | 字节数 | 描述                         |
| -------------- | -------- | ------ | ---------------------------- |
| serisId        | uint32_t | 4      | 包ID。用于确认响应的是哪个包 |
| identifierCode | uint64_t | 8      | 跳板机ID                     |

### UTenMeetInsiderResponse



| 名称           | 类型                      | 字节数 | 描述                                                     |
| -------------- | ------------------------- | ------ | -------------------------------------------------------- |
| serisId        | uint32_t                  | 4      | 包ID。用于确认响应的是哪个包                             |
| identifierCode | uint64_t                  | 8      | 跳板机ID                                                 |
| respCode       | enum UTenResponseCode :16 | 2      | 状态。0:SUCCESS  1:FAILED                                |
| address        | char[MAX_ADDRESS_LEN]     | 128    | 服务器这边看到的跳板机地址。ip:port  MAX_ADDRESS_LEN=128 |

### UTenMeetOutsiderResponse

| 名称    | 类型                  | 字节数 | 描述                         |
| ------- | --------------------- | ------ | ---------------------------- |
| address | char[MAX_ADDRESS_LEN] | 128    | ip:port  MAX_ADDRESS_LEN=128 |