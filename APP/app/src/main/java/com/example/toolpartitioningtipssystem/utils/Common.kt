package com.example.toolpartitioningtipssystem.utils

import android.content.Context
import android.util.Log
import com.example.toolpartitioningtipssystem.entity.Receive
import com.example.toolpartitioningtipssystem.entity.Send
import com.example.toolpartitioningtipssystem.entity.User
import com.google.gson.Gson
import com.itfitness.mqttlibrary.MQTTHelper

object Common {

//    const val PORT = "6002" // mqtt服务器端口号
//    const val SERVER_ADDRESS = "192.168.137.1"// mqtt 服务器地址

    const val PORT = "1883" // mqtt服务器端口号
    const val SERVER_ADDRESS = "broker.emqx.io"// mqtt 服务器地址
    const val URL = "tcp://$SERVER_ADDRESS:$PORT" // mqtt连接地址
    const val RECEIVE_TOPIC = "/broadcast/h9sjbUhCD1T/test2" // 接收消息订阅的主题 - 下位机发送消息的主题
    const val PUSH_TOPIC = "/broadcast/h9sjbUhCD1T/test1" // 推送消息的主题 - 下位机接收消息的主题
    const val DRIVER_ID = "APP_Client_2026" // mqtt id
    const val DRIVER_NAME = "app_client" // mqtt 用户名 （oneNET中为产品ID）
    const val DRIVER_PASSWORD = "password" // mqtt 鉴权或者密码
    const val DRIVER_ID_HARDWARE = "1211070683" // mqtt 硬件id
    const val API_KEY = "HPtgv9tFnmoLlt=suai8ogSEwPg=" // （oneNET） APIkey
    var HARDWARE_ONLINE = false // 硬件在线标志位
    var mqttHelper: MQTTHelper? = null // mqtt 连接服务函数
    var user: User? = null
    var registryFlag = false
    var receive: Receive? = null

    /***
     * 随机生成四位数密码
     */
    fun randomCipher(): String {
        return (1000..9999).random().toString()
    }

    /***
     * @brief 包装发送函数，只有建立了连接才发送消息
     */
    fun sendMessage(context: Context, cmd: Int, vararg data: String): String {
        return if (mqttHelper == null || !mqttHelper!!.connected) {
            MToast.mToast(context, "未建立连接")
            ""
        } else {
            try {
                val send = Send(cmd = cmd)
                when (cmd) {
                    1 -> {
                        send.ls_v = data[0].toInt()
                        send.lm_v = data[1].toInt()
                    }

                    2 -> {
                        send.door_1 = data[0].toInt()
                    }

                    3 -> {
                        send.door_2 = data[0].toInt()
                    }

                    4 -> {
                        send.ls = data[0].toInt()
                        send.lm = data[1].toInt()
                    }
                }
                val result = Gson().toJson(send)
                mqttHelper!!.publish(PUSH_TOPIC, result, 1)
                result
            } catch (e: Exception) {
                e.printStackTrace()
                Log.e("发送错误", e.message.toString())
                MToast.mToast(context, "数据发送失败")
                ""
            }
        }
    }

}