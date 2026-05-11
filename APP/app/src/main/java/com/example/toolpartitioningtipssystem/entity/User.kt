package com.example.toolpartitioningtipssystem.entity

data class User(
    var uid: Int? = null,
    var account: String? = null,//账号
    var password: String? = null,//密码
    var name: String? = null,//姓名
    var location: String? = null,//职位 1 管理员  2 教师 3 学生
    var phone: String? = null,// 开门密码
    var sex: String? = null,// 性别
    var createDateTime: String? = null
)
