package com.example.toolpartitioningtipssystem.db

import android.content.ContentValues
import android.content.Context
import android.database.Cursor
import android.database.sqlite.SQLiteDatabase
import android.util.Log
import com.example.toolpartitioningtipssystem.entity.User
import com.example.toolpartitioningtipssystem.utils.MToast
import com.example.toolpartitioningtipssystem.utils.TimeCycle

class UserDao(private val context: Context) : BaseDao {
    private var helper = DBOpenHelper(context)
    private lateinit var db: SQLiteDatabase
    private val TAG = "UserDao"

    override fun insert(bean: Any): Int {
        return try {
            val user: User = bean as User
            db = helper.writableDatabase
            val values = ContentValues()
            values.put("account", user.account)
            values.put("name", user.name)
            values.put("password", user.password)
            values.put("location", user.location)
            values.put("phone", user.phone)
            values.put("sex", user.sex)
            values.put("createDateTime", TimeCycle.getDateTime())
            db.insert("user", null, values)
            db.close()
            1
        } catch (e: Exception) {
            e.printStackTrace()
            MToast.mToast(context, "添加失败")
            -1
        }
    }

    override fun update(bean: Any, vararg data: String): Int {
        return try {
            val user = bean as User
            db = helper.writableDatabase
            val values = ContentValues()
            values.put("name", user.name)
            values.put("password", user.password)
            values.put("location", user.location)
            values.put("phone", user.phone)
            values.put("sex", user.sex)
            db.update("user", values, "uid=?", arrayOf(data[0]))
            db.close()
            1
        } catch (e: java.lang.Exception) {
            e.printStackTrace()
            MToast.mToast(context, "修改失败")
            -1
        }
    }

    override fun query(vararg data: String): MutableList<Any>? {
        return try {
            val result: MutableList<Any> = ArrayList()
            val cursor: Cursor
            val sql: String
            db = helper.readableDatabase
            when (data.size) {
                1 -> {
                    sql = "SELECT * from user where uid = ? order by createDateTime desc"
                    cursor = db.rawQuery(sql, arrayOf(data[0]))
                }
                2 ->  if (data[1] == "account") {
                    sql = "SELECT * FROM user where account = ? order by createDateTime desc"
                    cursor = db.rawQuery(sql, arrayOf(data[0]))
                }else if (data[1] == "name") {
                    sql = "SELECT * FROM user where name = ? order by createDateTime desc"
                    cursor = db.rawQuery(sql, arrayOf(data[0]))
                } else {
                    sql =
                        "SELECT * FROM user where account = ? and password = ? order by createDateTime desc"
                    cursor = db.rawQuery(sql, data)
                }

                else -> {
                    sql = "select * from user order by createDateTime desc"
                    cursor = db.rawQuery(sql, null)
                }
            }
            while (cursor.moveToNext()) {
                val user = User(
                    cursor.getInt(cursor.getColumnIndexOrThrow("uid")),
                    cursor.getString(cursor.getColumnIndexOrThrow("account")),
                    cursor.getString(cursor.getColumnIndexOrThrow("password")),
                    cursor.getString(cursor.getColumnIndexOrThrow("name")),
                    cursor.getString(cursor.getColumnIndexOrThrow("location")),
                    cursor.getString(cursor.getColumnIndexOrThrow("phone")),
                    cursor.getString(cursor.getColumnIndexOrThrow("sex")),
                    cursor.getString(cursor.getColumnIndexOrThrow("createDateTime"))
                )
                result.add(user)
            }
            cursor.close()
            db.close()
            result
        } catch (e: java.lang.Exception) {
            e.printStackTrace()
            Log.e("查询", e.toString())
            MToast.mToast(context, "查询失败")
            null
        }
    }

    override fun delete(vararg data: String): Int {
        return try {
            if (data.isEmpty()) {
                return 0
            }
            db = helper.writableDatabase
            db.delete("user", "uid=?", arrayOf(data[0]))
            db.close()
            1
        } catch (e: java.lang.Exception) {
            e.printStackTrace()
            MToast.mToast(context, "删除失败")
            -1
        }
    }
}