# 最终记录补齐

本次原样归档完整 progress/ARM_LZMA_0921B，包括最终报告与已结束的两轮 GDB 记录。原报告中的“尚未推送”是写成当时的历史状态，未改原文。

以 FINAL_DELIVERY_MANIFEST.json 为最终完整清单；每个 stored 列表按序拼接、gzip 解压后，应与对应 sha256 相符。超过 16000000 字节的文件以 .final.gz.part-* 无损分片存储，全部已解压校验。未隐去任何源文件。

旧 DELIVERY_MANIFEST.json 与旧 .gz.part-* 是运行中快照的历史材料，保留不删除；其中部分未压缩路径已更新为最终内容，故不要用旧清单校验最终快照。最终清单才对应本次交付。
