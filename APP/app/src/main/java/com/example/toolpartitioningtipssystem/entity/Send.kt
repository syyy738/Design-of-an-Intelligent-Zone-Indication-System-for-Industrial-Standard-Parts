package com.example.toolpartitioningtipssystem.entity

data class Send(
    var cmd: Int,
    var ls_v: Int? = null,// 螺丝数量阈值
    var lm_v: Int? = null,// 螺母数量阈值
    var ls: Int? = null,// 单个螺丝重量
    var lm: Int? = null,// 单个螺母重量
    var door_1: Int? = null,// 门的控制（螺丝）
    var door_2: Int? = null // 门的控制（螺母）
)
