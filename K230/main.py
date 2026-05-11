# -*- coding: utf-8 -*-
'''
Script: main.py
脚本名称：main.py

Description:
    This script runs a real-time object detection application on K230.
    It uses a pipeline to capture video frames, performs inference using a pre-trained Kmodel,
    displays the detection results (bounding boxes, class labels) on screen,
    and sends detection results to the lower computer via UART.

脚本说明：
    本脚本在 K230 上运行实时目标检测应用。它通过捕获视频帧，使用预训练的 Kmodel 进行推理，
    在屏幕上显示检测结果（边界框、标签、类别标签），并通过串口向下位机发送检测结果。
'''

import os, gc, time
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

# 计数变量
Counter = 0  # 计数器
ls = 0  # 标签 1 的检测状态（0 或 1）
lm = 0  # 标签 2 的检测状态（0 或 2）
kind = 0  # 检测结果组合值（ls + lm）
last_send_time = 0  # 上次发送时间

# 设置模型和配置文件的根目录路径
root_path = "/sdcard/mp_deployment_source/"

# 从 JSON 文件加载部署配置
deploy_conf = read_json(root_path + "/deploy_config.json")
# 获取 KModel 文件路径
model_path = root_path + deploy_conf["kmodel_path"]
# 获取类别标签列表
labels = deploy_conf["categories"]
# 获取置信度阈值
confidence_threshold = deploy_conf["confidence_threshold"]
# 获取非极大值抑制阈值
nms_threshold = deploy_conf["nms_threshold"]
# 获取模型输入尺寸
model_input_size = deploy_conf["img_size"]
# 获取检测模型类型
model_type = deploy_conf["model_type"]
# 初始化锚点列表
anchors = []
# 如果模型类型是基于锚点的检测器，则合并所有层级的锚点
if model_type == "AnchorBaseDet":
    anchors = deploy_conf["anchors"][0] + deploy_conf["anchors"][1] + deploy_conf["anchors"][2]

# 推理配置
inference_mode = "video"  # 推理模式：'video' 表示视频流处理
debug_mode = 0  # 调试模式标志，0 表示关闭调试

# 创建并初始化视频/显示管道
pl = PipeLine(rgb888p_size=rgb888p_size, display_mode=display_mode)
pl.create()
# 获取显示屏的实际尺寸
display_size = pl.get_display_size()

# 初始化目标检测应用程序
det_app = DetectionApp(
    inference_mode,           # 推理模式
    model_path,               # KModel 文件路径
    labels,                   # 类别标签列表
    model_input_size,         # 模型输入尺寸
    anchors,                  # 锚点列表（如果适用）
    model_type,               # 模型类型
    confidence_threshold,     # 置信度阈值
    nms_threshold,            # 非极大值抑制阈值
    rgb888p_size,             # 原始 RGB 帧尺寸
    display_size,             # 显示屏尺寸
    debug_mode=debug_mode     # 调试模式
)

# 配置模型预处理参数
det_app.config_preprocess()

# 主循环：持续捕获帧、运行推理、显示结果
while True:
    # 获取当前时间
    current_time = time.ticks_ms()
    
    # 每秒发送一次检测结果
    if time.ticks_diff(current_time, last_send_time) >= 1000:
        Counter += 1
        kind = ls + lm
        # 向下位机发送识别结果
        uart.send('kind:' + str(kind) + "\n")
        print('kind:' + str(kind) + "\n")
        # 重置检测状态
        ls = 0
        lm = 0
        last_send_time = current_time
    
    # 从管道获取当前视频帧
    img = pl.get_frame()
    # 对当前帧运行目标检测推理
    res = det_app.run(img)
    
    # 处理检测结果
    current_ls = 0
    current_lm = 0
    if res["boxes"]:
        for i in range(len(res["boxes"])):
            label_idx = res["idx"][i]
            if label_idx < len(labels):
                label = labels[label_idx]
                # 根据检测到的标签更新 ls 和 lm 值
                # 假设标签 1 是螺丝 (bolt)，标签 2 是螺母 (nut)
                if label == 'bolt' or label == 'screw':
                    current_ls = 1  # 检测到标签 1（螺丝）
                if label == 'nut':
                    current_lm = 2  # 检测到标签 2（螺母）
                print("Detected label:", label)
    
    # 更新全局变量
    ls = current_ls
    lm = current_lm
    
    # 在 OSD 图像上绘制检测结果（边界框、标签、置信度）
    det_app.draw_result(pl.osd_img, res)
    
    # 在图像上显示检测结果
    pl.osd_img.draw_string_advanced(10, 10, 24, 'Kind: ' + str(kind), color=(255, 255, 255, 255))
    
    # 将带有检测结果的图像显示在屏幕上
    pl.show_image()
    
    # 强制执行垃圾回收，释放内存
    gc.collect()

# 清理资源：这些清理代码只有在循环被中断时才会执行（例如 IDE 停止或外部中断）
det_app.deinit()  # 反初始化检测应用程序，释放模型资源
pl.destroy()      # 销毁管道实例，释放摄像头和显示资源
uart.deinit()     # 关闭串口
print("Resources released")
