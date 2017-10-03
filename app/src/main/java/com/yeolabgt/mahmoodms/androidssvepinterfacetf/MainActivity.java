package com.yeolabgt.mahmoodms.androidssvepinterfacetf;

import android.Manifest;
import android.app.ActionBar;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by mahmoodms on 6/30/2016.
 * Main Activity Allows for scanning and selection of Bluetooth LE devices for data acquisition,
 * interfacing and classification.
 */

public class MainActivity extends Activity {
    private final static String TAG = DeviceControlActivity.class.getSimpleName();
    private boolean mScanning;
    private Handler mHandler;
    private BluetoothAdapter mBluetoothAdapter;
    private static final int MULTIPLE_PERMISSIONS_REQUEST = 139;
    private ScannedDeviceAdapter scannedDeviceAdapter;
    private ScannedDeviceAdapter selectedDeviceAdapter;
    private static final long SCAN_PERIOD = 10000;
    private final static int REQUEST_ENABLE_BT = 12;
    private List<String> mDeviceAddressesMAC = new ArrayList<>();
    private List<String> mDeviceNames = new ArrayList<>();
    private int mDevicesSelectedCount = 0;

    public final static String INTENT_DEVICES_KEY = "DEVICES_TO_PARSE";
    public final static String INTENT_DEVICES_NAMES = "DEVICE_NAMES_TO_PARSE";
    public final static String INTENT_TRAINING_STIMULUS = "DELAY_VALUE_SECONDS";

    EditText mEditDelayText;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_scan);
        //Set Orientation in Landscape
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        //Set up action bar:
        if (getActionBar() != null) getActionBar().setDisplayHomeAsUpEnabled(false);
        final ActionBar actionBar = getActionBar();
        actionBar.setBackgroundDrawable(new ColorDrawable(Color.parseColor("#800020")));
        //Set orientation of device based on screen type/size:
        //Flag to keep screen on (stay-awake):
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        //Set up timer handler:
        mHandler = new Handler();

        //Initialize scanningDeviceListView Adapter:
        ListView scanningDeviceListView = findViewById(R.id.scanningList);
        ListView selectedDeviceListView = findViewById(R.id.selectedList);
        //Check for BLE Support
        if (!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
            Toast.makeText(this, R.string.ble_not_supported, Toast.LENGTH_SHORT).show();
            finish();
        }
        //Initialize Bluetooth manager
        BluetoothManager bluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        if (bluetoothManager!=null) mBluetoothAdapter = bluetoothManager.getAdapter();
        if (mBluetoothAdapter == null) {
            Toast.makeText(this, R.string.error_bluetooth_not_supported, Toast.LENGTH_SHORT).show();
            finish();
            return;
        }
        final Button connectButton = findViewById(R.id.connectButton);
        //Initialize list view adapter:
        scannedDeviceAdapter = new ScannedDeviceAdapter(this, R.layout.scanning_item, new ArrayList<ScannedDevice>());
        selectedDeviceAdapter = new ScannedDeviceAdapter(this, R.layout.scanning_item, new ArrayList<ScannedDevice>());
        scanningDeviceListView.setAdapter(scannedDeviceAdapter);
        selectedDeviceListView.setAdapter(selectedDeviceAdapter);
        mEditDelayText = findViewById(R.id.editDelayText);
        // Click Item Listener:
        scanningDeviceListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int position, long id) {
                ScannedDevice item = scannedDeviceAdapter.getItem(position);
                if (item != null) {
                    if (!mDeviceAddressesMAC.contains(item.getDeviceMac())) {
                        mDeviceNames.add(item.getDisplayName());
                        mDeviceAddressesMAC.add(item.getDeviceMac());
                        mDevicesSelectedCount++;
                        scannedDeviceAdapter.remove(position);
                        scannedDeviceAdapter.notifyDataSetChanged();
                        selectedDeviceAdapter.add(item);
                        selectedDeviceAdapter.notifyDataSetChanged();
                        Toast.makeText(MainActivity.this, "Device Selected: " + item.getDisplayName() + "\n" + item.getDeviceMac(), Toast.LENGTH_SHORT).show();
                    } else {
                        Toast.makeText(MainActivity.this, "Device Already in List!", Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });
        connectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (mDevicesSelectedCount > 0) {
                    if (mScanning) {
                        if (mBluetoothAdapter.isEnabled())
                            mBluetoothAdapter.getBluetoothLeScanner().stopScan(mScanCallback);
                        mScanning = false;
                    }
                    //Send intent
                    if (mDeviceAddressesMAC != null) {
                        String[] selectedDeviceArray = mDeviceAddressesMAC.toArray(new String[0]);
                        String[] selectedDeviceNames = mDeviceNames.toArray(new String[0]);
                        String[] selectedStimulus = new String[1];
                        selectedStimulus[0] = mEditDelayText.getText().toString();
                        final Intent intent = new Intent(MainActivity.this, DeviceControlActivity.class);
                        intent.putExtra(INTENT_DEVICES_KEY, selectedDeviceArray);
                        intent.putExtra(INTENT_DEVICES_NAMES, selectedDeviceNames);
                        intent.putExtra(INTENT_TRAINING_STIMULUS, selectedStimulus);
                        startActivity(intent);
                    } else {
                        Toast.makeText(MainActivity.this, "No Devices Selected!", Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });
    }

    public boolean checkPermissions() {
        int permissionCheck1 = ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION);
        int permissionCheck2 = ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE);
        int permissionCheck3 = ContextCompat.checkSelfPermission(this, Manifest.permission.SEND_SMS);
        int permissionCheck4 = ContextCompat.checkSelfPermission(this, Manifest.permission.READ_PHONE_STATE);
        List<String> listPermissionsNeeded = new ArrayList<>();
        if (permissionCheck1 != PackageManager.PERMISSION_GRANTED) {
            listPermissionsNeeded.add(Manifest.permission.ACCESS_COARSE_LOCATION);
        }
        if (permissionCheck2 != PackageManager.PERMISSION_GRANTED) {
            listPermissionsNeeded.add(Manifest.permission.WRITE_EXTERNAL_STORAGE);
        }
        if (permissionCheck3 != PackageManager.PERMISSION_GRANTED) {
            listPermissionsNeeded.add(Manifest.permission.SEND_SMS);
        }
        if (permissionCheck4 != PackageManager.PERMISSION_GRANTED) {
            listPermissionsNeeded.add(Manifest.permission.READ_PHONE_STATE);
        }
        Log.e(TAG, "Permissions List Size: " + String.valueOf(listPermissionsNeeded.size()));
        if (!listPermissionsNeeded.isEmpty()) {
            ActivityCompat.requestPermissions(this, listPermissionsNeeded.toArray(new String[listPermissionsNeeded.size()]), MULTIPLE_PERMISSIONS_REQUEST);
            return false;
        }
        return true;
    }

