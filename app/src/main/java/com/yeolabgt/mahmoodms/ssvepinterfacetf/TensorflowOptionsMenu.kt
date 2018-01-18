package com.yeolabgt.mahmoodms.ssvepinterfacetf

import android.app.Activity
import android.app.AlertDialog
import android.app.Dialog
import android.app.DialogFragment
import android.os.Bundle
import android.util.Log

/**
 * Created by mmahmood31 on 1/12/2018.
 *
 */

class TensorflowOptionsMenu : DialogFragment() {
    private var selectedValue = 0
    private var mListener: NoticeDialogListener? = null

    override fun onCreateDialog(savedInstanceState: Bundle): Dialog {
        val builder = AlertDialog.Builder(activity)
        builder.setTitle("Select TF Model")
                .setItems(R.array.tf_options) { _, which ->
                    // The 'which' argument contains the index position
                    // of the selected item
                    Log.i("TensorflowOptionsMenu", "whichOption: " + which.toString())
                    selectedValue = which
                    mListener?.onTensorflowOptionsClick(which)
                }
        return builder.create()
    }

    interface NoticeDialogListener {
        fun onTensorflowOptionsClick(integerValue: Int)
    }

    override fun onAttach(activity: Activity) {
        super.onAttach(activity)
        // Verify that the host activity implements the callback interface
        try {
            // Instantiate the NoticeDialogListener so we can send events to the host
            mListener = activity as NoticeDialogListener
        } catch (e: ClassCastException) {
            // The activity doesn't implement the interface, throw exception
            throw ClassCastException(activity.toString() + " must implement NoticeDialogListener")
        }
    }

//    override fun onAttach(context: Context?) {
//        super.onAttach(context)
//        try {
//            mListener = activity as NoticeDialogListener
//        } catch (e: ClassCastException) {
//            throw ClassCastException(activity.toString() + "Must Implement NoticeDialogListener")
//        }
//    }
}
