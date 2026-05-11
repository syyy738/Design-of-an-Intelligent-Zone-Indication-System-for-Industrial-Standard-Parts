# -*- coding: utf-8 -*-
'''
Script: deploy_det_video.py
脚本名称：deploy_det_video.py

Description:
    This script runs a real-time object detection application on an embedded device.
    It uses a pipeline to capture video frames, performs inference using a pre-trained Kmodel,
    and displays the detection results (bounding boxes, class labels) on screen.

    The model configuration is loaded from the Canaan online training platform via a JSON config file.

脚本说明：
    本脚本在嵌入式设备上运行实时目标检测应用。它通过捕获视频帧，使用预训练的 Kmodel 进行推理，并在屏幕上显示检测结果（边界框、类别标签）。

    模型配置文件通过 Canaan 在线训练平台从 JSON 文件加载。

Author: Canaan Developer
作者：Canaan 开发者
'''


import os, gc
from libs.PlatTasks import DetectionApp
from libs.PipeLine import PipeLine
from libs.Utils import *
from ybUtils.YbUart import YbUart
# 设置显示模式：可选项包括 'hdmi', 'lcd', 'lt9611', 'st7701', 'hx8399'
# 'hdmi' 默认为 'lt9611' (分辨率 1920x1080)；'lcd' 默认为 'st7701' (分辨率 800x480)
display_mode = "lt9611"

# 初始化串口通信，波特率为 115200
uart = YbUart(baudrate=115200)
# 向串口发送问候信息
uart.send("Hello k230\n")

# 定义 RGB888P 视频帧的输入尺寸
rgb888p_size = [1280, 720]

# 设置模型和配置文件的根目录路径
root_path = "/sdcard/mp_deployment_source/"

# 从 JSON 文件加载部署配置
deploy_conf = read_json(root_path + "/deploy_config.json")
# 获取 KModel 文件路径
kmodel_path = root_path + deploy_conf["kmodel_path"]
# 获取类别标签列表
labels = deploy_conf["categories"]
# 获取置信度阈值
confidence_threshold = deploy_conf["confidence_threshold"]
# 获取非极大值抑制阈值
nms_threshold = deploy_conf["nms_threshold"]
# 获取模型输入尺寸
model_input_size = deploy_conf["img_size"]
# 获取非极大值抑制策略
nms_option = deploy_conf["nms_option"]
# 获取检测模型类型
model_type = deploy_conf["model_type"]
# 初始化锚点列表
anchors = []
# 如果模型类型是基于锚点的检测器，则合并所有层级的锚点
if model_type == "AnchorBaseDet":
    anchors = deploy_conf["anchors"][0] + deploy_conf["anchors"][1] + deploy_conf["anchors"][2]

# 推理配置
inference_mode = "video"                                          # 推理模式：'video' 表示视频流处理
debug_mode = 0                                                    # 调试模式标志，0表示关闭调试

# 创建并初始化视频/显示管道
pl = PipeLine(rgb888p_size=rgb888p_size, display_mode=display_mode)
pl.create()
# 获取显示屏的实际尺寸
display_size = pl.get_display_size()

# 初始化目标检测应用程序
det_app = DetectionApp(
    inference_mode,           # 推理模式
    kmodel_path,              # KModel 文件路径
    labels,                   # 类别标签列表
    model_input_size,         # 模型输入尺寸
    anchors,                  # 锚点列表（如果适用）
    model_type,               # 模型类型
    confidence_threshold,     # 置信度阈值
    nms_threshold,            # 非极大值抑制阈值
    rgb888p_size,             # 原始RGB帧尺寸
    display_size,             # 显示屏尺寸
    debug_mode=debug_mode     # 调试模式
)

# 配置模型预处理参数
det_app.config_preprocess()

# 主循环：持续捕获帧、运行推理、显示结果
while True:
    # 使用计时器测量整个循环的总耗时，打印间隔为1秒
    with ScopedTiming("total", 1):
        # 从管道获取当前视频帧
        img = pl.get_frame()
        # 对当前帧运行目标检测推理
        res = det_app.run(img)
        # 在 OSD 图像上绘制检测结果（边界框、标签、置信度）
        det_app.draw_result(pl.osd_img, res)
        # 将带有检测结果的图像显示在屏幕上
        pl.show_image()
        # 强制执行垃圾回收，释放内存
        gc.collect()

# 清理资源：这些清理代码只有在循环被中断时才会执行（例如IDE停止或外部中断）
det_app.deinit()                                      # 反初始化检测应用程序，释放模型资源
pl.destroy()                                          # 销毁管道实例，释放摄像头和显示资源

