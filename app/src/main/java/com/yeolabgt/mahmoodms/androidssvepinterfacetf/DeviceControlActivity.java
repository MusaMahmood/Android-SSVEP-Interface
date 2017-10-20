package com.yeolabgt.mahmoodms.androidssvepinterfacetf;

import android.app.ActionBar;
import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.ColorDrawable;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.support.v4.app.NavUtils;
import android.support.v4.content.FileProvider;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.androidplot.Plot;
import com.androidplot.util.Redrawer;
import com.beele.BluetoothLe;
import com.opencsv.CSVWriter;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.Locale;

/**
 * Created by mahmoodms on 5/31/2016.
 * Android Activity for Controlling Bluetooth LE Device Connectivity
 */

public class DeviceControlActivity extends Activity implements BluetoothLe.BluetoothLeListener {
    public static final String HZ = "0 Hz";
    // Graphing Variables:
    private boolean mGraphInitializedBoolean = false;
    private GraphAdapter mGraphAdapterCh1;
    private GraphAdapter mGraphAdapterCh2;
    private GraphAdapter mGraphAdapterCh1PSDA;
    private GraphAdapter mGraphAdapterCh2PSDA;
    public XYPlotAdapter mTimeDomainPlotAdapter;
    public XYPlotAdapter mFreqDomainPlotAdapter;
    private double[] fPSD;
    private int mSampleRate = 250;
    public static Redrawer redrawer;
    private final static String TAG = DeviceControlActivity.class.getSimpleName();
    //Refactored Data Channel Classes
    DataChannel mCh1;
    DataChannel mCh2;
    boolean mMSBFirst = false;
    //LocalVars
    private String mDeviceName;
    private String mDeviceAddress;
    private boolean mConnected;
    private boolean mRunTrainingBool;
    //Class instance variable
    private boolean mBleInitializedBoolean = false;
    private BluetoothLe mBluetoothLe;
    //Connecting to Multiple Devices
    private String[] deviceMacAddresses = null;
    private BluetoothGatt[] mBluetoothGattArray = null;
    private BluetoothGattService mLedWheelchairControlService = null;
    private int mWheelchairGattIndex;
    private boolean mEEGConnected_2ch = false;
    // Classification
    private int mNumber2ChPackets = -1;
    private static int mPacketBuffer = 6;
    private int mNumberOfClassifierCalls = 0;
    //Layout - TextViews and Buttons
    private TextView mTrainingInstructions;
    private TextView mBatteryLevel;
    private TextView mDataRate;
    private TextView mSSVEPClassTextView;
    private TextView mYfitTextView;
    private Button mExportButton;
    private Switch mDomainSwitch;
    private long mLastTime;
    private int mPSDDataPointsToShow = 0;
    int fPSDStartIndex = 0;
    int fPSDEndIndex = 100;
    private int byteResolution = 3;
    private boolean mFrequencyDomain = true;
    private int points = 0;
    private Menu menu;
    //RSSI:
    private static final int RSSI_UPDATE_TIME_INTERVAL = 2000;
    private Handler mTimerHandler = new Handler();
    private boolean mTimerEnabled = false;
    //Data Variables:
    private int batteryWarning = 20;//
    private double dataRate;
    private double mSSVEPClass = 0;
    private double mStimulusDelaySeconds = 0;
    //Classification
    private boolean mWheelchairControl = false; //Default classifier.
    //Play Sound:
    MediaPlayer mMediaBeep;
    //File Save Stuff:
    private boolean fileSaveInitialized = false;
    private CSVWriter csvWriter;
    private File file;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_control);
        //Set orientation of device based on screen type/size:
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        //Receive Intents:
        final Intent intent = getIntent();
        deviceMacAddresses = intent.getStringArrayExtra(MainActivity.INTENT_DEVICES_KEY);
        String[] deviceDisplayNames = intent.getStringArrayExtra(MainActivity.INTENT_DEVICES_NAMES);
        String[] intentStimulusClass = intent.getStringArrayExtra(MainActivity.INTENT_DELAY_VALUE_SECONDS);
        if (intent.getExtras() != null)
            mRunTrainingBool = intent.getExtras().getBoolean(MainActivity.INTENT_TRAIN_BOOLEAN);
        else Log.e(TAG, "ERROR: intent.getExtras = null");

        mStimulusDelaySeconds = Integer.valueOf(intentStimulusClass[0]);
        mDeviceName = deviceDisplayNames[0];
        mDeviceAddress = deviceMacAddresses[0];
        Log.d(TAG, "Device Names: " + Arrays.toString(deviceDisplayNames));
        Log.d(TAG, "Device MAC Addresses: " + Arrays.toString(deviceMacAddresses));
        Log.d(TAG, Arrays.toString(deviceMacAddresses));
        //Set up action bar:
        if (getActionBar() != null) {
            getActionBar().setDisplayHomeAsUpEnabled(true);
        }
        ActionBar actionBar = getActionBar();
        actionBar.setBackgroundDrawable(new ColorDrawable(Color.parseColor("#6078ef")));
        //Flag to keep screen on (stay-awake):
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        //Set up TextViews
        mExportButton = findViewById(R.id.button_export);
        mDomainSwitch = findViewById(R.id.domainSwitch);
        mBatteryLevel = findViewById(R.id.batteryText);
        mTrainingInstructions = findViewById(R.id.trainingInstructions);
        updateTrainingView(mRunTrainingBool);
        mDataRate = findViewById(R.id.dataRate);
        mDataRate.setText("...");
        mYfitTextView = findViewById(R.id.textViewYfit);
        ActionBar ab = getActionBar();
        ab.setTitle(mDeviceName);
        ab.setSubtitle(mDeviceAddress);
        //Initialize Bluetooth
        if(!mBleInitializedBoolean) initializeBluetoothArray();
        mExportButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    terminateDataFileWriter();
                } catch (IOException e) {
                    Log.e(TAG, "IOException in saveDataFile");
                    e.printStackTrace();
                }
                Context context = getApplicationContext();
                Uri uii = FileProvider.getUriForFile(context, context.getPackageName() + ".provider", file);
                Intent exportData = new Intent(Intent.ACTION_SEND);
                exportData.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
                exportData.putExtra(Intent.EXTRA_SUBJECT, "Sensor Data Export Details");
                exportData.putExtra(Intent.EXTRA_STREAM, uii);
                exportData.setType("text/html");
                startActivity(exportData);
            }
        });
        makeFilterSwitchVisible(false);
        mDomainSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                mFrequencyDomain = isChecked;
            }
        });
        mLastTime = System.currentTimeMillis();
        mSSVEPClassTextView = findViewById(R.id.eegClassTextView);
        ToggleButton toggleButton1 = findViewById(R.id.toggleButtonWheelchairControl);
        toggleButton1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                mWheelchairControl = b;
            }
        });

        ToggleButton ch1 = findViewById(R.id.toggleButtonCh1);
        ch1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                mGraphAdapterCh1.setPlotData(b);
                mGraphAdapterCh2.setPlotData(!b);
            }
        });
        ToggleButton ch2 = findViewById(R.id.toggleButtonCh2);
        ch2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                mGraphAdapterCh1PSDA.setPlotData(b);
                mGraphAdapterCh2PSDA.setPlotData(b);
            }
        });
        Button resetButton = findViewById(R.id.resetActivityButton);
        resetButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                resetActivity();
            }
        });
        mMediaBeep = MediaPlayer.create(this, R.raw.beep_01a);
    }

    private void resetActivity() {
        recreate();
    }

    public String getTimeStamp() {
        return new SimpleDateFormat("yyyy.MM.dd_HH.mm.ss", Locale.US).format(new Date());
    }

    /**
     * @throws IOException some IO Error
     */
    public void terminateDataFileWriter() throws IOException {
        if (fileSaveInitialized) {
            csvWriter.flush();
            csvWriter.close();
            fileSaveInitialized = false;
        }
    }

    /**
     * Initializes CSVWriter For Saving Data.
     *
     * @throws IOException bc
     */
    public void saveDataFile() throws IOException {
        File root = Environment.getExternalStorageDirectory();
//        String fileTimeStamp = "EEG_SSVEPData_" + getTimeStamp() + "_" + String.valueOf((int) mSSVEPClass);
        String fileTimeStamp = "EEG_SSVEPData_" + getTimeStamp() + "_" + String.valueOf(mSampleRate) + "Hz";
        Log.e(TAG, "fileTimeStamp: " + fileTimeStamp);
        if (root.canWrite()) {
            File dir = new File(root.getAbsolutePath() + "/EEGData");
            boolean resultMkdir = dir.mkdirs();
            if (!resultMkdir) {
                Log.e(TAG, "MKDIRS FAILED");
            }
            file = new File(dir, fileTimeStamp + ".csv");
            if (file.exists() && !file.isDirectory()) {
                Log.d(TAG, "File " + file.toString() + " already exists - appending data");
                FileWriter fileWriter = new FileWriter(file, true);
                csvWriter = new CSVWriter(fileWriter);
            } else {
                csvWriter = new CSVWriter(new FileWriter(file));
            }
            fileSaveInitialized = true;
        }
    }

    public void exportFileWithClass(double eegData1, double eegData2) throws IOException {
        if (fileSaveInitialized) {
            String[] writeCSVValue = new String[3];
            writeCSVValue[0] = eegData1 + "";
            writeCSVValue[1] = eegData2 + "";
            writeCSVValue[2] = mSSVEPClass + "";
            csvWriter.writeNext(writeCSVValue, false);
        }
    }

    @Override
    public void onResume() {
        makeFilterSwitchVisible(true);
        jmainInitialization(false);
        double a[] = new double[1000];
        Arrays.fill(a, 0.0);
        double b[] = new double[1000];
        Arrays.fill(b, 0.0);
        jClassifySSVEP(a, b, 2.28300);
        double aa[] = new double[8000];
        Arrays.fill(aa, 0.0);
        double bb[] = new double[8000];
        Arrays.fill(bb, 0.0);
        jClassifySSVEP4k(aa, bb, 2.278);
        if (redrawer != null) {
            redrawer.start();
        }
        super.onResume();
    }

    @Override
    protected void onPause() {
        if (redrawer != null) redrawer.pause();
        makeFilterSwitchVisible(false);
        super.onPause();
    }

    private void initializeBluetoothArray() {
        BluetoothManager mBluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        BluetoothDevice[] mBluetoothDeviceArray = new BluetoothDevice[deviceMacAddresses.length];
        mBluetoothGattArray = new BluetoothGatt[deviceMacAddresses.length];
        Log.d(TAG, "Device Addresses: " + Arrays.toString(deviceMacAddresses));
        if (deviceMacAddresses != null && mBluetoothManager != null) {
            for (int i = 0; i < deviceMacAddresses.length; i++) {
                mBluetoothDeviceArray[i] = mBluetoothManager.getAdapter().getRemoteDevice(deviceMacAddresses[i]);
            }
        } else {
            Log.e(TAG, "No Devices Queued, Restart!");
            Toast.makeText(this, "No Devices Queued, Restart!", Toast.LENGTH_SHORT).show();
        }
        mBluetoothLe = new BluetoothLe(this, mBluetoothManager, this);
        for (int i = 0; i < mBluetoothDeviceArray.length; i++) {
            mBluetoothGattArray[i] = mBluetoothLe.connect(mBluetoothDeviceArray[i], false);
            Log.e(TAG, "Connecting to Device: " + String.valueOf(mBluetoothDeviceArray[i].getName() + " " + mBluetoothDeviceArray[i].getAddress()));
            if ("WheelchairControl".equals(mBluetoothDeviceArray[i].getName())) {
                mWheelchairGattIndex = i;
                Log.e(TAG, "mWheelchairGattIndex: " + mWheelchairGattIndex);
            }
            if ("EMG 250Hz".equals(mBluetoothDeviceArray[i].getName())) {
                mMSBFirst = false;
            } else if ("EMG 3CH 250Hz".equals(mBluetoothDeviceArray[i].getName())) {
                mMSBFirst = true;
            } else if (mBluetoothDeviceArray[i].getName().toLowerCase().contains("nRF52".toLowerCase())) {
                mMSBFirst = true;
            }
            if (mBluetoothDeviceArray[i].getName().toLowerCase().contains("8k".toLowerCase())) {
                mSampleRate = 8000;
                mPacketBuffer = 32;
                byteResolution = 2; //FOR ECG ONLY
            } else if (mBluetoothDeviceArray[i].getName().toLowerCase().contains("4k".toLowerCase())) {
                mSampleRate = 4000;
                mPacketBuffer = 16;
                byteResolution = 3;
            } else if (mBluetoothDeviceArray[i].getName().toLowerCase().contains("2k".toLowerCase())) {
                mSampleRate = 2000;
                mPacketBuffer = 8;
                byteResolution = 3;
            } else if (mBluetoothDeviceArray[i].getName().toLowerCase().contains("1k".toLowerCase())) {
                mSampleRate = 1000;
                mPacketBuffer = 4;
                byteResolution = 3;
            } else if (mBluetoothDeviceArray[i].getName().toLowerCase().contains("500".toLowerCase())) {
                mSampleRate = 500;
                mPacketBuffer = 2;
                byteResolution = 3;
            } else {
                mSampleRate = 250;
                mPacketBuffer = 2;
            }
            fPSDStartIndex = 16;
            fPSDEndIndex = 80;
            Log.e(TAG, "mSampleRate: " + mSampleRate + "Hz");
            fPSD = jLoadfPSD(mSampleRate);

            if(!mGraphInitializedBoolean) setupGraph();

            mGraphAdapterCh1.setxAxisIncrementFromSampleRate(mSampleRate);
            mGraphAdapterCh2.setxAxisIncrementFromSampleRate(mSampleRate);

            mGraphAdapterCh1.setSeriesHistoryDataPoints(250 * 5);
            mGraphAdapterCh2.setSeriesHistoryDataPoints(250 * 5);

            if (!fileSaveInitialized) {
                try {
                    saveDataFile();
                } catch (IOException ex) {
                    Log.e("IOEXCEPTION:", ex.toString());
                }
            }
        }
        mBleInitializedBoolean = true;
    }

    private void setupGraph() {
        // Initialize our XYPlot reference:
        mGraphAdapterCh1 = new GraphAdapter(mSampleRate * 4, "EEG Data Ch 1", false, Color.BLUE, mSampleRate * 4); //Color.parseColor("#19B52C") also, RED, BLUE, etc.
        mGraphAdapterCh2 = new GraphAdapter(mSampleRate * 4, "EEG Data Ch 2", false, Color.RED, mSampleRate * 4); //Color.parseColor("#19B52C") also, RED, BLUE, etc.
        mGraphAdapterCh1PSDA = new GraphAdapter(mPSDDataPointsToShow, "EEG Power Spectrum (Ch1)", false, Color.BLUE, 0);
        mGraphAdapterCh2PSDA = new GraphAdapter(mPSDDataPointsToShow, "EEG Power Spectrum (Ch2)", false, Color.RED, 0);
        //PLOT CH1 By default
        mGraphAdapterCh1.plotData = true;
        mGraphAdapterCh1PSDA.plotData = true;
        mGraphAdapterCh2PSDA.plotData = true;
        mGraphAdapterCh1.setPointWidth((float) 2);
        mGraphAdapterCh2.setPointWidth((float) 2);
        mGraphAdapterCh1PSDA.setPointWidth((float) 2);
        mGraphAdapterCh2PSDA.setPointWidth((float) 2);
        mTimeDomainPlotAdapter = new XYPlotAdapter(findViewById(R.id.eegTimeDomainXYPlot), false, 1000);
        mTimeDomainPlotAdapter.xyPlot.addSeries(mGraphAdapterCh1.series, mGraphAdapterCh1.lineAndPointFormatter);
        mTimeDomainPlotAdapter.xyPlot.addSeries(mGraphAdapterCh2.series, mGraphAdapterCh2.lineAndPointFormatter);
        mFreqDomainPlotAdapter = new XYPlotAdapter(findViewById(R.id.frequencyAnalysisXYPlot), "Frequency (Hz)", "Power Density (W/Hz)", ((double) mSampleRate / 125.0));
        mFreqDomainPlotAdapter.xyPlot.addSeries(mGraphAdapterCh1PSDA.series, mGraphAdapterCh1PSDA.lineAndPointFormatter);
        mFreqDomainPlotAdapter.xyPlot.addSeries(mGraphAdapterCh2PSDA.series, mGraphAdapterCh2PSDA.lineAndPointFormatter);

        redrawer = new Redrawer(
                Arrays.asList(new Plot[]{mTimeDomainPlotAdapter.xyPlot, mFreqDomainPlotAdapter.xyPlot}), 60, false);
        redrawer.start();
        mGraphInitializedBoolean = true;
    }

    private void setNameAddress(String name_action, String address_action) {
        MenuItem name = menu.findItem(R.id.action_title);
        MenuItem address = menu.findItem(R.id.action_address);
        name.setTitle(name_action);
        address.setTitle(address_action);
        invalidateOptionsMenu();
    }

    @Override
    protected void onDestroy() {
        redrawer.finish();
        disconnectAllBLE();
        try {
            terminateDataFileWriter();
        } catch (IOException e) {
            Log.e(TAG, "IOException in saveDataFile");
            e.printStackTrace();
        }
        stopMonitoringRssiValue();
        jmainInitialization(true); //Just a technicality; doesn't actually do anything
        super.onDestroy();
    }

    private void disconnectAllBLE() {
        if (mBluetoothLe != null) {
            for (BluetoothGatt bluetoothGatt : mBluetoothGattArray) {
                mBluetoothLe.disconnect(bluetoothGatt);
                mConnected = false;
                resetMenuBar();
            }
        }
    }

    private void resetMenuBar() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (menu != null) {
                    menu.findItem(R.id.menu_connect).setVisible(true);
                    menu.findItem(R.id.menu_disconnect).setVisible(false);
                }
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_device_control, menu);
        getMenuInflater().inflate(R.menu.actionbar_item, menu);
        if (mConnected) {
            menu.findItem(R.id.menu_connect).setVisible(false);
            menu.findItem(R.id.menu_disconnect).setVisible(true);
        } else {
            menu.findItem(R.id.menu_connect).setVisible(true);
            menu.findItem(R.id.menu_disconnect).setVisible(false);
        }
        this.menu = menu;
        setNameAddress(mDeviceName, mDeviceAddress);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.menu_connect:
                if (mBluetoothLe != null) {
                    initializeBluetoothArray();
                }
                connect();
                return true;
            case R.id.menu_disconnect:
                if (mBluetoothLe != null) {
                    disconnectAllBLE();
                }
                return true;
            case android.R.id.home:
                if (mBluetoothLe != null) {
                    disconnectAllBLE();
                }
                NavUtils.navigateUpFromSameTask(this);
                onBackPressed();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private void connect() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                MenuItem menuItem = menu.findItem(R.id.action_status);
                menuItem.setTitle("Connecting...");
            }
        });
    }

    @Override
    public void onServicesDiscovered(BluetoothGatt gatt, int status) {
        Log.i(TAG, "onServicesDiscovered");
        if (status == BluetoothGatt.GATT_SUCCESS) {
            for (BluetoothGattService service : gatt.getServices()) {
                if ((service == null) || (service.getUuid() == null)) {
                    continue;
                }
                if (AppConstant.SERVICE_DEVICE_INFO.equals(service.getUuid())) {
                    //Read the device serial number
                    mBluetoothLe.readCharacteristic(gatt, service.getCharacteristic(AppConstant.CHAR_SERIAL_NUMBER));
                    //Read the device software version
                    mBluetoothLe.readCharacteristic(gatt, service.getCharacteristic(AppConstant.CHAR_SOFTWARE_REV));
                }
                if (AppConstant.SERVICE_WHEELCHAIR_CONTROL.equals(service.getUuid())) {
                    mLedWheelchairControlService = service;
                    Log.i(TAG, "BLE Wheelchair Control Service found");
                }

                if (AppConstant.SERVICE_3CH_EMG_SIGNAL.equals(service.getUuid())) {
                    makeFilterSwitchVisible(true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_3CH_EMG_SIGNAL_CH1), true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_3CH_EMG_SIGNAL_CH2), true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_3CH_EMG_SIGNAL_CH3), true);
                }

                if (AppConstant.SERVICE_EEG_SIGNAL.equals(service.getUuid())) {
                    makeFilterSwitchVisible(true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EEG_CH1_SIGNAL), true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EEG_CH2_SIGNAL), true);
                    if (service.getCharacteristic(AppConstant.CHAR_EEG_CH3_SIGNAL) != null) {
                        mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EEG_CH3_SIGNAL), true);
                    }
                    if (service.getCharacteristic(AppConstant.CHAR_EEG_CH4_SIGNAL) != null) {
                        mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EEG_CH4_SIGNAL), true);
                    }
                }

                if (AppConstant.SERVICE_EOG_SIGNAL.equals(service.getUuid())) {
                    makeFilterSwitchVisible(true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EOG_CH1_SIGNAL), true);
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EOG_CH2_SIGNAL), true);
                    for (BluetoothGattCharacteristic c : service.getCharacteristics()) {
                        if (AppConstant.CHAR_EOG_CH3_SIGNAL.equals(c.getUuid())) {
                            mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_EOG_CH3_SIGNAL), true);
                        }
                    }
                }

                if (AppConstant.SERVICE_BATTERY_LEVEL.equals(service.getUuid())) { //Read the device battery percentage
                    mBluetoothLe.readCharacteristic(gatt, service.getCharacteristic(AppConstant.CHAR_BATTERY_LEVEL));
                    mBluetoothLe.setCharacteristicNotification(gatt, service.getCharacteristic(AppConstant.CHAR_BATTERY_LEVEL), true);
                }
            }
        }
    }

    private void makeFilterSwitchVisible(final boolean visible) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (visible) {
                    mDomainSwitch.setVisibility(View.VISIBLE);
                    mExportButton.setVisibility(View.VISIBLE);
                } else {
                    mExportButton.setVisibility(View.INVISIBLE);
                    mDomainSwitch.setVisibility(View.INVISIBLE);
                }
            }
        });
    }

    @Override
    public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
        Log.i(TAG, "onCharacteristicRead");
        if (status == BluetoothGatt.GATT_SUCCESS) {
            if (AppConstant.CHAR_BATTERY_LEVEL.equals(characteristic.getUuid())) {
                int batteryLevel = characteristic.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT16, 0);
                updateBatteryStatus(batteryLevel);
                Log.i(TAG, "Battery Level :: " + batteryLevel);
            }
        } else {
            Log.e(TAG, "onCharacteristic Read Error" + status);
        }
    }

    @Override
    public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
        if ((mCh1 == null || mCh2 == null)) {
            mCh1 = new DataChannel(false, mMSBFirst);
            mCh2 = new DataChannel(false, mMSBFirst);
        }

        if (AppConstant.CHAR_BATTERY_LEVEL.equals(characteristic.getUuid())) {
            int batteryLevel = characteristic.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT16, 0);
            updateBatteryStatus(batteryLevel);
        }

        if (AppConstant.CHAR_EEG_CH1_SIGNAL.equals(characteristic.getUuid())) {
            byte[] mNewEEGdataBytes = characteristic.getValue();
            if (!mCh1.chEnabled) {
                mCh1.chEnabled = true;
            }
            getDataRateBytes(mNewEEGdataBytes.length);
            if (mEEGConnected_2ch) {
                mCh1.handleNewData(mNewEEGdataBytes);
                if (mCh1.packetCounter == mPacketBuffer) {
                    addToGraphBuffer(mCh1, mGraphAdapterCh1, true);
                }
            }
        }

        if (AppConstant.CHAR_EEG_CH2_SIGNAL.equals(characteristic.getUuid())) {
            if (!mCh2.chEnabled) {
                mCh2.chEnabled = true;
            }
            byte[] mNewEEGdataBytes = characteristic.getValue();
            int byteLength = mNewEEGdataBytes.length;
            getDataRateBytes(byteLength);
            if (mEEGConnected_2ch) {
                mCh2.handleNewData(mNewEEGdataBytes);
                if (mCh2.packetCounter == mPacketBuffer) {
                    addToGraphBuffer(mCh2, mGraphAdapterCh2, false);
                }
            }
        }
        if (mCh1.chEnabled && mCh2.chEnabled) {
            mNumber2ChPackets++;
            mEEGConnected_2ch = true;
            mCh1.chEnabled = false;
            mCh2.chEnabled = false;
            if (mCh1.characteristicDataPacketBytes != null && mCh2.characteristicDataPacketBytes != null) {
                writeToDisk24(mCh1.characteristicDataPacketBytes, mCh2.characteristicDataPacketBytes);
            }
            if (mNumber2ChPackets % 10 == 0) { //Every x * 6 data points
                Thread classifyTaskThread = new Thread(mClassifyTaskRunnableThread);
                classifyTaskThread.start();
                Log.e(TAG, "[" + String.valueOf(mNumberOfClassifierCalls + 1) + "] CALLING CLASSIFIER FUNCTION!");
            }
        }

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                String concat = "C:[" + mSSVEPClass + "]";
                mSSVEPClassTextView.setText(concat);
            }
        });
    }

    void addToGraphBuffer(DataChannel dataChannel, GraphAdapter graphAdapter, boolean updateTrainingRoutine) {
        if (byteResolution == 3) {
            for (int i = 0; i < dataChannel.dataBuffer.length / 3; i += graphAdapter.sampleRate / 250) {
                graphAdapter.addDataPoint(DataChannel.bytesToDouble(dataChannel.dataBuffer[3 * i],
                        dataChannel.dataBuffer[3 * i + 1], dataChannel.dataBuffer[3 * i + 2]),
                        dataChannel.totalDataPointsReceived - dataChannel.dataBuffer.length / 3 + i);
                if (updateTrainingRoutine) {
                    for (int j = 0; j < graphAdapter.sampleRate / 250; j++) {
                        updateTrainingRoutine(dataChannel.totalDataPointsReceived - dataChannel.dataBuffer.length / 3 + i + j);
                    }
                }
            }
        } else if (byteResolution == 2) {
            for (int i = 0; i < dataChannel.dataBuffer.length / 2; i += graphAdapter.sampleRate / 250) {
                graphAdapter.addDataPoint(DataChannel.bytesToDouble(dataChannel.dataBuffer[2 * i],
                        dataChannel.dataBuffer[2 * i + 1]),
                        dataChannel.totalDataPointsReceived - dataChannel.dataBuffer.length / 2 + i);
                if (updateTrainingRoutine) {
                    for (int j = 0; j < graphAdapter.sampleRate / 250; j++) {
                        updateTrainingRoutine(dataChannel.totalDataPointsReceived - dataChannel.dataBuffer.length / 2 + i + j);
                    }
                }
            }
        }
        dataChannel.dataBuffer = null;
        dataChannel.packetCounter = 0;
    }

    private Runnable mClassifyTaskRunnableThread = new Runnable() {
        @Override
        public void run() {
            if (mFrequencyDomain) {
                double[] PSD2ch;
                double[] PSDCh1 = new double[mSampleRate];
                double[] PSDCh2 = new double[mSampleRate];
                double[] getInstancePSD1 = new double[mSampleRate * 2];
                double[] getInstancePSD2 = new double[mSampleRate * 2];
                System.arraycopy(mGraphAdapterCh1.classificationBuffer, mSampleRate * 2, getInstancePSD1, 0, mSampleRate * 2);
                System.arraycopy(mGraphAdapterCh2.classificationBuffer, mSampleRate * 2, getInstancePSD2, 0, mSampleRate * 2);
                if(mSampleRate <8000) {
                    PSD2ch = jPSDExtraction(getInstancePSD1, getInstancePSD2, mSampleRate); //250 Hz: For PSDA/each channel[0>mSampleRate|mSampleRate:end]
                    System.arraycopy(PSD2ch, 0, PSDCh1, 0, mSampleRate);
                    System.arraycopy(PSD2ch, mSampleRate, PSDCh2, 0, mSampleRate);
                } else {
                    Arrays.fill(PSDCh1,0.0);
                    Arrays.fill(PSDCh2,0.0);
                }

                if (mPSDDataPointsToShow == 0) {
                    mPSDDataPointsToShow = fPSDEndIndex - fPSDStartIndex;
                    mGraphAdapterCh1PSDA.setSeriesHistoryDataPoints(mPSDDataPointsToShow);
                    mGraphAdapterCh2PSDA.setSeriesHistoryDataPoints(mPSDDataPointsToShow);
                    if (mPSDDataPointsToShow > 64)
                        mFreqDomainPlotAdapter.setXyPlotDomainIncrement(6.0);
                    else mFreqDomainPlotAdapter.setXyPlotDomainIncrement(2.0);
                }
                mGraphAdapterCh1PSDA.addDataPointsGeneric(fPSD, PSDCh1, fPSDStartIndex, fPSDEndIndex);
                mGraphAdapterCh2PSDA.addDataPointsGeneric(fPSD, PSDCh2, fPSDStartIndex, fPSDEndIndex);
            }

            double Y[];
            if (mSampleRate == 250) {
                double[] getInstance1 = mGraphAdapterCh1.classificationBuffer;
                double[] getInstance2 = mGraphAdapterCh2.classificationBuffer;
                Y = jClassifySSVEP(getInstance1, getInstance2, 1.5); // Size of 501, where first two are
            } else if (mSampleRate == 4000) {
                //require last 8k pts:
                double[] getInstance1 = new double[mSampleRate * 2];
                double[] getInstance2 = new double[mSampleRate * 2];
                System.arraycopy(mGraphAdapterCh1.classificationBuffer, mSampleRate * 2, getInstance1, 0, mSampleRate * 2); //8000→end
                System.arraycopy(mGraphAdapterCh2.classificationBuffer, mSampleRate * 2, getInstance2, 0, mSampleRate * 2);
                Y = jClassifySSVEP4k(getInstance1, getInstance2, 1.5);
            } else {
                Y = new double[]{0.0, 0.0};
            }
            mNumberOfClassifierCalls++;
            Log.e(TAG, "Classifier Output: [#" + String.valueOf(mNumberOfClassifierCalls) + "::" + String.valueOf(Y[0]) + "," + String.valueOf(Y[1]) + "]");
            final String s = "SSVEP cPSDA\n: [" + String.valueOf(Y[1]) + "]";
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mYfitTextView.setText(s);
                }
            });
            executeWheelchairCommand((int) (double) Y[1]);
        }
    };

    private void updateTrainingRoutine(int dataPoints) {
        if (dataPoints % mSampleRate == 0 && mRunTrainingBool) {
            int second = dataPoints / mSampleRate;
            int mSDS = (int) mStimulusDelaySeconds;
            int eventSecondCountdown = 0;
            if (second >= 0 && second < mSDS) {
                eventSecondCountdown = mSDS - second;
                updateTrainingPrompt("EYES CLOSED");
                updateTrainingPromptColor(Color.GREEN);
                mSSVEPClass = 0;
            } else if (second >= mSDS && second < 2 * mSDS) {
                eventSecondCountdown = 2 * mSDS - second;
                updateTrainingPrompt("15.15Hz");
                mSSVEPClass = 1;
            } else if (second >= 2 * mSDS && second < 3 * mSDS) {
                eventSecondCountdown = 3 * mSDS - second;
                updateTrainingPrompt("16.67hz");
                mSSVEPClass = 2;
            } else if (second >= 3 * mSDS && second < 4 * mSDS) {
                eventSecondCountdown = 4 * mSDS - second;
                updateTrainingPrompt("18.51Hz");
                mSSVEPClass = 3;
            } else if (second >= 4 * mSDS && second < 5 * mSDS) {
                eventSecondCountdown = 5 * mSDS - second;
                updateTrainingPrompt("20.00Hz");
                mSSVEPClass = 4;
            } else if (second >= 5 * mSDS && second < 6 * mSDS) {
                eventSecondCountdown = 6 * mSDS - second;
                updateTrainingPrompt("Stop!");
                updateTrainingPromptColor(Color.RED);
                mSSVEPClass = 0;
                disconnectAllBLE();
            }
            if (eventSecondCountdown == 10) {
                mMediaBeep.start();
            }
        }
    }

    private void updateTrainingPrompt(final String prompt) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mRunTrainingBool) {
                    mTrainingInstructions.setText(prompt);
                }
            }
        });
    }

    private void updateTrainingView(final boolean b) {
        final int visibility = (b) ? View.VISIBLE : View.GONE;
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mTrainingInstructions.setVisibility(visibility);
            }
        });
    }

    private void updateTrainingPromptColor(final int color) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mRunTrainingBool) {
                    mTrainingInstructions.setTextColor(color);
                }
            }
        });
    }

    private void executeWheelchairCommand(int command) {
        byte[] bytes = new byte[1];
        switch (command) {
            case 0:
                bytes[0] = (byte) 0x00;
                break;
            case 1:
                bytes[0] = (byte) 0x01; //Stop
                break;
            case 2:
                bytes[0] = (byte) 0xF0; //?
                break;
            case 3:
                bytes[0] = (byte) 0x0F;
                break;
            case 4:
                // TODO: 6/27/2017 Disconnect instead of reverse?
                bytes[0] = (byte) 0xFF;
                break;
            default:
                break;
        }
        if (mLedWheelchairControlService != null && mWheelchairControl) {
            mBluetoothLe.writeCharacteristic(mBluetoothGattArray[mWheelchairGattIndex], mLedWheelchairControlService.getCharacteristic(AppConstant.CHAR_WHEELCHAIR_CONTROL), bytes);
        }
    }

    private void writeToDisk24(byte[] ch1Bytes, byte[] ch2Bytes) {
        if(byteResolution==3) {
            for (int i = 0; i < ch1Bytes.length / 3; i++) {
                try {
                    exportFileWithClass(DataChannel.bytesToDouble(ch1Bytes[3 * i], ch1Bytes[3 * i + 1], ch1Bytes[3 * i + 2]),
                            DataChannel.bytesToDouble(ch2Bytes[3 * i], ch2Bytes[3 * i + 1], ch2Bytes[3 * i + 2]));
                } catch (IOException e) {
                    Log.e("IOException", e.toString());
                }
            }
        } else if (byteResolution==2) {
            for (int i = 0; i < ch1Bytes.length / 2; i++) {
                try {
                    exportFileWithClass(DataChannel.bytesToDouble(ch1Bytes[2 * i], ch1Bytes[2 * i + 1]),
                            DataChannel.bytesToDouble(ch2Bytes[2 * i], ch2Bytes[2 * i + 1]));
                } catch (IOException e) {
                    Log.e("IOException", e.toString());
                }
            }
        }
    }

    private void getDataRateBytes(int bytes) {
        long mCurrentTime = System.currentTimeMillis();
        points += bytes;
        if (mCurrentTime > (mLastTime + 5000)) {
            dataRate = (points / 5);
            points = 0;
            mLastTime = mCurrentTime;
            Log.e(" DataRate:", String.valueOf(dataRate) + " Bytes/s");
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    String s = String.valueOf(dataRate) + " Bytes/s";
                    mDataRate.setText(s);
                }
            });
        }
    }

    @Override
    public void onReadRemoteRssi(BluetoothGatt gatt, int rssi, int status) {
        uiRssiUpdate(rssi);
    }

    @Override
    public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
        switch (newState) {
            case BluetoothProfile.STATE_CONNECTED:
                mConnected = true;
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        if (menu != null) {
                            menu.findItem(R.id.menu_connect).setVisible(false);
                            menu.findItem(R.id.menu_disconnect).setVisible(true);
                        }
                    }
                });
                Log.i(TAG, "Connected");
                updateConnectionState(getString(R.string.connected));
                invalidateOptionsMenu();
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mDataRate.setTextColor(Color.BLACK);
                        mDataRate.setTypeface(null, Typeface.NORMAL);
                    }
                });
                //Start the service discovery:
                gatt.discoverServices();
                startMonitoringRssiValue();
                break;
            case BluetoothProfile.STATE_DISCONNECTED:
                mConnected = false;
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        if (menu != null) {
                            menu.findItem(R.id.menu_connect).setVisible(true);
                            menu.findItem(R.id.menu_disconnect).setVisible(false);
                        }
                    }
                });
                Log.i(TAG, "Disconnected");
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mDataRate.setTextColor(Color.RED);
                        mDataRate.setTypeface(null, Typeface.BOLD);
                        mDataRate.setText(HZ);
                    }
                });
                updateConnectionState(getString(R.string.disconnected));
                stopMonitoringRssiValue();
                invalidateOptionsMenu();
                break;
            default:
                break;
        }
    }

    public void startMonitoringRssiValue() {
        readPeriodicallyRssiValue(true);
    }

    public void stopMonitoringRssiValue() {
        readPeriodicallyRssiValue(false);
    }

    public void readPeriodicallyRssiValue(final boolean repeat) {
        mTimerEnabled = repeat;
        // check if we should stop checking RSSI value
        if (!mConnected || mBluetoothGattArray == null || !mTimerEnabled) {
            mTimerEnabled = false;
            return;
        }

        mTimerHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (mBluetoothGattArray == null || !mConnected) {
                    mTimerEnabled = false;
                    return;
                }
                // request RSSI value
                mBluetoothGattArray[0].readRemoteRssi();
                // add call it once more in the future
                readPeriodicallyRssiValue(mTimerEnabled);
            }
        }, RSSI_UPDATE_TIME_INTERVAL);
    }

    @Override
    public void onCharacteristicWrite(BluetoothGatt gatt, BluetoothGattCharacteristic
            characteristic, int status) {
        Log.i(TAG, "onCharacteristicWrite :: Status:: " + status);
    }

    @Override
    public void onDescriptorWrite(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
    }

    @Override
    public void onDescriptorRead(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
        Log.i(TAG, "onDescriptorRead :: Status:: " + status);
    }

    @Override
    public void onError(String errorMessage) {
        Log.e(TAG, "Error:: " + errorMessage);
    }

    private void updateConnectionState(final String status) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (status.equals(getString(R.string.connected))) {
                    Toast.makeText(getApplicationContext(), "Device Connected!", Toast.LENGTH_SHORT).show();
                } else if (status.equals(getString(R.string.disconnected))) {
                    Toast.makeText(getApplicationContext(), "Device Disconnected!", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    private void updateBatteryStatus(final int integerValue) {
        final String status;
        double convertedBatteryVoltage = ((double) integerValue/(4096.0))*7.20;
        //Because TPS63001 dies below 1.8V, we need to set up a linear fit between 1.8-4.2V
        //Anything over 4.2V = 100%
        final double finalPercent;
        if (((125.0/3.0)*convertedBatteryVoltage - 75.0) > 100.0) {
            finalPercent = 100.0;
        } else if (((125.0/3.0)*convertedBatteryVoltage - 75.0) < 0) {
            finalPercent = 0;
        } else {
            finalPercent = (125.0/3.0)*convertedBatteryVoltage - 75.0;
        }
        Log.e(TAG,"Battery Integer Value: "+String.valueOf(integerValue));
        Log.e(TAG,"ConvertedBatteryVoltage: "+String.format(Locale.US,"%.5f",convertedBatteryVoltage)+"V : "+String.format(Locale.US,"%.3f",finalPercent)+"%");
        status = String.format(Locale.US,"%.1f",finalPercent)+"%";
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (finalPercent <= batteryWarning) {
                    mBatteryLevel.setTextColor(Color.RED);
                    mBatteryLevel.setTypeface(null, Typeface.BOLD);
                    Toast.makeText(getApplicationContext(), "Charge Battery, Battery Low " + status, Toast.LENGTH_SHORT).show();
                } else {
                    mBatteryLevel.setTextColor(Color.GREEN);
                    mBatteryLevel.setTypeface(null, Typeface.BOLD);
                }
                mBatteryLevel.setText(status);
            }
        });
    }

    private void uiRssiUpdate(final int rssi) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                MenuItem menuItem = menu.findItem(R.id.action_rssi);
                MenuItem status_action_item = menu.findItem(R.id.action_status);
                final String valueOfRSSI = String.valueOf(rssi) + " dB";
                menuItem.setTitle(valueOfRSSI);
                if (mConnected) {
                    String newStatus = "Status: " + getString(R.string.connected);
                    status_action_item.setTitle(newStatus);
                } else {
                    String newStatus = "Status: " + getString(R.string.disconnected);
                    status_action_item.setTitle(newStatus);
                }
            }
        });
    }

    /*
    * Application of JNI code:
    */
    static {
        System.loadLibrary("ssvep-lib");
    }

    public native int jmainInitialization(boolean b);

    public native double[] jClassifySSVEP(double[] a, double[] b, double c);

    public native double[] jClassifySSVEP4k(double[] a, double[] b, double c);

    public native double[] jPSDExtraction(double[] a, double[] b, int sampleRate);

    public native double[] jLoadfPSD(int sampleRate);

}
