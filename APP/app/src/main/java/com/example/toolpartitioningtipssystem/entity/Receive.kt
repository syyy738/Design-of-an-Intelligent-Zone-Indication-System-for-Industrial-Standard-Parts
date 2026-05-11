package com.example.toolpartitioningtipssystem.entity

data class Receive(
    var wight_1: String? = null,// 区域一重量
    var wight_2: String? = null, // 区域二重量
    var ls: String? = null,// 单颗螺丝重量
    var lm: String? = null,// 单颗螺母重量
    var ls_v: String? = null,// 螺丝数量阈值
    var lm_v: String? = null,// 螺母数量阈值
    var door_1: String? = null,// 门一，添加螺丝的时候会打开，直到门关闭，记录一次添加的数量（0关闭，1打开）
    var door_2: String? = null,// 门二，添加螺母的时候会打开，直到门关闭，记录一次添加的数量0关闭，1打开）
    var category: String? = null,// 种类
)