//    @Override
//    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
//        switch (requestCode) {
//            case MY_PERMISSIONS_LOCATIONS_COARSE: {
//                if(grantResults.length>0 && grantResults[0]==PackageManager.PERMISSION_GRANTED) {
//
//                } else {
//                    runOnUiThread(new Runnable() {
//                        @Override
//                        public void run() {
////                            Toast toast = Toast.makeText(getApplicationContext(),
////                                    "This permission is needed for Bluetooth LE!", Toast.LENGTH_LONG);
////                            toast.show();
//                        }
//                    });
//                }
//                return;
//            }
//            case MY_PERMISSIONS_WRITE_STORAGE: {
//                if(grantResults.length>0 && grantResults[0]==PackageManager.PERMISSION_GRANTED) {
//
//                } else {
//                    runOnUiThread(new Runnable() {
//                        @Override
//                        public void run() {
////                            Toast toast = Toast.makeText(getApplicationContext(),
////                                    "This permission is required to save data!", Toast.LENGTH_LONG);
////                            toast.show();
//                        }
//                    });
//                }
//                return;
//            }
//            case MULTIPLE_PERMISSIONS_REQUEST: {
//                if(grantResults.length>0 && grantResults[0]==PackageManager.PERMISSION_GRANTED && grantResults[1]==PackageManager.PERMISSION_GRANTED) {
//                } else {
//                    runOnUiThread(new Runnable() {
//                        @Override
//                        public void run() {
//                            Toast toast = Toast.makeText(getApplicationContext(),
//                                    "These permissions are required!", Toast.LENGTH_LONG);
//                            toast.show();
//                        }
//                    });
//                }
//            }
//        }
//    }

    @Override
    protected void onPause() {
        super.onPause();
        scanLeDevice(false);
        scannedDeviceAdapter.clear();
    }

    @Override
    protected void onResume() {
        super.onResume();
        /*
         * Ensures Bluetooth is enabled on the device - if not enabled - fire intent to display a
         * dialog to ask permissioo enable
         */
        if (checkPermissions()) {
            if (!mBluetoothAdapter.isEnabled()) {
                if (!mBluetoothAdapter.isEnabled()) {
                    Intent enableBt = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                    startActivityForResult(enableBt, REQUEST_ENABLE_BT);
                }
            }
            scanLeDevice(true);
        }
    }

    private ScanCallback mScanCallback = new ScanCallback() {
        @Override
        public void onScanResult(int callbackType, final ScanResult result) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (!mDeviceAddressesMAC.contains(result.getDevice().getAddress())) {
                        scannedDeviceAdapter.update(result.getDevice(), result.getRssi(), result.getScanRecord());
                        scannedDeviceAdapter.notifyDataSetChanged();
                    }
                }
            });
            super.onScanResult(callbackType, result);
        }
    };

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_device_scan, menu);
        if (!mScanning) {
            menu.findItem(R.id.menu_stop).setVisible(false);
            menu.findItem(R.id.menu_scan).setVisible(true);
            menu.findItem(R.id.menu_refresh).setActionView(null);
        } else {
            menu.findItem(R.id.menu_stop).setVisible(true);
            menu.findItem(R.id.menu_scan).setVisible(false);
            menu.findItem(R.id.menu_refresh).setActionView(
                    R.layout.actionbar_progress);
        }
        return true;
    }

    private void scanLeDevice(final boolean enable) {
        if (enable) {
            //stops scanning after ~seconds
            mHandler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    mScanning = false;
                    if (mBluetoothAdapter.isEnabled())
                        mBluetoothAdapter.getBluetoothLeScanner().stopScan(mScanCallback);
                    invalidateOptionsMenu();
                }
            }, SCAN_PERIOD);
            mScanning = true;
            Log.i(TAG, "isNull?1" + String.valueOf(mScanCallback == null));
            Log.i(TAG, "isNull?2" + String.valueOf(mBluetoothAdapter == null));
            if (mBluetoothAdapter.isEnabled())
                mBluetoothAdapter.getBluetoothLeScanner().startScan(mScanCallback);
        } else {
            mScanning = false;
            if (mBluetoothAdapter.isEnabled())
                mBluetoothAdapter.getBluetoothLeScanner().stopScan(mScanCallback);
        }
        invalidateOptionsMenu();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        //if user chose not to enable BT
        if (requestCode == REQUEST_ENABLE_BT && resultCode == Activity.RESULT_CANCELED) {
            finish();
            return;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.menu_scan:
                scannedDeviceAdapter.clear();
                scanLeDevice(true);
                break;
            case R.id.menu_stop:
                scanLeDevice(false);
                break;
        }
        return true;
    }
}
