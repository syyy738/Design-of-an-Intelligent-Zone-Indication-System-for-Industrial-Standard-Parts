package com.example.toolpartitioningtipssystem.adapter

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import com.example.toolpartitioningtipssystem.databinding.ListUserItemBinding
import com.example.toolpartitioningtipssystem.entity.User

class UserListViewAdapter(private val context: Context, private val list: MutableList<Any>) :
    BaseAdapter() {
    override fun getCount(): Int {
        return list.size
    }

    override fun getItem(p0: Int): Any {
        return list[p0]
    }

    override fun getItemId(p0: Int): Long {
        return p0.toLong()
    }

    override fun getView(p0: Int, p1: View?, p2: ViewGroup?): View {
        var view = p1
        val holder: ViewHolder
        if (view == null) {
            val binding = ListUserItemBinding.inflate(LayoutInflater.from(context), p2, false)
            view = binding.root
            holder = ViewHolder(binding)
            view.tag = holder
        } else {
            holder = view.tag as ViewHolder
        }

        initViews(holder.binding, p0)

        return view
    }

    private fun initViews(binding: ListUserItemBinding, index: Int) {
        val user = list[index] as User
        binding.nameText.text = user.name
        binding.accountText.text = user.account
        binding.createTimeText.text = user.createDateTime

    }


    private class ViewHolder(val binding: ListUserItemBinding)
}