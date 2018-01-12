package com.yeolabgt.mahmoodms.ssvepinterfacetf;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
/**
 * Created by mmahmood31 on 1/12/2018.
 *
 */

public class TensorflowOptionsMenu extends DialogFragment {
    public int selectedValue = 0;

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setTitle("Select TF Model")
                .setItems(R.array.tf_options, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        // The 'which' argument contains the index position
                        // of the selected item
                        Log.i("TensorflowOptionsMenu", "whichOption: " + String.valueOf(which));
                        selectedValue = which;
                        mListener.onTensorflowOptionsClick(which);
                    }
                });
        return builder.create();
    }
    NoticeDialogListener mListener;

    public interface NoticeDialogListener {
        void onTensorflowOptionsClick(int i);
    }

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        // Verify that the host activity implements the callback interface
        try {
            // Instantiate the NoticeDialogListener so we can send events to the host
            mListener = (NoticeDialogListener) activity;
        } catch (ClassCastException e) {
            // The activity doesn't implement the interface, throw exception
            throw new ClassCastException(activity.toString()
                    + " must implement NoticeDialogListener");
        }
    }
}
