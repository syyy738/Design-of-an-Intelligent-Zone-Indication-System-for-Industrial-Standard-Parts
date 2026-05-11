package com.example.toolpartitioningtipssystem.utils

import android.graphics.Typeface
import android.os.Bundle
import android.util.TypedValue
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.example.toolpartitioningtipssystem.adapter.UserListViewAdapter
import com.example.toolpartitioningtipssystem.databinding.BottomSheetDialogFrgmentLayoutBinding
import com.example.toolpartitioningtipssystem.db.UserDao
import com.example.toolpartitioningtipssystem.adapter.HistoryListViewAdapter
import com.example.toolpartitioningtipssystem.db.HistoryDao

import com.google.android.material.bottomsheet.BottomSheetDialogFragment


class CustomBottomSheetDialogFragment(private val type: Int, private val id: Int) :
    BottomSheetDialogFragment(),
    HandlerAction {
    private lateinit var binding: BottomSheetDialogFrgmentLayoutBinding
    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?
    ): View {


        // 填充底部弹窗的布局文件
        binding = BottomSheetDialogFrgmentLayoutBinding.inflate(
            inflater, container, false
        )

        when (type) {
            0 -> { // 查询记录
                val dao = HistoryDao(requireContext())
                val list = dao.query()
                if (list != null) {
                    if (list.size > 0) {
                        binding.settingList.adapter = HistoryListViewAdapter(requireContext(), list)
                        binding.settingList.setOnItemClickListener { parent, view, position, id ->
                        }
                    } else {
                        dismiss()
                        MToast.mToast(requireContext(), "还没有数据")
                    }
                } else {
                    dismiss()
                    MToast.mToast(requireContext(), "还没有数据")
                }
            }

            else -> { //
                val dao = UserDao(requireContext())
                val list = dao.query()
                if (list != null) {
                    list.removeAt(0)
                    if (list.size > 0) {
                        binding.settingList.adapter = UserListViewAdapter(requireContext(), list)
                        binding.settingList.setOnItemClickListener { parent, view, position, id ->
                        }
                    } else {
                        dismiss()
                        MToast.mToast(requireContext(), "还没有数据")
                    }
                } else {
                    dismiss()
                    MToast.mToast(requireContext(), "还没有数据")
                }
            }
        }

        return binding.root
    }

    private fun updateListUI(list: MutableList<Any>?) {
        if (list != null) {
            if (list.size > 0) {
                binding.settingList.adapter = HistoryListViewAdapter(
                    requireContext(),
                    list
                )
            } else {
                dismiss()
                MToast.mToast(requireContext(), "还没有数据")
            }
        }
    }


    override fun onDestroy() {
        super.onDestroy()

    }
}
