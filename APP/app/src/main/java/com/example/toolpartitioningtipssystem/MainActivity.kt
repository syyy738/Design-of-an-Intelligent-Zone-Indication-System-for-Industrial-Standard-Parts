package com.example.toolpartitioningtipssystem

import android.content.Intent
import android.content.SharedPreferences
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.widget.ArrayAdapter
import android.widget.SeekBar
import androidx.appcompat.app.AlertDialog
import com.blankj.utilcode.util.LogUtils
import com.example.toolpartitioningtipssystem.databinding.ActivityMainBinding
import com.example.toolpartitioningtipssystem.databinding.UpdateNumBinding
import com.example.toolpartitioningtipssystem.db.HistoryDao
import com.example.toolpartitioningtipssystem.entity.History
import com.example.toolpartitioningtipssystem.entity.Receive
import com.example.toolpartitioningtipssystem.utils.BeatingAnimation
import com.example.toolpartitioningtipssystem.utils.Common
import com.example.toolpartitioningtipssystem.utils.CustomBottomSheetDialogFragment
import com.example.toolpartitioningtipssystem.utils.MToast
import com.example.toolpartitioningtipssystem.utils.TimeCycle
import com.google.gson.Gson
import com.gyf.immersionbar.ImmersionBar
import com.itfitness.mqttlibrary.MQTTHelper
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken
import org.eclipse.paho.client.mqttv3.MqttCallback
import org.eclipse.paho.client.mqttv3.MqttMessage
import org.greenrobot.eventbus.EventBus
import kotlin.math.round
class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding
    private var isDebugView = false //是否显示debug界面
    private val arrayList = mutableListOf<String>() // debug消息数据
    private var adapter: ArrayAdapter<*>? = null // debug消息适配器
    private lateinit var sharedPreferences: SharedPreferences // 临时存储
    private lateinit var editor: SharedPreferences.Editor // 修改提交
    private lateinit var dao: HistoryDao

    private var door1 = "1"
    private var door2 = "1"
    private var  F = false
    private var  G = false
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        sharedPreferences = getSharedPreferences("local", MODE_PRIVATE)
        editor = sharedPreferences.edit()
        dao = HistoryDao(this)
        initViews()
        mqttConfig()
    }

    private fun initViews() {
        setSupportActionBar(binding.toolbar)
        binding.toolbarLayout.title = title
        ImmersionBar.with(this).init()
        binding.lsText.text = sharedPreferences.getString("lsNum", "1")
        binding.lmText.text = sharedPreferences.getString("lmNum", "1")
        binding.lsSeekBar.progress = sharedPreferences.getInt("ls_v", 50)
        binding.lsSeekBarText.text = binding.lsSeekBar.progress.toString()
        binding.lmSeekBar.progress = sharedPreferences.getInt("lm_v", 50)
        binding.lmSeekBarText.text = binding.lmSeekBar.progress.toString()
        debugView()
        eventManager()
    }

    private fun eventManager() {
        binding.warringLayout.setOnClickListener {
            warringLayout(false, ":")
        }

        binding.lsText.setOnClickListener {
            val builder = AlertDialog.Builder(this)
            val view = UpdateNumBinding.inflate(LayoutInflater.from(this))
            builder.setTitle("修改螺丝重量").setView(view.root)
            val dialog = builder.create()
            view.confirmButton.setOnClickListener {
                val num = view.inputNumEdit.text.toString()
                if (isValidInput(num)) {
                    binding.lsText.text = num
                    debugViewData(
                        1, Common.sendMessage(
                            this@MainActivity,
                            4,
                            binding.lsText.text.toString(),
                            binding.lmText.text.toString()
                        )
                    )
                    editor.putString("lsNum", num)
                    editor.commit()
                    MToast.mToast(this, "修改成功！")
                    dialog.dismiss()
                } else {
                    MToast.mToast(this, "请输入规则内的数字！")
                }

            }
            view.cancelButton.setOnClickListener {
                dialog.dismiss()
            }
            dialog.show()
        }


        binding.lmText.setOnClickListener {
            val builder = AlertDialog.Builder(this)
            val view = UpdateNumBinding.inflate(LayoutInflater.from(this))
            builder.setTitle("修改螺母重量").setView(view.root)
            val dialog = builder.create()

            view.confirmButton.setOnClickListener {
                val num = view.inputNumEdit.text.toString()
                if (isValidInput(num)) {
                    binding.lmText.text = num
                    debugViewData(
                        1, Common.sendMessage(
                            this@MainActivity,
                            4,
                            binding.lsText.text.toString(),
                            binding.lmText.text.toString()
                        )
                    )
                    editor.putString("lmNum", num)
                    editor.commit()
                    MToast.mToast(this, "修改成功！")
                    dialog.dismiss()
                } else {
                    MToast.mToast(this, "请输入规则内的数字！")
                }

            }
            view.cancelButton.setOnClickListener {
                dialog.dismiss()
            }
            dialog.show()
        }

        binding.button1.setOnClickListener {
            debugViewData(
                1, Common.sendMessage(
                    this@MainActivity,
                    2, door1
                )
            )
        }

        binding.button2.setOnClickListener {
            debugViewData(
                1, Common.sendMessage(
                    this@MainActivity,
                    3, door2
                )
            )
        }

        binding.lmSeekBar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(p0: SeekBar?, p1: Int, p2: Boolean) {
                binding.lmSeekBarText.text = p1?.toString()
            }

            override fun onStartTrackingTouch(p0: SeekBar?) {

            }

            override fun onStopTrackingTouch(p0: SeekBar?) {
                binding.lmSeekBarText.text = p0?.progress.toString()
                debugViewData(
                    1, Common.sendMessage(
                        this@MainActivity,
                        1,
                        binding.lsSeekBarText.text.toString(),
                        binding.lmSeekBarText.text.toString()
                    )
                )
                editor.putInt("lm_v", p0?.progress!!)
                editor.commit()
            }
        })

        binding.lsSeekBar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(p0: SeekBar?, p1: Int, p2: Boolean) {
                binding.lsSeekBarText.text = p1?.toString()
            }

            override fun onStartTrackingTouch(p0: SeekBar?) {

            }

            override fun onStopTrackingTouch(p0: SeekBar?) {
                binding.lsSeekBarText.text = p0?.progress.toString()
                debugViewData(
                    1, Common.sendMessage(
                        this@MainActivity,
                        1,
                        binding.lsSeekBarText.text.toString(),
                        binding.lmSeekBarText.text.toString()
                    )
                )
                editor.putInt("ls_v", p0?.progress!!)
                editor.commit()
            }
        })
    }

    /**
     * @brief debug界面的初始化
     */
    private fun debugView() {
        adapter = ArrayAdapter(this, android.R.layout.simple_list_item_1, arrayList)
        binding.debugView.adapter = adapter
    }

    /**
     * @param str  如果为 1 添加发送数据到界面   为 2 添加接受消息到界面
     * @param data 数据字符串
     * @brief debug界面数据添加
     */
    private fun debugViewData(str: Int, data: String) {
        if (arrayList.size >= 255) {
            arrayList.clear()
        }
        runOnUiThread {
            when (str) {
                1 -> arrayList.add("时间:${TimeCycle.getDateTime()}发送消息:$data".trimIndent())
                2 -> arrayList.add("时间:${TimeCycle.getDateTime()}接到消息:$data".trimIndent())
            }
            // 在添加新数据之后调用以下方法，滚动到列表底部
            binding.debugView.post { binding.debugView.setSelection(if (adapter != null) adapter!!.count - 1 else 0) }
            adapter?.notifyDataSetChanged()
        }
    }

    /**
     * @brief 显示警告弹窗和设置弹窗内容
     * @param visibility 是否显示
     * @param str 显示内容
     */
    private fun warringLayout(visibility: Boolean, str: String) {
        if (visibility) {
            binding.warringLayout.visibility = View.VISIBLE
            binding.warringText.text = str
            BeatingAnimation().onAnimation(binding.warringImage)
        } else {
            binding.warringLayout.visibility = View.GONE
        }
    }

    /****
     * @brief 配置mqtt连接参数并进行连接
     */
    private fun mqttConfig() {
        if (Common.mqttHelper == null) {
            // 配置mqtt参数
            Common.mqttHelper = MQTTHelper(
                this,
                Common.URL,
                Common.DRIVER_ID,
                Common.DRIVER_NAME,
                Common.DRIVER_PASSWORD,
                true,
                30,
                30
            )
            try {
                // 尝试连接mqtt服务器
                Common.mqttHelper!!.connect(Common.RECEIVE_TOPIC, 1, true, object : MqttCallback {
                    override fun connectionLost(cause: Throwable?) {
                        // 连接中断或丢失时触发
                    }

                    //接受到消息时触发
                    override fun messageArrived(topic: String?, message: MqttMessage?) {
                        LogUtils.eTag(
                            "接收到消息-未解码",
                            if (message!!.payload != null) String(message.payload) else ""
                        )

                        val receive = message.toString()
                        //数据转换
                        val data: Receive = Gson().fromJson(receive, Receive::class.java)
                        LogUtils.eTag(
                            "接收到消息-解码", if (message.payload != null) data else ""
                        )
                        debugViewData(2, receive)
                        dataAnalysis(data)
                    }

                    // 消息发送完成时触发
                    override fun deliveryComplete(token: IMqttDeliveryToken?) {

                    }

                })
            } catch (e: Exception) {
                e.printStackTrace()
                Log.e("mqttConfig()", e.message.toString())
                MToast.mToast(this, "连接时发生错误")
            }
        }
    }

    /**
     * @brief 解析接受到的数据
     */
    private fun dataAnalysis(data: Receive?) {
        if (data != null) {
            if (data.category != null) {
                if(data.category  == "1"){
                    binding.category.text = "螺丝"
                }
                else if(data.category  == "2"){
                    binding.category.text = "螺母"
                }
                else if(data.category  == "3"){
                    binding.category.text = "混合"
                }
                else
                {
                    binding.category.text = "暂无"
                }
            }

            if (data.door_1 != null) {
                if(data.door_1  == "1"){
                    binding.button1.text = "关闭"
                    door1 = "0"
                }else if(data.door_1  == "0")
                {
                    if (door1 == "0") {
                        dao.insert(History(type = "螺丝数量", weight = data.wight_1))
                    }
                    door1 = "1"
                    binding.button1.text = "打开"
                }
            }

            if (data.door_2 != null) {
                if(data.door_2  == "1"){
                    binding.button2.text = "关闭"
                    door2 = "0"

                }else if(data.door_2  == "0")
                {
                    if (door2 == "0") {
                        dao.insert(History(type = "螺母数量", weight = data.wight_2))
                    }
                    door2 = "1"
                    binding.button2.text = "打开"
                }
            }
            if (data.lm != null) {
                binding.lmText.text = data.lm
                editor.putString("lmNum", data.lm)
                editor.commit()
            }
            if (data.ls != null) {
                binding.lsText.text = data.ls
                editor.putString("lsNum", data.ls)
                editor.commit()
            }

            if (data.wight_2 != null) {

                binding.area2Value.text = data.wight_2
            }
            if (data.wight_1 != null) {

                binding.area1Value.text = data.wight_1
            }
            if (data.lm_v != null) {
                binding.lmSeekBar.progress = data.lm_v!!.toInt()
                binding.lmSeekBarText.text = data.lm_v!!.toString()
                editor.putInt("lm_v", data.lm_v!!.toInt())
                editor.commit()
            }

            if (data.ls_v != null) {
                binding.lsSeekBar.progress = data.ls_v!!.toInt()
                binding.lsSeekBarText.text = data.ls_v!!.toString()
                editor.putInt("ls_v", data.ls_v!!.toInt())
                editor.commit()
            }
            if (data.wight_1 != null && data.ls != null) {
                val ls_n = data.wight_1!!.toFloat() / data.ls!!.toInt()
                binding.area1Num.text = round(ls_n).toInt().toString()
                F = ls_n < binding.lsSeekBarText.text.toString().toInt()


            }
            if (data.wight_2 != null && data.lm != null) {
                val lm_n = data.wight_2!!.toFloat() / data.lm!!.toInt()
                binding.area2Num.text = round(lm_n).toInt().toString()
                G = lm_n < binding.lmSeekBarText.text.toString().toInt()


            }
            if (G || F){
                if (G){
                    warringLayout(true, "螺母数量不够")
                }
                if (F){
                    warringLayout(true, "螺丝数量不够")
                }
            }
            else
                warringLayout(false, "螺母数量不够")



        }
    }


    //填充右上角目录
    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        menuInflater.inflate(R.menu.menu_scrolling, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        when (item.itemId) {
            R.id.debugView -> {
                // 调试窗口
                isDebugView = !isDebugView
                binding.debugView.visibility = if (isDebugView) View.VISIBLE else View.GONE
            }

            R.id.historyView -> {
                //历史记录
                val customBottomSheetDialogFragment =
                    CustomBottomSheetDialogFragment(0, 0)
                customBottomSheetDialogFragment.show(
                    supportFragmentManager,
                    customBottomSheetDialogFragment.tag
                )
            }
        }

        return super.onOptionsItemSelected(item)
    }

    /**
     * @brief 匹配正则 只能输入数字
     */
    private fun isValidInput(input: String): Boolean {
        val regex = "^[1-9][0-9]*$".toRegex()   // 创建正则表达式对象
        return regex.matches(input)  // 检查输入是否匹配正则表达式
    }
}